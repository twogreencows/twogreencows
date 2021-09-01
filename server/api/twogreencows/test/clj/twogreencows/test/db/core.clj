(ns twogreencows.test.db.core
  (:require
    [twogreencows.db.core :refer [*db*] :as db]
    [luminus-migrations.core :as migrations]
    [clojure.test :refer :all]
    [clojure.java.jdbc :as jdbc]
    [twogreencows.config :refer [env]]
    [mount.core :as mount]))

(use-fixtures
  :once
  (fn [f]
    (mount/start
      #'twogreencows.config/env
      #'twogreencows.db.core/*db*)
    (migrations/migrate ["migrate"] (select-keys env [:database-url]))
    (f)))

(deftest test-users
  (jdbc/with-db-transaction [t-conn *db*]
    (jdbc/db-set-rollback-only! t-conn)
    (is (= 1 (db/create-user!
               t-conn
               {:uuid         (str "u-" (clojure.string/replace (.toString (java.util.UUID/randomUUID)) #"-" ""))
                :object_version 1
                :data_version   1
                :display_name  "laurent"
                :creation_date  (java.time.LocalDateTime/now)
                :password      "toto"
                :phone         "+33687853131"
                :country       "FRA"})))
    ;;(is (= {
    ;;        :first_name "Sam"
    ;;       :last_name  "Smith"
    ;;        :email      "sam.smith@example.com"
    ;;        :pass       "pass"
    ;;        :admin      nil
    ;;        :last_login nil
    ;;        :is_active  nil}
    ;;       (db/get-user t-conn {:phone "+33687853131"})))
    
    ))
