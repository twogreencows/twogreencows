(ns twogreencows.db.corecassandra
  (:require
   [qbits.alia :as alia]
    [clojure.tools.logging :as log]
   [twogreencows.config :refer [env]]
   [mount.core :refer [defstate]]))

(defstate ^:dynamic *db-cassandra*
  :start (log/info "=[twogreencows] DB Cassandra started successfully ")
  :stop (println "Stop cassandra"))



