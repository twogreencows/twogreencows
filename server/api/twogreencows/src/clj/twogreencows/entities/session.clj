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
              [:platform {:min 1} :string]
              [:vendor_uuid :string]
              [:os_version :string]
              [:display_name :string] ]]]
 
        
      [:fn {:error/message "Missing one of the needed identifier"
            :error/path [:email :display_name :phone_number]}
         (fn [{:keys [email phone_number display_name]}]
          (or (some? email) (some? phone_number) (some? display_name)))]
 ] 
)

(def session-description
    (mu/merge tgc-util/tgc-entity-description (m/schema [:map 
                                                         [:user string?] 
                                                         [:terminated_at string?] 
                                                         [:deviceh string?] 
                                                         [:token_uuid string?] 
                                                         [:is_new_user boolean?]
                                                         [:is_new_device boolean?]
                                                         ])))


(def session-subobjects {:user "user_uuid" :device "device_uuid"})

(defn get-session-subobjects [session_uuid subobjectkeyword]
  (let [query (str "select * from " (name subobjectkeyword) " where " (str (session-subobjects subobjectkeyword)) "=?") 
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
     (let [newuuid (str session-prefix "-" (clojure.string/replace (.toString (java.util.UUID/randomUUID)) #"-" "")) 
           tnow (java.time.LocalDateTime/now)
           newsession (db/execute-query ["insert into sessions (uuid, created_at, updated_at, data_version, object_version, user_uuid, device_uuid, token_uuid, is_new_user, is_new_device) 
                  values (?,?,?,?,?,?,?,?,?,?)", newuuid tnow tnow session-data-version 1 "usr-kkkkk" "dev-llll" "tok-jjjjj" false false])
           s (get newsession 0)
           ]
                (format-with-subobjects s subobjects)
                ))


(defn get-session [uuid subobjects]
  (let [sessionquery "select * from sessions where uuid= ?"
        existing-sessions (db/execute-query [sessionquery uuid])]
      (if (not-empty existing-sessions)
        (format-with-subobjects (get existing-sessions 0) subobjects)
        nil)
    ))

(defn delete-session [uuid]
  (db/execute-query ["delete from sessions where uuid=? returning *" uuid]))


