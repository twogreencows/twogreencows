(ns twogreencows.entities.environment
  (:require
    [twogreencows.db.core :as db]
    [twogreencows.middleware :as middleware]
    [twogreencows.entities.util :as tgc-util]
    [malli.core :as m]
    [malli.error :as me]
    [malli.util :as mu]
  ))

(def environment-data-version 1)
(def environment-prefix "env")
(def current-version "0.0.8")

(def words_1 ["little" "big" "nice" "grumpy" "sad" "funny" "sleepy" "laughing" "joyous" "crying" "maniac" "fighting" "cheerful" "gloomy" "calm"])
(def words_2  ["blue" "green" "orange" "yellow" "red" "black" "purple" "pink" "lavender" "plum" "gray" "gold" "silver" "cyan" "olive"])
(def words_3 ["cow" "sheep" "kangaroo" "crocodile" "dog" "cat" "ant" "butterfly" "mouse" "axolotl" "elephant" "tuna" "turtle" "ladybug" "antelope"])

(defn generate-environment-name []
  (str (rand-nth words_1) "-" (rand-nth words_2) "-" (rand-nth words_3) "-" (int (rand 27130410))))

(def environment-description  
  (mu/merge tgc-util/tgc-entity-description (m/schema [:map 
                                                         [:name :string] 
                                                         [:sem_version :string]])))


(defn get-environment! []
  (if-let [envs (db/execute-query ["select * from environments"])]
        (if (= (count envs) 0) 
          (try
            (let [t_now (java.time.Instant/now) newuuid (str environment-prefix "-" (tgc-util/tgc-entity-uuidpostfix))
                newenv (db/execute-query ["insert into environments (uuid, created_at, updated_at, data_version, object_version, name, sem_version) values (?, ?, ?, ?, ?, ?, ?)" 
                                   newuuid t_now t_now environment-data-version 1 (generate-environment-name) current-version])]
                (get newenv 0)
            ))
          (get envs 0))))


(defn count-environment []
  (let [envs (db/execute-query ["select * from environments"])]
    (count envs)))

(defn unique-environment [] 
  (get-environment!))
