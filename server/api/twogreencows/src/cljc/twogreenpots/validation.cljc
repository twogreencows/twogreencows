(ns twogreenpots.validation
  (:require [struct.core :as st]))

(def user-schema
  [[:display_name st/required st/string]
   [:password st/required st/string
    {:message "Password too short, should b 8 minimum"
         :validate (fn [msg] (>= (count msg) 8))}]])

(defn validate-user [params] 
  (do
  (println "VALIDATE")
  (println params)
  (first (st/validate params user-schema))))
