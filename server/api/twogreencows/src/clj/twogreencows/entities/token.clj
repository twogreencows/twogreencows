(ns twogreencows.entities.token
  (:require
    [twogreencows.db.core :as db]
    [clojure.java.io :as io]    
    [twogreencows.entities.util :as tgc-util]
    [twogreencows.middleware :as middleware]
    ))

(def token-object-version 1)
(def token-prefix "tok")
(def current-version "0.0.8")
