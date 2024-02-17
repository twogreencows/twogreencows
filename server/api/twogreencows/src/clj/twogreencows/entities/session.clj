(ns twogreencows.entities.session
  (:require
    [twogreencows.db.core :as db]
    [clojure.java.io :as io]    
    [malli.core :as m]
    [malli.error :as me]
    [malli.util :as mu]
    [malli.experimental.time :as mt]
    [twogreencows.middleware :as middleware]
    [twogreencows.entities.util :as tgc-util]
    [twogreencows.entities.user :as tgc-user]
    [twogreencows.entities.token :as tgc-token]
    [twogreencows.entities.device :as tgc-device]
    ))

(def session-data-version 1)
(def session-prefix "ses")
(def current-version "0.0.8")


(def session-post-description 
  [:and 
    [:map   
         [:user 
           [:map
              [:display_name {:min 1 :optional true} :string]
              [:password {:min 8} :string]
              [:confirm_password {:optional true} :string]
              [:phone_number {:optional true} :string]
              [:email {:optional true} :string]]]
          [:device
            [:map
              [:kind {:min 4 :max 4} :string]
              [:platform {:min 1 :optional true} :string]
              [:vendor_uuid {:optional true} :string]
              [:os_version {:optional true} :string]
              [:display_name {:optional true} :string] ]]
          ]
 
      [:fn {:error/message "Missing one of the needed identifier"
            :error/path [:user :email]}
         (fn [{{:keys [email phone_number display_name]} :user}]
          (or (some? email) (some? phone_number) (some? display_name)))]

      [:fn {:error/message "Missing one of the needed identifier"
            :error/path [:user :phone_number]}
         (fn [{{:keys [email phone_number display_name]} :user}]
          (or (some? email) (some? phone_number) (some? display_name)))]
     
      [:fn {:error/message "Missing one of the needed identifier"
            :error/path [:user :display_name]}
         (fn [{{:keys [email phone_number display_name]} :user}]
          (or (some? email) (some? phone_number) (some? display_name)))]
 ] 
)

(def session-description
    (mu/merge tgc-util/tgc-entity-description (m/schema [:map
                                                         [:user tgc-user/user-description] 
                                                         [:terminated_at :string] 
                                                         [:device tgc-device/device-description] 
                                                         [:token_uuid :string] 
                                                         [:is_new_user :boolean]
                                                         [:is_new_device :boolean]
                                                         ])))


(def session-subobjects {:user "user" :device "device"})

(defn get-session-subobjects [session_uuid subobjectkeyword]
  (let [query (str "select * from " (name subobjectkeyword) " where " (str (session-subobjects subobjectkeyword) "_uuid") "=?") 
        subobjects (db/execute-query [query session_uuid])]
      (identity subobjects)
  ))



(defn format-with-subobjects 
  ([session] (format-with-subobjects session []))
  ([session subobjects]
  (let [real_subobjects (filter #(contains? session-subobjects %) subobjects)]
    (merge (into {} (remove (fn [[k v]] (nil? v)) session )) (reduce conj {} (zipmap real_subobjects (map (fn [s] (get-session-subobjects (session :uuid) s)) real_subobjects))))
  ))
)



(defn session-list [subobjects] 
  (let [sessions (db/execute-query ["select * from sessions"])]
    sessions))


(defn new-session! [params subobjects]
     (let [tmpuser (if-let [tmpuser (tgc-user/check-for-user (params :user) [:tokens])] (identity tmpuser) (tgc-user/new-user! (params :user) [:tokens]))
           tmpdevice (if-let [tmpdevice (tgc-device/check-for-device (params :device) [:token])] (identity tmpdevice) (tgc-device/new-device! (assoc (params :device) :owner_uuid (tmpuser :uuid))[:token]))]
       (do
         (println "LALALA")
         (println tmpuser)
         (println tmpdevice)
         (println "LALALA")
       (let
           [tmptoken (tgc-token/new-token! {:owner_uuid (tmpuser :uuid) :kind "sess"})
           newuuid (str session-prefix "-" (clojure.string/replace (.toString (java.util.UUID/randomUUID)) #"-" "")) 
           tnow (java.time.LocalDateTime/now)
           newsession (db/execute-query ["insert into sessions (uuid, created_at, updated_at, data_version, object_version, user_uuid, device_uuid, token_uuid, is_new_user, is_new_device) 
                  values (?,?,?,?,?,?,?,?,?,?)", newuuid tnow tnow session-data-version 1 (tmpuser :uuid) (tmpdevice :uuid) (tmptoken :uuid)  false false])
           s (get newsession 0)
           ]
       
                (do  
                  (println "lololo")
                (format-with-subobjects s subobjects)
                
                )

                ))))


(defn get-session [uuid subobjects]
  (let [sessionquery "select * from sessions where uuid= ?"
        existing-sessions (db/execute-query [sessionquery uuid])]
      (if (not-empty existing-sessions)
        (format-with-subobjects (get existing-sessions 0) subobjects)
        nil)
    ))

(defn delete-session [uuid]
  (db/execute-query ["delete from sessions where uuid=? returning *" uuid]))


