(ns twogreenpots.routes.home
  (:require
    [twogreenpots.layout :as layout]
    ;[twogreenpots.users :as usrs]
    ;[twogreenpots.db.core :as db]
    ;[clojure.java.io :as io]    
    [twogreenpots.middleware :as middleware]
    ;[ring.util.http-response :as response]
    ))

(defn home-page [request]
  (layout/render 
    request 
    "home.html")) 

(defn about-page [request]
  (layout/render request "about.html"))

                  
(defn home-routes []
  [""
   {:middleware [middleware/wrap-csrf
                 middleware/wrap-formats]}
   ["/" {:get home-page}]
   ["/about" {:get about-page}]])

