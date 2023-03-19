(ns twogreencows.entities.util
  (:require
    [malli.core :as m]
    [malli.error :as me]
    [malli.registry :as mr]
    [malli.experimental.time :as met]
    ))

(import (javax.crypto Cipher KeyGenerator SecretKey)
        (javax.crypto.spec SecretKeySpec)
        (java.security SecureRandom)
        (org.apache.commons.codec.binary Base64))

(mr/set-default-registry!
  (mr/composite-registry (m/default-schemas) (met/schemas)))

(def tgc-entity-description  [:map {:closed true}
                                 [:uuid :string] 
                                 [:object_version :int] 
                                 [:data_version :int]
                                 [:created_at :time/instant] 
                                 [:updated_at :time/instant]])

(defn tgc-entity-uuidpostfix [] (clojure.string/replace (.toString (java.util.UUID/randomUUID)) #"-" ""))

(defn tgc-httpanswer-metadescription [x] [:map {:closed true}
                                          [:data x] 
                                          [:server 
                                           [:map 
                                            [:server_duration :int] 
                                            [:status :int]]]]) 


(defn bytes [s]
  (.getBytes s "UTF-8"))

(defn base64 [b]
  (Base64/encodeBase64String b))

(defn debase64 [s]
  (Base64/decodeBase64 (bytes s)))

(defn get-raw-key [seed]
  (let [keygen (KeyGenerator/getInstance "AES")
        sr (SecureRandom/getInstance "SHA1PRNG")]
    (.setSeed sr (bytes seed))
    (.init keygen 128 sr)
    (.. keygen generateKey getEncoded)))

(defn get-cipher [mode seed]
  (let [key-spec (SecretKeySpec. (get-raw-key seed) "AES")
        cipher (Cipher/getInstance "AES")]
    (.init cipher mode key-spec)
    cipher))

(defn encrypt [text key]
  (let [bytes (bytes text)
        cipher (get-cipher Cipher/ENCRYPT_MODE key)]
    (base64 (.doFinal cipher bytes))))

(defn decrypt [text key]
  (let [cipher (get-cipher Cipher/DECRYPT_MODE key)]
    (String. (.doFinal cipher (debase64 text)))))
