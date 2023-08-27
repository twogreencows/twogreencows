(ns twogreencows.entities.device
  (:require
    [twogreencows.db.core :as db]
    [twogreencows.db.corecassandra :as dbcassandra]
    [malli.core :as m]
    [malli.error :as me]
    [malli.util :as mu]
    [malli.experimental.time :as mt]
    [twogreencows.entities.error :as tgc-error]
    [twogreencows.entities.util :as tgc-util]
    [twogreencows.middleware :as middleware]
    ))

(def device-object-version 1)
(def datapoint-prefix "dev")

(def device-post-description
  [:and 
   [:map 
              [:kind {:min 4 } :string]
              [:vendor {:min 1} :string]
              [:platform {:min 1} :string]
              [:vendor_uuid :string]
              [:email {:optional true} :string]]
        
      [:fn {:error/message "Unknown kind"
            :error/path [:kind]}
         (fn [{:keys [kind]}]
          (contains? #{"desk" "mobi" "web*" "embd"} kind))]
     ]  
     )

(def device-description
    (mu/merge tgc-util/tgc-entity-description (m/schema [:map 
                                                         [:kind string?] 
                                                         [:vendor_uuid string?] 
                                                         [:vendor string?]
                                                         [:platform string?]
                                                         [:owner_uuid string?] 
                                                         ])))


(defn get-device [uuid]
  (db/execute-query [(str "select * from devices where uuid=" uuid)]))

;;   owner_uuid CHAR(36),
    
;;    kind VARCHAR(8),
;;    vendor_uuid CHAR(36),
;;    platform VARCHAR(8),  
;;    os_version VARCHAR(16),

