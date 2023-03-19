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

 (defn token-description [] 
  (assoc (tgc-util/tgc-entity-description) :creation_date inst? )) ;:expiration_date string? :is_valid boolean? :value string? :owner_uuid string?))


