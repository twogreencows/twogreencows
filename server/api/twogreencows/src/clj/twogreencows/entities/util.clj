(ns twogreencows.entities.util)

(defn tgc-entity-description [] {:uuid string? :object_version int? :data_version int?})

(defn tgc-entity-uuidpostfix [] (clojure.string/replace (.toString (java.util.UUID/randomUUID)) #"-" ""))

(defn tgc-httpanswer-metadescription [x] {:data x :server {:server_duration int? :status int?}}) 
