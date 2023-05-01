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
           [hsalt hpassword] (tgc-util/tgc-hash-passwword (params :password)) 
           newuser (db/execute-query ["insert into users (uuid, created_at, updated_at, data_version, object_version, country, phone_number, display_name, salt, password) 
                  values (?,?,?,?,?,?,?,?,?,?)", newuuid tnow tnow user-data-version 1 "FRA" (params :phone_number) (params :display_name) hsalt hpassword])]
             (get newuser 0) 
          ) 
      )
  

(defn get-user [uuid]
  (db/execute-query [(str "select * from users where uuid="  uuid)]))


(defn check-for-user [params]
  (let [existingusers (db/execute-query (str "select * from users where phone_number=" (params :phone_number)))]
      (if (not-empty existingusers) 
          (let [tmpuser (get existingusers 0)]
            (identity tmpuser)))))
            ;;(if  (= (hashers/derive (tmpuser :password) {:alg :bcrypt+sha512}) (params :password))
              ;; tmpuser  false)) nil)))       



