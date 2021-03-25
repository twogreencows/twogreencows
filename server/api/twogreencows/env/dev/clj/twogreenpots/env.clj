(ns twogreenpots.env
  (:require
    [selmer.parser :as parser]
    [clojure.tools.logging :as log]
    [twogreenpots.dev-middleware :refer [wrap-dev]]))

(def defaults
  {:init
   (fn []
     (parser/cache-off!)
     (log/info "\n-=[twogreenpots started successfully using the development profile]=-"))
   :stop
   (fn []
     (log/info "\n-=[twogreenpots has shut down successfully]=-"))
   :middleware wrap-dev})
