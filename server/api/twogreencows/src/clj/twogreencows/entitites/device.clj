(ns twogreencows.entities.device
  (:require
    [twogreencows.db.core :as db]
    [clojure.java.io :as io]    
    [twogreencows.middleware :as middleware]
    ))

(def device-object-version 1)

(defn get-device [uuid]
  (db/get-device-by-uuid {:uuid uuid} ))


