(ns twogreencows.entities.user
    (:require 
    [twogreencows.db.core :as db]
    [buddy.hashers :as hashers]
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

(def user-description  
  (mu/merge tgc-util/tgc-entity-description (m/schema [:map 
                                                         [:country string?] 
                                                         [:display_name string?] 
                                                         [:phone_number string?]])))



(defn user-list [] (vec (db/get-users)))

(defn new-user! [params]
  (let [errors (m/validate params user-post-schema)]
    (println "USERPOST")

       
     
     ;;)
      ;(let [newuuid (str user-prefix "-" (clojure.string/replace (.toString (java.util.UUID/randomUUID)) #"-" ""))]
       ; (db/create-user! (assoc params :uuid newuuid
         ;   :data_version user-data-version :object_version 1 :country "FRA" :creation_date (java.time.LocalDateTime/now)))
       ; )
      )
  )
  

(defn get-user [uuid]
  (db/get-user-by-uuid {:uuid uuid} ))

;(defn delete-user [uuid]
;  (db/delete-user-with-uuid {:uuid uuid}))




