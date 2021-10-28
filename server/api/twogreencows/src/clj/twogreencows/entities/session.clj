(ns twogreencows.entities.session
  (:require
    [twogreencows.db.core :as db]
    [clojure.java.io :as io]    
    [twogreencows.middleware :as middleware]
    ))

(def session-object-version 1)
(def session-prefix "ses")

;(defn get-session [uuid]
;  (db/get-session-by-uuid {:uuid uuid} ))


