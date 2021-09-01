(ns ^:dev/once twogreencows.app
  (:require 
    [devtools.core :as devtools]
    [twogreencows.core :as core]))

(enable-console-print!)
(println "Loading env/dev/cljs/twogreencows/app.cljs...")
(devtools/install!)
(core/init!)
