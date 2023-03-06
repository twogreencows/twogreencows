(ns twogreencows.entities.user
    (:require 
    [twogreencows.db.core :as db]
    [buddy.hashers :as hashers]
    [malli.core :as m]
    [twogreencows.entities.error :as tgc-error]
    [twogreencows.entities.util :as tgc-util]))

(def user-data-version 1)
(def user-prefix "usr")

(def user-schema
  [[:display_name st/required st/string]
   [:password st/required st/string {:message "Password too short, should be 8 minimum" :validate (fn [msg] (>= (count msg) 8))}]
   [:confirm_password st/required st/string [st/identical-to :password]]
   [:phone_number st/string st/required]
   ])

 

(defn user-list [] (vec (db/get-users)))

(defn new-user! [params]
  (let [[errors _] (st/validate params user-schema)]

    (if (nil? errors)
     (println "no error") 
     (tgc-error/create-error 400 (str errors))
       
     
     )
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



(defn user-description [] 
  (assoc (assoc (assoc (assoc (tgc-util/tgc-entity-description) :creation_date inst?) :display_name string?) :country string?) :phone_number string?))


(defn user-post-description [] {:display_name string? :password string? :confirm_password string? :phone_number string?})
