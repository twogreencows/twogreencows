(ns twogreencows.websockets
  (:require-macros [mount.core :refer [defstate]])
  (:require [re-frame.core :as rf]
            [taoensso.sente :as sente]
            mount.core))

(defonce channel (atom nil))

(defstate socket
  :start (sente/make-channel-socket!
           "/ws"
           (.-value (.getElementById js/document "token"))
           {:type :auto :wrap-recv-evs? false}))


(defn send! [usr]
  (if-let [send-fn (:send-fn @socket)]
    (send-fn usr)
    (throw (ex-info "Cound not send user creation, channel isn't open!" {:message usr}))))


(defmulti handle-user
  (fn [{:keys [id]} _]
    id))

(defmethod handle-user :user/add 
  [_ user-add-event]
  (rf/dispatch user-add-event))


(defmethod handle-user :user/creation-errors
  [_ [_ response]]
  (rf/dispatch [:form/set-server-errors (:errors response)]))


(defmethod handle-user :chsk/handshake
    [{:keys [event]} _]
    (.log js/console "COnnection established" (pr-str event)))

(defmethod handle-user :chsk/state
    [{:keys [event]} _]
    (.warn js/console "Inknown websocket message" (pr-str event)))


(defn receive-user!
  [{:keys [id event] :as ws-message}]
  (do
    (.log js/console "Event Received: " (pr-str event))
    (handle-user ws-message event)))

(defstate channel-router
  :start (sente/start-chsk-router!
          (:ch-recv @socket)
          #'receive-user!)
  :stop (when-let [stop-fn @channel-router]
          (stop-fn)))






