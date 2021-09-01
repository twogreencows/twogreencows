(ns twogreencows.env
  (:require
    [selmer.parser :as parser]
    [clojure.tools.logging :as log]
    [twogreencows.dev-middleware :refer [wrap-dev]]))

(def defaults
  {:init
   (fn []
     (parser/cache-off!)
     (log/info "\n-=[twogreencows started successfully using the development profile]=-"))
   :stop
   (fn []
     (log/info "\n-=[twogreencows has shut down successfully]=-"))
   :middleware wrap-dev})
