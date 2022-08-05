(ns twogreencows.entities.error
  (:require
    [twogreencows.db.core :as db]
    [twogreencows.middleware :as middleware]
    [twogreencows.entities.util :as tgc-util]
  ))

(def error-data-version 1)
(def error-prefix "err")

(defn error-description [] 
  (assoc (assoc (tgc-util/tgc-entity-description) :message string?) :code int?))

(defn create-error [code message] 
   (let [tmpuuid (str error-prefix "-" (clojure.string/replace (.toString (java.util.UUID/randomUUID)) #"-" ""))]
            {:message message :code code :uuid tmpuuid :object_version 1 :data_version error-data-version}))
