(ns twogreenpots.env
  (:require [clojure.tools.logging :as log]))

(def defaults
  {:init
   (fn []
     (log/info "\n-=[twogreenpots started successfully]=-"))
   :stop
   (fn []
     (log/info "\n-=[twogreenpots has shut down successfully]=-"))
   :middleware identity})
