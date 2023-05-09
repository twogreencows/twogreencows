(ns twogreencows.entities.util
  (:require
    [malli.core :as m]
    [malli.error :as me]
    [malli.registry :as mr]
    [malli.experimental.time :as met]
    ))

(import (javax.crypto Cipher KeyGenerator SecretKey SecretKeyFactory)
        (javax.crypto.spec SecretKeySpec)
        (javax.crypto.spec PBEKeySpec)
        (java.security SecureRandom)
        (org.apache.commons.codec.binary Base64))

(def iterations-count 65536) 
(def salt-reference-n-default 16) ;;this means a salt of 3n bytes which will give 4n base64 string 

(mr/set-default-registry!
  (mr/composite-registry (m/default-schemas) (met/schemas)))

(def tgc-entity-description  [:map {:closed true}
                                 [:uuid :string] 
                                 [:object_version :int] 
                                 [:data_version :int]
                                 [:created_at :time/instant] 
                                 [:updated_at :time/instant]
                                 ])

(defn tgc-entity-uuidpostfix [] (clojure.string/replace (.toString (java.util.UUID/randomUUID)) #"-" ""))

(defn tgc-httpanswer-metadescription [x] [:map {:closed true}
                                          [:data x] 
                                          [:server 
                                           [:map 
                                            [:server_duration :int] 
                                            [:status :int]]]]) 



(defn tobase64 [b]
  (Base64/encodeBase64String b))

(defn frombase64 [s]
  (Base64/decodeBase64 s))

(defn tgc-hash-generate-salt 
  ([] (tgc-hash-generate-salt salt-reference-n-default))
  ([n] (let [sr (java.security.SecureRandom/getInstance "SHA1PRNG")
             salt (byte-array (* 3 n))]
          (do
              (.nextBytes sr salt) 
              (identity salt)))))

(defn tgc-hash-password 
    ( [password] (tgc-hash-password password nil))
    ( [password saltstring] 
     (let [salt (if (nil? saltstring) (tgc-hash-generate-salt) (frombase64 saltstring))]
          (let [key_spec (javax.crypto.spec.PBEKeySpec. (.toCharArray password) salt iterations-count 512)
                factory  (javax.crypto.SecretKeyFactory/getInstance "PBKDF2WithHmacSHA256")]
                [(tobase64 salt) (tobase64 (.getEncoded (.generateSecret factory key_spec))) ]
        ))))

