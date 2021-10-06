(ns twogreencows.entities.growbox
  (:require
    [twogreencows.db.core :as db]
    [clojure.java.io :as io]    
    [twogreencows.middleware :as middleware]
    ))

(def growbox-object-version 1)

(defn get-growbox [uuid]
  (db/get-growbox-by-uuid {:uuid uuid} ))


