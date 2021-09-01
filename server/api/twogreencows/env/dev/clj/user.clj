(ns user
  (:require
    [twogreencows.config :refer [env]]
    [clojure.spec.alpha :as s]
    [expound.alpha :as expound]
    [mount.core :as mount]
    [twogreencows.core :refer [start-app]]
    [twogreencows.db.core]
    [conman.core :as conman]
    [luminus-migrations.core :as migrations]))

(alter-var-root #'s/*explain-out* (constantly expound/printer))

(add-tap (bound-fn* clojure.pprint/pprint))

(defn start []
  (mount/start-without #'twogreencows.core/repl-server))

(defn stop []
  (mount/stop-except #'twogreencows.core/repl-server))

(defn restart []
  (stop)
  (start))

(defn restart-db []
  (mount/stop #'twogreencows.db.core/*db*)
  (mount/start #'twogreencows.db.core/*db*)
  (binding [*ns* (the-ns 'twogreencows.db.core)]
    (conman/bind-connection twogreencows.db.core/*db* "sql/queries.sql")))

(defn reset-db []
  (migrations/migrate ["reset"] (select-keys env [:database-url])))

(defn migrate []
  (migrations/migrate ["migrate"] (select-keys env [:database-url])))

(defn rollback []
  (migrations/migrate ["rollback"] (select-keys env [:database-url])))

(defn create-migration [name]
  (migrations/create name (select-keys env [:database-url])))


