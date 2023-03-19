(ns twogreencows.entities.session
  (:require
    [twogreencows.db.core :as db]
    [clojure.java.io :as io]    
    [malli.core :as m]
    [malli.error :as me]
    [malli.experimental.time :as mt]
    [twogreencows.middleware :as middleware]
    [twogreencows.entities.util :as tgc-util]
    ))

(def session-object-version 1)
(def session-prefix "ses")
(def current-version "0.0.8")

;(defn get-session [uuid]
;  (db/get-session-by-uuid {:uuid uuid} ))


