(ns twogreencows.handler
  (:require
    [twogreencows.middleware :as middleware]
    [twogreencows.layout :refer [error-page]]
    [twogreencows.routes.home :refer [home-routes]]
    [twogreencows.routes.services :refer [service-routes]]
    [twogreencows.entities.user :as tgc-user]
    [twogreencows.entities.environment :as tgc-environment]
    [clojure.tools.logging :as log]
    [reitit.ring :as reitit]
    [ring.middleware.content-type :refer [wrap-content-type]]
    [ring.middleware.webjars :refer [wrap-webjars]]
    [twogreencows.env :refer [defaults]]
    [mount.core :as mount]
    [reitit.ring.middleware.dev :as dev]
    [twogreencows.routes.websockets :refer [websockets-routes]]))

(mount/defstate init-app
  :start ((or (:init defaults) (fn [])))
  :stop  ((or (:stop defaults) (fn []))))

(mount/defstate postinit-app
  ;:init (log/info "postinit init")
  :start  (let [allusers (tgc-user/user-list []) t (tgc-environment/unique-environment)] 
              (if (= 0 (count allusers)) (tgc-user/new-user! tgc-user/user-admin-data []) ))                             
  :stop (fn [] (println "postinit stop")))



(mount/defstate app-routes
  :start
  (middleware/wrap-base
    (reitit/ring-handler
      (reitit/router
        [(home-routes) (service-routes)] ;(websockets-routes)]
        ;;{:reitit.middleware/transform dev/print-request-diffs} 
        )
      (reitit/routes
        (reitit/create-resource-handler
          {:path "/"})
        (wrap-content-type
          (wrap-webjars (constantly nil)))
        (reitit/create-default-handler
          {:not-found
           (constantly (error-page {:status 404, :title "404 - Page not found"}))
           :method-not-allowed
           (constantly (error-page {:status 405, :title "405 - Not allowed"}))
           :not-acceptable
           (constantly (error-page {:status 406, :title "406 - Not acceptable"}))})))))


(defn app [] 
  (middleware/wrap-base #'app-routes))
