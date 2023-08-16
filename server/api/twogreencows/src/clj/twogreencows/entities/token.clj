(ns twogreencows.entities.token
  (:require
    [twogreencows.db.core :as db]
    [clojure.java.io :as io]    
    [malli.core :as m]
    [malli.error :as me]
    [malli.util :as mu]
    [malli.experimental.time :as mt]
    [twogreencows.entities.util :as tgc-util]
    [twogreencows.middleware :as middleware]
    ))

(def token-data-version 1)
(def token-prefix "tok")
(def current-version "0.0.8")

(def token-description  
    (mu/merge tgc-util/tgc-entity-description (m/schema [:map 
                                                         [:expires_at :time/instant] 
                                                         [:is_valid boolean?] 
                                                         [:value string?]
                                                         [:device_uuid string?]
                                                         [:owner_uuid string?]])))


(defn new-token! [params]
     (let [newuuid (str token-prefix "-" (clojure.string/replace (.toString (java.util.UUID/randomUUID)) #"-" "")) 
           tnow (java.time.LocalDateTime/now)
           texpiration (.plusDays tnow 182)
           token_value (tgc-util/tobase64 (tgc-util/tgc-hash-generate-salt 8))
           newtoken (db/execute-query ["insert into tokens (uuid, created_at, updated_at, data_version, object_version, expires_at, is_valid, value, owner_uuid ) 
                  values (?, ?, ?, ?, ?, ?, ?, ?, ?)" newuuid tnow tnow token-data-version 1 texpiration true token_value (params :owner_uuid)])]
             (get newtoken 0) 
          )) 
      
  

(defn get-token [uuid]
  (db/execute-query [(str "select * from tokens where uuid="  uuid)]))

(defn get-tokens-for-user [owner-uuid]
  (db/execute-query [(str "select * from tokens where owner_uuid="  owner-uuid)]))

(defn delete-tokens-for-user [owner-uuid]
  (db/execute-query [(str "delete from tokens where owner_uuid="  owner-uuid)]))

(defn delete-token [uuid] 
  (db/execute-query ["delete from tokens where uuid=? returning * " uuid])) 

