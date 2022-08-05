(ns twogreencows.db.core-cassandra
  (:require
   [qbits.alia :as alia]
   [twogreencows.config :refer [env]]
   [mount.core :refer [defstate]]))

(defstate ^:dynamic *db-cassandra*
  :start (println "Start cassandra")
  :stop (println "Stop cassandra")))



