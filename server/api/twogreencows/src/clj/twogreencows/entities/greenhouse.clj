(ns twogreencows.entities.greenhouse
  (:require
    [twogreencows.db.core :as db]
    [malli.core :as m]
    [malli.error :as me]
    [malli.util :as mu]
    [malli.experimental.time :as mt]
    [twogreencows.entities.error :as tgc-error]
    [twogreencows.entities.util :as tgc-util]
    [clojure.java.io :as io]    
    [twogreencows.entities.util :as tgc-util]
    [twogreencows.middleware :as middleware]
    ))

(def greenhouse-object-version 1)
(def greenhouse-prefix "grn")

(def greenhouse-subobjects  {:growbox "greenhouse-uuid"})

(def greenhouse-post-description
   [:map 
              [:latitude :float]
              [:longitude :float]
              [:display_name {:min 8} :string]
              [:description {:min 8} :string]
              [:owner_uuid {:optional true} :string]
              ]
        
     )

(def greenhouse-description
    (mu/merge tgc-util/tgc-entity-description (m/schema [:map 
                                                         [:display_name :string] 
                                                         [:label :string] 
                                                         ;;[:latitude :float] 
                                                         ;;[:longitude :float]
                                                         [:owner-uuid :string]
                                                         ])))


(defn format-with-subobjects 
  ([greenhouse] (format-with-subobjects greenhouse []))
  ([greenhouse subobjects]
  (let [real_subobjects (filter #(contains? greenhouse-subobjects %) subobjects)]
    (identity greenhouse)
    ;;(merge (into {} (remove (fn [[k v]] (nil? v)) user )) (reduce conj {} (zipmap real_subobjects (map (fn [u] (get-user-subobjects (user :uuid) u)) real_subobjects)))
  )
    ))


(defn greenhouse-list [subobjects] 
  (let [greenhouses (db/execute-query ["select * from greenhouses"])]
      (map (fn [g] (format-with-subobjects g subobjects)) greenhouses) 
    ))


