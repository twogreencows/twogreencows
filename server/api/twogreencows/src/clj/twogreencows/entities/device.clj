(ns twogreencows.entities.device
  (:require
    [twogreencows.db.core :as db]
    [twogreencows.db.corecassandra :as dbcassandra]
    [malli.core :as m]
    [malli.error :as me]
    [malli.util :as mu]
    [malli.experimental.time :as mt]
    [twogreencows.entities.error :as tgc-error]
    [twogreencows.entities.util :as tgc-util]
    [twogreencows.entities.token :as tgc-token]
    [twogreencows.middleware :as middleware]
    ))

(def device-object-version 1)
(def device-prefix "dev")
(def devices-subobjects {:token "devices_tokens"})

(def device-post-description
  [:and 
   [:map 
              [:kind {:min 4 :max 4} :string]
              [:platform {:min 1} :string]
              [:vendor_uuid {:optional true} :string]
              [:os_version {:optional true} :string]
              [:display_name :string]
              [:owner_uuid {:optional true} :string]
              ]
        
      [:fn {:error/message "Unknown kind"
            :error/path [:kind]}
         (fn [{:keys [kind]}]
          (contains? #{"desk" "mobi" "web*" "embd"} kind))]

     [:fn {:error/message "Incompatible parameters"
            :error/path [:kind :vendor_uuid]}
         (fn [{:keys [kind vendor_uuid]}]
           (if (not (= kind "web*")) (if (nil? vendor_uuid) false true) true) 
          )]
     ] 
     )

(def device-description
    (mu/merge tgc-util/tgc-entity-description (m/schema [:map 
                                                         [:kind :string] 
                                                         [:vendor_uuid {:optional true} :string] 
                                                         [:display_name :string] 
                                                         [:os_version {:optional true} :string]
                                                         [:platform :string]
                                                         [:last_connection_date :time/instant]
                                                         [:token {:optional true} [:or tgc-token/token-description :nil]]                                          
                                                         [:owner_uuid :string] 
                                                         ])))

(defn get-device-subobjects [device_uuid subobjectkeyword]
  ;; we make the assumption there will be only :token for now. TO be refactored
  (let [query "select * from tokens t inner join devices_tokens dt on dt.token_uuid = t.uuid where dt.device_uuid=?" 
        subobjects (db/execute-query [query device_uuid])]
      (nth subobjects 0 nil))
  )

(defn format-with-subobjects 
  ([device] (format-with-subobjects device  []))
  ([device subobjects]
  (let [real_subobjects (filter #(contains? devices-subobjects %) subobjects)]
    (do    
      (merge (into {} (remove (fn [[k v]] (nil? v)) device)) (reduce conj {} (zipmap real_subobjects (map (fn [u] (get-device-subobjects (device :uuid) u)) real_subobjects)))))
  ))
)


(defn device-list [subobjects] 
  (let [devices (db/execute-query ["select * from devices"])]
       (map (fn [d] (format-with-subobjects d subobjects)) devices) 
    ))

(defn new-device! [params subobjects]
     (let [newuuid (str device-prefix "-" (clojure.string/replace (.toString (java.util.UUID/randomUUID)) #"-" "")) 
           tnow (java.time.LocalDateTime/now)
           display_name (get params :display_name)
           kind (get params :kind "web*")
           os_version (get params :os_version)
           owner_uuid (get params :owner_uuid)
           platform (get params :platform)
           vendor_uuid (get params :vendor_uuid "---")
           tusage (get params :last_connection_date (java.time.LocalDateTime/ofEpochSecond 0,0, java.time.ZoneOffset/UTC))
           newdevice (db/execute-query ["insert into devices (uuid, created_at, updated_at, data_version, object_version, owner_uuid, display_name, kind, os_version, platform, vendor_uuid, last_connection_date ) values (?,?,?,?,?,?,?,?,?,?,?,?)", newuuid tnow tnow 1 device-object-version owner_uuid display_name kind os_version platform vendor_uuid tusage])
           d (get newdevice 0)]
                (format-with-subobjects d subobjects)
                ))


(defn get-device [uuid subobjects]
  (let [devicequery "select * from devices where uuid= ?"
        existing-devices (db/execute-query [devicequery uuid])]
      (if (not-empty existing-devices)
        (format-with-subobjects (get existing-devices 0) subobjects)
        nil)
    ))

(defn delete-device [uuid]
  (db/execute-query ["delete from devices where uuid=? returning *" uuid]))


(defn check-for-device [params subobjects]
 (let [devicequerykey :vendor_uuid 
       devicequery  (str "select * from devices where " (name devicequerykey)  "= ?")
       existing-devices (db/execute-query [devicequery (params devicequerykey)])]
      (if (not-empty existing-devices)
          (let [tmpdevice (get existing-devices 0)]       
                   (if (and (= 0 (compare (params :owner_uuid) (tmpdevice :owner_uuid))) (= 0 (compare (params :kind) (tmpdevice :kind))))

                        (format-with-subobjects tmpdevice subobjects)                        
                        false )) 
           nil)))




