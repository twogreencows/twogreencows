(ns twogreencows.entities.device
  (:require
    [twogreencows.db.core :as db]
    [twogreencows.middleware :as middleware]
    ))

(def device-object-version 1)
(def datapoint-prefix "dev")

(defn get-device [uuid]
  (db/get-device-by-uuid {:uuid uuid} ))


