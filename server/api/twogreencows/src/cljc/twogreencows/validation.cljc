(ns twogreencows.validation
  (:require [struct.core :as st]))

;(def user-schema
;  [[:display_name st/required st/string]
;   [:password st/required st/string {:message "Password too short, should be 8 minimum" :validate (fn [msg] (>= (count msg) 8))}]
;   [:confirm_password st/required st/string st/identical-to :password]
;[   [:phone_number st/string st/required]
;   ])

;(defn validate-user [params]
 ; (do
;  (print "lala")
;  (print (st/validate params user-schema))))
  ;(first (st/validate params user-schema)))
