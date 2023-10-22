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
(def devices-subobjects {})

(def device-post-description
  [:and 
   [:map 
              [:kind {:min 4 :max 4} :string]
              [:platform {:min 1} :string]
              [:vendor_uuid {:optional true}:string]
              [:os_version :string]
              [:display_name :string]
              ]
        
      [:fn {:error/message "Unknown kind"
            :error/path [:kind]}
         (fn [{:keys [kind]}]
          (contains? #{"desk" "mobi" "web*" "embd"} kind))]

     [:fn {:error/message "Imcompatible parameters"
            :error/path [:kind :vendor_uuid]}
         (fn [{:keys [kind vendor_uuid]}]
           (if (not (= kind "web*")) (if (nil? vendor_uuid) false true) true) 
          )]
     ] 
     )

(def device-description
    (mu/merge tgc-util/tgc-entity-description (m/schema [:map 
                                                         [:kind :string] 
                                                         [:vendor_uuid :string] 
                                                         [:display_name :string] 
                                                         [:os_version :string]
                                                         [:platform :string]
                                                         [:last_connection_date :time/instant]
                                                         [:token {:optional true} tgc-token/token-description]                                          
                                                         [:owner_uuid :string] 
                                                         ])))

(defn get-device-subobjects [device_uuid subobjectkeyword]
  (let [query (str "select * from " (name subobjectkeyword) " where " (str (devices-subobjects subobjectkeyword)) "=?") 
        subobjects (db/execute-query [query device_uuid])]
      (identity subobjects)
  ))

(defn format-with-subobjects 
  ([device] (format-with-subobjects device  []))
  ([device subobjects]
  (let [real_subobjects (filter #(contains? devices-subobjects %) subobjects)]
    (merge (into {} (remove (fn [[k v]] (nil? v)) device)) (reduce conj {} (zipmap real_subobjects (map (fn [u] (get-device-subobjects (device :uuid) u)) real_subobjects))))
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


;(defn check-for-device [params subobjects]
; (let [userquerykey (if (some? (params :phone_number)) :phone_number (if (some? (params :email)) :email :display_name ))
;       userquery  (str "select * from users where " (name userquerykey)  "= ?")
;       existing-users (db/execute-query [userquery (params userquerykey)])]
;      (if (not-empty existing-users)
;          (let [tmpuser (get existing-users 0)
;                proposed-hashed-password (nth (tgc-util/tgc-hash-password (params :password) (tmpuser :salt)) 1)]
;                   (if (= 0 (compare proposed-hashed-password (tmpuser :password)))
;                        (format-with-subobjects tmpuser subobjects)                        
;                        false )) 
;           nil)))



;;   owner_uuid CHAR(36),
    
;;    kind VARCHAR(8),
;;    vendor_uuid CHAR(36),
;;    platform VARCHAR(8),  
;;    os_version VARCHAR(16),

