(ns twogreencows.app 
  (:require 
    [twogreencows.core :as core]))

;; ignore println in prod
(set! *print-fn* (fn [& _]))

(core/init!)
