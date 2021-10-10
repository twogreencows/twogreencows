(ns twogreencows.entities.environment
  (:require
    [twogreencows.db.core :as db]
    [twogreencows.middleware :as middleware]
    [twogreencows.entities.util :as tgc-util]
  ))

(def environment-data-version 1)
(def environment-prefix "env")
(def current-version "0.0.8")

(def words_1 ["little" "big" "nice" "grumpy" "sad" "funny" "sleepy" "laughing" "joyous" "crying" "maniac" "fighting" "cheerful" "gloomy" "calm"])
(def words_2  ["blue" "green" "orange" "yellow" "red" "black" "purple" "pink" "lavender" "plum" "gray" "gold" "silver" "cyan" "olive"])
(def words_3 ["cow" "sheep" "kangaroo" "crocodile" "dog" "cat" "ant" "butterfly" "mouse" "axolotl" "elephant" "tuna" "turtle" "ladybug" "antelope"])

(defn generate-environment-name []
  (str (rand-nth words_1) "-" (rand-nth words_2) "-" (rand-nth words_3) "-" (int (rand 27130410))))

(defn environment-description [] 
  (assoc (assoc (tgc-util/tgc-entity-description) :name string?) :sem_version string?))


(defn get-environment! []
  (if-let [envs (vec (db/get-environments))] 
        (if (= (count envs) 0) 
          (try
            (let [newuuid (str environment-prefix "-" (tgc-util/tgc-entity-uuidpostfix))]
                (db/create-environment! {:uuid newuuid :data_version environment-data-version :object_version 1 :name (generate-environment-name) :sem_version current-version})
                (db/get-environment-by-uuid {:uuid newuuid})
            ))
          (get envs 0))))


(defn unique-environment [] {:data (get-environment!)})
