(ns twogreencows.entities.user
    (:require 
    [twogreencows.db.core :as db]
    ;;[buddy.hashers :as hashers]
    [malli.core :as m]
    [malli.error :as me]
    [malli.util :as mu]
    [malli.experimental.time :as mt]
    [twogreencows.entities.error :as tgc-error]
    [twogreencows.entities.token :as tgc-token]
    [twogreencows.entities.util :as tgc-util]))

(def user-data-version 1)
(def user-prefix "usr")

(def user-post-description
  [:and 
   [:map {:closed true}
              [:display_name [:string {:min 1}]]
              [:password [:string {:min 8}]]
              [:confirm_password :string]
              [:phone_number :string]]
        [:fn {:error/message "confirmation password does not match"
              :error/path [:confirm_password]}
         (fn [{:keys [password confirm_password]}]
         (= password confirm_password))]]
     )

(def user-description
    (mu/merge tgc-util/tgc-entity-description (m/schema [:map 
                                                         [:display_name string?] 
                                                         [:country string?] 
                                                         [:phone_number string?]
                                                         [:tokens {:optional true} [:vector tgc-token/token-description]]])))

(def user-subobjects {:tokens "owner_uuid" :devices "owner_uuid"})


(defn get-user-subobjects [user_uuid subobjectkeyword]
  (let [query (str "select * from " (name subobjectkeyword) " where " (str (user-subobjects subobjectkeyword)) "=?") 
        subobjects (db/execute-query [query user_uuid])]
      (identity subobjects)
  ))

(defn format-with-subobjects [user subobjects]
  (let [real_subobjects (filter #(contains? user-subobjects %) subobjects)]
    (merge user (reduce conj {} (zipmap real_subobjects (map (fn [u] (get-user-subobjects (user :uuid) u)) real_subobjects))))
  )
)


(defn user-list [] (db/execute-query ["select * from users"]))

(defn new-user! [params subobjects]
     (let [newuuid (str user-prefix "-" (clojure.string/replace (.toString (java.util.UUID/randomUUID)) #"-" "")) 
           tnow (java.time.LocalDateTime/now)
           [hsalt hpassword] (tgc-util/tgc-hash-password (params :password)) 
           newuser (db/execute-query ["insert into users (uuid, created_at, updated_at, data_version, object_version, country, phone_number, display_name, salt, password) 
                  values (?,?,?,?,?,?,?,?,?,?)", newuuid tnow tnow user-data-version 1 "FRA" (params :phone_number) (params :display_name) hsalt hpassword])
           u (get newuser 0)
           tmptoken (tgc-token/new-token! {:owner_uuid newuuid})]
                (format-with-subobjects u subobjects)
                ))


(defn get-user [uuid subobjects]
  (let [userquery "select * from users where uuid= ?"
        existing-users (db/execute-query [userquery uuid])]
      (if (not-empty existing-users)
        (do
          (prn subobjects)
          (prn existing-users)
          (format-with-subobjects (get existing-users 0) subobjects)
        )
        nil)
    ))

(defn delete-user [uuid]
  (db/execute-query ["delete from users where uuid=? returning *" uuid]))


(defn check-for-user [params subobjects]
  (let [userquery "select * from users where phone_number= ?"
        existing-users (db/execute-query [userquery (params :phone_number)])]
      (if (not-empty existing-users)
          (let [tmpuser (get existing-users 0)
                proposed-display-name (params :display_name)
                proposed-hashed-password (nth (tgc-util/tgc-hash-password (params :password) (tmpuser :salt)) 1)]
                   (if (and (= 0 (compare proposed-display-name (tmpuser :display_name))) (= 0 (compare proposed-hashed-password (tmpuser :password))))
                        (format-with-subobjects tmpuser subobjects)                        
                        false )) 
           nil)))

