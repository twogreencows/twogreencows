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
  (if-let [envs (vec (db/get-environments))] 
        (if (= (count envs) 0) 
          (try
            (let [t_now (java.time.Instant/now) newuuid (str environment-prefix "-" (tgc-util/tgc-entity-uuidpostfix))]
                (db/create-environment! {:uuid newuuid :data_version environment-data-version :object_version 1 :name (generate-environment-name) :sem_version current-version :created_at t_now :updated_at t_now})
                ;;(db/get-environment-by-uuid {:uuid newuuid})
            ))
          (get envs 0))))


(defn unique-environment [] 
  (get-environment!))
