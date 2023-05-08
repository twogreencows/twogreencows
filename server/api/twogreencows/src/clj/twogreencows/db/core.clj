(ns twogreencows.db.core
  (:require
    [java-time :refer [java-date]]
    [next.jdbc.date-time :as jdt]
    [next.jdbc.result-set :as rs]
    [cheshire.core :refer [generate-string parse-string]]
    [next.jdbc :as jdbc]
    [clojure.tools.logging :as log]
    [conman.core :as conman]
    ;[twogreencows.entities.environment :as tgc-environment]
    [java-time :as jt]
    [twogreencows.config :refer [env]]
    [mount.core :refer [defstate]])
  (:import org.postgresql.util.PGobject
           java.sql.Array
           clojure.lang.IPersistentMap
           clojure.lang.IPersistentVector
           [java.sql
            BatchUpdateException
            PreparedStatement]))

(defstate ^:dynamic *db*
  :start (if-let [jdbc-url (env :database-sql-url)] 
           (let [*db* (jdbc/get-datasource jdbc-url)]
             (identity *db*))))

(defn execute-query 
  ([query_array] (execute-query query_array false))
  ([query_array with_transaction]
  (do
   (println (apply str query_array))
   (jdbc/execute! *db* query_array {:return-keys true :builder-fn rs/as-unqualified-lower-maps}))))

(jdt/read-as-instant)
;;(conman/bind-connection *db* "sql/queries.sql")
;(tgc-environment/unique-environment)

;;(defn sql-timestamp->inst [t] 
;;  (-> t
;;      ;;(identity)))
;;      (.toLocalDateTime)
;;      (.atZone (java.time.ZoneId/systemDefault))
;;      (java-date)))

;;(extend-protocol next.jdbc.result-set/ReadableColumn
;;    java.sql.Timestamp
;;    (read-column-by-label [^java.sql.Timestamp v _]
;;      (sql-timestamp->inst v))
;;    (read-column-by-index [^java.sql.Timestamp v _2 _3]
;;      (sql-timestamp->inst v))
;;    java.sql.Date
;;    (read-column-by-label [^java.sql.Date v _]
;;      (.toLocalDate v))
;;   (read-column-by-index [^java.sql.Date v _2 _3]
;;     (.toLocalDate v))
;;    java.sql.Time
;;    (read-column-by-label [^java.sql.Time v _]
;;      (.toLocalTime v))
;;    (read-column-by-index [^java.sql.Time v _2 _3]
;;      (.toLocalTime  v)))



;;(defn to-pg-json [value]
;;  (doto (PGobject.)
;;    (.setType "jsonb")
;;    (.setValue (generate-string value))))

;;(extend-type clojure.lang.IPersistentVector
 ;; jdbc/ISQLParameter
;;  (set-parameter [v ^java.sql.PreparedStatement stmt ^long idx]
;;    (let [conn      (.getConnection stmt)
;;          meta      (.getParameterMetaData stmt)
;;          type-name (.getParameterTypeName meta idx)]
;;      (if-let [elem-type (when (= (first type-name) \_) (apply str (rest type-name)))]
;;        (.setObject stmt idx (.createArrayOf conn elem-type (to-array v)))
;;        (.setObject stmt idx (to-pg-json v))))))

;;(extend-protocol jdbc/ISQLValue
;;    java.util.Date
;;  (sql-value [v]
;;    (java.sql.Timestamp. (.getTime v)))
;;  java.time.LocalTime
;;  (sql-value [v]
;;    (jt/sql-time v))
;;  java.time.LocalDate
;;  (sql-value [v]
;;    (jt/sql-date v))
;;  java.time.LocalDateTime
;;  (sql-value [v]
;;    (jt/sql-timestamp v))
;;  java.time.ZonedDateTime
;;  (sql-value [v]
;;    (jt/sql-timestamp v))
;;  IPersistentMap
;;  (sql-value [value] (to-pg-json value))
;;  IPersistentVector
;;  (sql-value [value] (to-pg-json value)))
;;;

