(ns twogreencows.entities.user
    (:require 
    [twogreencows.db.core :as db]
    ;;[buddy.hashers :as hashers]
    [malli.core :as m]
    [malli.error :as me]
    [malli.util :as mu]
    [malli.experimental.time :as mt]
    [twogreencows.entities.error :as tgc-error]
    [twogreencows.entities.util :as tgc-util]))

(def user-data-version 1)
(def user-prefix "usr")

(def user-post-schema
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
                                                         [:phone_number string?]])))


(defn user-list [] (db/execute-query ["select * from users"]))

(defn new-user! [params]
     (let [newuuid (str user-prefix "-" (clojure.string/replace (.toString (java.util.UUID/randomUUID)) #"-" "")) 
           tnow (java.time.LocalDateTime/now)
           [hsalt hpassword] (tgc-util/tgc-hash-password (params :password)) 
           newuser (db/execute-query ["insert into users (uuid, created_at, updated_at, data_version, object_version, country, phone_number, display_name, salt, password) 
                  values (?,?,?,?,?,?,?,?,?,?)", newuuid tnow tnow user-data-version 1 "FRA" (params :phone_number) (params :display_name) hsalt hpassword])]
             (get newuser 0) 
          ) 
      )
  

(defn get-user [uuid]
  (db/execute-query [(str "select * from users where uuid="  uuid)]))


(defn check-for-user [params]
  (let [existing-users (db/execute-query ["select * from users where phone_number= ?" (params :phone_number)])]
      (do
      (if (not-empty existing-users) 
          (let [tmpuser (get existing-users 0)
                proposed-display-name (params :display_name)
                proposed-hashed-password (nth (tgc-util/tgc-hash-password (params :password) (tmpuser :salt)) 1)]
                   (do 
                     (println proposed-hashed-password)
                     (println (tmpuser :password))
                     (println (compare proposed-hashed-password (tmpuser :password)))
                   (if (and (= 0 (compare proposed-display-name (tmpuser :display_name))) (= 0 (compare proposed-hashed-password (tmpuser :password))))
                        tmpuser
                        false))) 
                    nil
      
                ))))

