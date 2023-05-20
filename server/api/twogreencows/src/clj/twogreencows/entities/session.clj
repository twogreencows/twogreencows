(ns twogreencows.entities.session
  (:require
    [twogreencows.db.core :as db]
    [clojure.java.io :as io]    
    [malli.core :as m]
    [malli.error :as me]
    [malli.util :as mu]
    [malli.experimental.time :as mt]
    [twogreencows.middleware :as middleware]
    [twogreencows.entities.util :as tgc-util]
    ))

(def session-data-version 1)
(def session-prefix "ses")
(def current-version "0.0.8")

(def session-description
    (mu/merge tgc-util/tgc-entity-description (m/schema [:map 
                                                         [:user_uuid string?] 
                                                         [:terminated_at string?] 
                                                         [:device_uuid string?] 
                                                         [:token_uuid string?] 
                                                         [:is_new_user boolean?]
                                                         [:is_new_device boolean?]
                                                         ])))



(defn session-list [] (db/execute-query ["select * from sessions"]))
;(defn get-session [uuid]
;  (db/get-session-by-uuid {:uuid uuid} ))


