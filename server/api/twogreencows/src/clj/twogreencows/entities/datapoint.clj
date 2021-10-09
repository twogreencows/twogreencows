(ns twogreencows.entities.datapoint
  (:require
    [twogreencows.db.core :as db]
    [twogreencows.middleware :as middleware]
    ))

(def datapoint-object-version 1)
(def datapoint-prefix "dpt")
