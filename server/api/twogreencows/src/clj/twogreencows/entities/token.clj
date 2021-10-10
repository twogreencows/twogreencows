(ns twogreencows.entities.token
  (:require
    [twogreencows.db.core :as db]
    [clojure.java.io :as io]    
    [twogreencows.middleware :as middleware]
    ))

(def token-object-version 1)
(def token-prefix "tok")

