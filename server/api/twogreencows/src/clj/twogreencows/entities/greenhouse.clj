(ns twogreencows.entities.greenhouse
  (:require
    [twogreencows.db.core :as db]
    [clojure.java.io :as io]    
    [twogreencows.entities.util :as tgc-util]
    [twogreencows.middleware :as middleware]
    ))

(def greenhouse-object-version 1)
(def greenhouse-predix "grn")


;(defn get-greenhouse [uuid]
;  (db/get-greenhouse-by-uuid {:uuid uuid} ))

(defn greenhous-description [] 
  (assoc (assoc (assoc (assoc (tgc-util/tgc-entity-description) :creation_date string?) :display_name string?) :country string?) :phone string?))


