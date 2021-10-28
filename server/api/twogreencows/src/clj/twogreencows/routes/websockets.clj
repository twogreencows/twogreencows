(ns twogreencows.routes.websockets
  (:require  [clojure.tools.logging :as log]
             [org.httpkit.server :as http-kit]
             [clojure.edn :as edn]
             [mount.core :refer [defstate]]
             [taoensso.sente :as sente]
             [taoensso.sente.server-adapters.http-kit :refer [get-sch-adapter]]
             [twogreencows.entities.user :as tgc-user]
             [twogreencows.entities.token :as tgc-token]
             [twogreencows.entities.environment :as tgc-environment]
             [twogreencows.entities.greenhouse :as tgc-greenhouse]
             [twogreencows.entities.device :as tgc-device]
             [twogreencows.entities.growbox :as tgc-growbox]
             [twogreencows.middleware :as middleware]))



(defstate socket 
  :start (sente/make-channel-socket!
           (get-sch-adapter)
           {:user-id-fn (fn [ring-req] 
                          (do
                          (get-in ring-req [:params :client-id])))}))

(defn send! [uid user]
  ((:send-fn socket) uid user))

(defmulti handle-user (fn [{:keys [id]}] id))

(defmethod handle-user :default
  [{:keys [id]}]
  (log/debug "Received unrecognized websocket event type: " id))

(defmethod handle-user :user/create!
  [{:keys [?data uid] :as user}]
  (let [response (try tgc-user/new-user! ?data)
     response (try
              (tgc-user/new-user! user)
              (catch Exception e
                (let [{id :twogreencows/error-id errors :errors} (ex-data e)]
                  (case id
                    :validation
                    {:errors errors}
                    ;;else
                    {errors {:server-error ["Failed to save user"]}}))))]
    (if (:errors response)
      (send! uid [:user/creation-errors response])
      (doseq [uid (:any @(:connected-uids socket))]
        (send! uid[:user/add response])))))

(defn receive-user! [{:keys [id] :as user}]
  (log/debug "Got message with id: " id)
  (handle-user user))



(defstate channel-router
  :start (sente/start-chsk-router!
          (:ch-recv socket)
         #'receive-user!)
  :stop (when-let [stop-fn channel-router] (stop-fn)))


(defn websockets-routes []
  ["/ws"
    {:middleware [middleware/wrap-csrf 
                  middleware/wrap-formats]
     :get (:ajax-get-or-ws-handshake-fn socket)
     :post (:ajax-post-fn socket)}])

  












