(ns twogreencows.env
  (:require [clojure.tools.logging :as log]))

(def defaults
  {:init
   (fn []
     (log/info "\n-=[twogreencows started successfully]=-"))
   :stop
   (fn []
     (log/info "\n-=[twogreencows has shut down successfully]=-"))
   :middleware identity})
