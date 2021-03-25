(ns twogreenpots.users
    (:require 
    [twogreenpots.db.core :as db]
    [twogreenpots.validation :refer [validate-user]]))


(defn user-list []
  {:users (vec (db/get-users))})

(defn new-user! [params]
  (if-let [errors (validate-user params)]
     (throw (ex-info "User parameters are invalid" {:twogreenpots/error-id :validation :errors errors}))
    (try
      (let [newuuid (str "u-" (clojure.string/replace (.toString (java.util.UUID/randomUUID)) #"-" ""))]
        (db/create-user! (assoc params :uuid newuuid
            :data_version 1 :object_version 1 :country "FRA" :creation_date (java.time.LocalDateTime/now)))
        (db/get-user-by-uuid {:uuid newuuid})))))

