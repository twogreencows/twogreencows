(ns twogreencows.entities.device
  (:require
    [twogreencows.db.core :as db]
    [twogreencows.db.corecassandra :as dbcassandra]
    [twogreencows.middleware :as middleware]
    ))

(def device-object-version 1)
(def datapoint-prefix "dev")

(defn get-device [uuid]
  (db/execute-query [(str "select * from devices where uuid=" uuid)]))


