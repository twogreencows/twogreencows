(ns twogreenpots.routes.home
  (:require
    [twogreenpots.layout :as layout]
    [twogreenpots.db.core :as db]
    [clojure.java.io :as io]
    [twogreenpots.middleware :as middleware]
    [ring.util.http-response :as response]
    [struct.core :as st]))

(defn home-page [{:keys [flash] :as request}]
  (layout/render 
    request 
    "home.html" 
    (merge {:users (db/get-users)}
           (select-keys flash [:name :message :errors]
           ))))

(defn about-page [request]
  (layout/render request "about.html"))

(def user-schema [[:display_name 
                   st/required 
                   st/string]
                  [:password st/required 
                   st/string
                   {:message "Password too short"
                    :validate (fn [pwd] (>= (count pwd) 8))}]])


(defn validate-user [params] 
  (println "validate")
  (first (st/validate params user-schema)))
                  
(defn new-user! [{:keys [params]}]
  (println params)
  (if-let [errors (validate-user params)]
    (do
     (println "klkl")
     (println errors)
     (-> (response/found "/") 
        (assoc :flash (assoc params :errors errors)))
     )
    (do
    (println "there")  
    (db/create-user! (assoc params :uuid (str "u-" (clojure.string/replace (.toString (java.util.UUID/randomUUID)) #"-" ""))
            :data_version 1 :object_version 1 :country "FRA" :creation_date (java.time.LocalDateTime/now)))
    (response/found "/"))))

(defn home-routes []
  [""
   {:middleware [middleware/wrap-csrf
                 middleware/wrap-formats]}
   ["/" {:get home-page}]
   ["/user" {:post new-user!}]
   ["/about" {:get about-page}]])

