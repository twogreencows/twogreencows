(ns twogreencows.entities.user
    (:require 
    [twogreencows.db.core :as db]
    [malli.core :as m]
    [malli.error :as me]
    [malli.util :as mu]
    [malli.experimental.time :as mt]
    [twogreencows.entities.error :as tgc-error]
    [twogreencows.entities.token :as tgc-token]
    [twogreencows.entities.util :as tgc-util])
    (:import 
      [com.google.i18n.phonenumbers PhoneNumberUtil]) 
    )

(def user-data-version 1)
(def user-prefix "usr")

(def user-level-none 0)
(def user-level-grower 4)
(def user-level-housesadmin 32)
(def user-level-superadmin 128)

(def user-admin-data {:display_name "admin" :password "admin" :phone_number "+3300000000" :user_level user-level-superadmin :country "FRA" :email "admin@twogreencows.local"})

(def userwords_1 ["little" "big" "nice" "grumpy" "sad" "funny" "sleepy" "laughing" "joyous" "crying" "maniac" "fighting" "cheerful" "gloomy" "calm"])
(def userwords_2  ["joe" "angela" "lucy" "bob" "jack" "mary" "desmond" "stella" "harry" "tilda" "bill" "scarlett"])

(defn generate-user-name []
  (str (rand-nth userwords_1) "-" (rand-nth userwords_2) "-" (int (rand 27130410))))


(def user-post-description
  [:and 
   [:map 
              [:display_name {:min 1 :optional true} :string]
              [:password {:min 8} :string]
              [:confirm_password {:optional true} [:maybe :string]]
              [:phone_number {:optional true} :string]
              [:email {:optional true} :string]]
        
      [:fn {:error/message "Missing one of the needed identifier"
            :error/path [:email]}
         (fn [{:keys [email phone_number display_name]}]
          (or (some? email) (some? phone_number) (some? display_name)))]
      
      [:fn {:error/message "Missing one of the needed identifier"
            :error/path [:display_name]}
         (fn [{:keys [email phone_number display_name]}]
          (or (some? email) (some? phone_number) (some? display_name)))]
     
      [:fn {:error/message "Missing one of the needed identifier"
            :error/path [:phone_number]}
         (fn [{:keys [email phone_number display_name]}]
          (or (some? email) (some? phone_number) (some? display_name)))]
 
      [:fn {:error/message "password is too short or is not having a digit"
              :error/path [:password]}
         (fn [{:keys [password]}]
          (and (boolean (re-find #"\d" password)) (>= (count password) 8 )))]
 
      [:fn {:error/message "confirmation password does not match"
              :error/path [:confirm_password]}
         (fn [{:keys [password confirm_password]}]
         (= password confirm_password))]
        ]
     )

(def user-description
    (mu/merge tgc-util/tgc-entity-description (m/schema [:map 
                                                         [:display_name string?] 
                                                         [:country string?] 
                                                         [:phone_number {:optional true} string?]
                                                         [:email {:optional true} string?]
                                                         [:user_level int?] 
                                                         [:tokens {:optional true} [:vector tgc-token/token-description]]])))

(def user-subobjects  {:tokens  "owner_uuid" :devices "owner_uuid"})


(defn get-user-subobjects [user_uuid subobjectkeyword]
  (let [query (str "select * from " (name subobjectkeyword) " where " (str (user-subobjects subobjectkeyword)) "=?") 
        subobjects (db/execute-query [query user_uuid])]
      (identity subobjects)
      
  ))

(defn format-with-subobjects 
  ([user] (format-with-subobjects user []))
  ([user subobjects]
  (let [real_subobjects (filter #(contains? user-subobjects %) subobjects)]
    (merge (into {} (remove (fn [[k v]] (nil? v)) user )) (reduce conj {} (zipmap real_subobjects (map (fn [u] (get-user-subobjects (user :uuid) u)) real_subobjects)))
  )))
)


(defn user-list [subobjects] 
  (let [users (db/execute-query ["select * from users"])]
      (map (fn [u] (format-with-subobjects u subobjects)) users) 
    ))

(defn new-user! [userparams subobjects]
     (let [newuuid (str user-prefix "-" (clojure.string/replace (.toString (java.util.UUID/randomUUID)) #"-" "")) 
           tnow (java.time.LocalDateTime/now)
           display_name (get userparams :display_name (generate-user-name))
           phone_number (get userparams :phone_number)
           email (get userparams :email)
           user_level (get userparams :user_level user-level-none)
           [hsalt hpassword] (tgc-util/tgc-hash-password (userparams :password))]

        (let [newuser (db/execute-query ["insert into users (uuid, created_at, updated_at, data_version, object_version, country, phone_number, display_name, email, user_level, salt, password) values (?,?,?,?,?,?,?,?,?,?,?,?)", newuuid tnow tnow user-data-version 1 "FRA" phone_number display_name email user_level hsalt hpassword])
           u (get newuser 0)
           tmptoken (tgc-token/new-token! {:owner_uuid newuuid :kind "devc"})]
                (format-with-subobjects u subobjects)
                )))


(defn get-user [uuid subobjects]
  (let [userquery "select * from users where uuid= ?"
        existing-users (db/execute-query [userquery uuid])]
      (if (not-empty existing-users)
        (format-with-subobjects (get existing-users 0) subobjects)
        nil)
    ))

(defn delete-user [uuid]
  (db/execute-query ["delete from users where uuid=? returning *" uuid]))



(defn check-for-user 
  ([params] (check-for-user [:tokens :devices]))
  ([params subobjects]
    (let [userquerykey (if (some? (params :phone_number)) :phone_number (if (some? (params :email)) :email :display_name ))
       userquery  (str "select * from users where " (name userquerykey)  "= ?")
       existing-users (db/execute-query [userquery (params userquerykey)])]
      (if (not-empty existing-users)
          (let [tmpuser (get existing-users 0)
                proposed-hashed-password (nth (tgc-util/tgc-hash-password (params :password) (tmpuser :salt)) 1)]
                   (if (= 0 (compare proposed-hashed-password (tmpuser :password)))
                        [:exist (format-with-subobjects tmpuser subobjects)]                        
                        [:conflict tmpuser])) 
           (identity [:absent nil]) )))
)

