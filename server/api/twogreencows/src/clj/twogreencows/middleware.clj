(ns twogreencows.middleware
  (:require
    [twogreencows.env :refer [defaults]]
    [cheshire.generate :as cheshire]
    [cognitect.transit :as transit]
    [clojure.tools.logging :as log]
    [twogreencows.layout :refer [error-page]]
    [ring.middleware.anti-forgery :refer [wrap-anti-forgery]]
    [twogreencows.middleware.formats :as formats]
    [muuntaja.middleware :refer [wrap-format wrap-params]]
    [twogreencows.config :refer [env]]
    [ring.middleware.flash :refer [wrap-flash]]
    [immutant.web.middleware :refer [wrap-session]]
    [ring.middleware.defaults :refer [site-defaults wrap-defaults]])
           )

(defn wrap-internal-error [handler]
  (fn [req]
    (try
      (handler req)
      (catch Throwable t
        (log/error t (.getMessage t))
        (error-page {:status 500
                     :title "Something very bad has happened!"
                     :message "No more sprout, no more cows!"})))))

(defn wrap-csrf [handler]
  (wrap-anti-forgery
    handler
    {:error-response
     (error-page
       {:status 403
        :title "Invalid anti-forgery token"})}))


(defn wrap-formats [handler]
  (let [wrapped (-> handler wrap-params (wrap-format formats/instance))]
    (fn [request]
      ;; disable wrap-formats for websockets
      ;; since they're not compatible with this middleware
      ((if (:websocket? request) handler wrapped) request))))



(defn wrap-base [handler]
  (-> ((:middleware defaults) handler)
      wrap-flash
      (wrap-session {:cookie-attrs {:http-only true}})
      (wrap-defaults
        (-> site-defaults
            (assoc-in [:security :anti-forgery] false)
            (dissoc :session)))
      wrap-internal-error))

(defn wrap-server-metatagging [handler]
  (fn [request]
        (let [xt-in (System/currentTimeMillis) w (handler request) xt-out (System/currentTimeMillis)] 
              (let [data (get w :body) server {:server_duration (- xt-out xt-in) :status (get w :status)}]
                (assoc-in (assoc-in (assoc-in (assoc-in {}  [:body :data] data) [:body :server] server ) [:headers] (get w :headers)) [:status] (get w :status))
                ))
        ))
             
              
(defn wrap-server-allowcors [handler]
  (fn [request]
    (let [w (handler request)]
      (assoc-in  w [:headers "Access-Control-Allow-Origin"] "http://127.0.0.1:5173")
    ;;(-> request handler) ;(assoc-in [:headers "Access-Control-Allow-Origin"] "http://127.0.0.1:5173"))
    )))  
              


 

          
