(ns twogreencows.entities.greenhouse
  (:require
    [twogreencows.db.core :as db]
    [clojure.java.io :as io]    
    [twogreencows.middleware :as middleware]
    ))

(def greenhouse-object-version 1)
(def greenhouse-predix "grn")


;(defn get-greenhouse [uuid]
;  (db/get-greenhouse-by-uuid {:uuid uuid} ))


