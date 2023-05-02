(ns twogreencows.entities.error
  (:require
    [malli.core :as m]
    [malli.error :as me]
    [malli.util :as mu]
    [twogreencows.entities.util :as tgc-util]
    [twogreencows.db.core :as db]
    [twogreencows.middleware :as middleware]
  ))

(def error-data-version 1)
(def error-prefix "err")

(def error-description  
    (mu/merge tgc-util/tgc-entity-description (m/schema [:map 
                                                         [:message :string] 
                                                         [:code :int]])))
  
(defn create-error [code message] 
   (let [tmpuuid (str error-prefix "-" (clojure.string/replace (.toString (java.util.UUID/randomUUID)) #"-" ""))
           tnow (java.time.Instant/now)]
            {:message message :code code :uuid tmpuuid :object_version 1 :data_version error-data-version :created_at tnow :updated_at tnow}))
