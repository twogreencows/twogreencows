(ns twogreencows.users
    (:require 
    [twogreencows.db.core :as db]
    [buddy.hashers :as hashers]
    [twogreencows.validation :refer [validate-user]]))

(def user-data-version 1)

(defn user-list []
  {:users (vec (db/get-users))})
  ;(vec (db/get-users)))

(defn new-user! [params]
  (if-let [errors (validate-user params)]
     (throw (ex-info "User parameters are invalid" {:twogreencows/error-id :validation :errors errors}))
    (try
      (let [newuuid (str "u-" (clojure.string/replace (.toString (java.util.UUID/randomUUID)) #"-" ""))]
        (db/create-user! (assoc params :uuid newuuid
            :data_version user-data-version :object_version 1 :country "FRA" :creation_date (java.time.LocalDateTime/now)))
        (db/get-user-by-uuid {:uuid newuuid})))))

(defn get-user [uuid]
  (db/get-user-by-uuid {:uuid uuid} ))

;(defn delete-user [uuid]
;  (db/delete-user-with-uuid {:uuid uuid}))

