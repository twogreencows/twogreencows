(ns user
  (:require
    [twogreenpots.config :refer [env]]
    [clojure.spec.alpha :as s]
    [expound.alpha :as expound]
    [mount.core :as mount]
    [twogreenpots.core :refer [start-app]]
    [twogreenpots.db.core]
    [conman.core :as conman]
    [luminus-migrations.core :as migrations]))

(alter-var-root #'s/*explain-out* (constantly expound/printer))

(defn start []
  (mount/start-without #'twogreenpots.core/repl-server))

(defn stop []
  (mount/stop-except #'twogreenpots.core/repl-server))

(defn restart []
  (stop)
  (start))

(defn restart-db []
  (mount/stop #'twogreenpots.db.core/*db*)
  (mount/start #'twogreenpots.db.core/*db*)
  (binding [*ns* 'twogreenpots.db.core]
    (conman/bind-connection twogreenpots.db.core/*db* "sql/queries.sql")))

(defn reset-db []
  (migrations/migrate ["reset"] (select-keys env [:database-url])))

(defn migrate []
  (migrations/migrate ["migrate"] (select-keys env [:database-url])))

(defn rollback []
  (migrations/migrate ["rollback"] (select-keys env [:database-url])))

(defn create-migration [name]
  (migrations/create name (select-keys env [:database-url])))


