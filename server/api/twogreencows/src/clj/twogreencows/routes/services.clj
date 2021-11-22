 (ns twogreencows.routes.services
   (:require
    [reitit.swagger :as swagger]
    [reitit.swagger-ui :as swagger-ui]
    [reitit.ring.coercion :as coercion]
    [reitit.coercion.spec :as spec-coercion]
    [reitit.ring.middleware.muuntaja :as muuntaja]
    [reitit.ring.middleware.exception :as exception]
    [reitit.ring.middleware.multipart :as multipart]
    [reitit.ring.middleware.parameters :as parameters]
    [twogreencows.entities.user :as tgc-user]
    [twogreencows.entities.token :as tgc-token]
    [twogreencows.entities.environment :as tgc-environment]
    [twogreencows.entities.greenhouse :as tgc-greenhouse]
    [twogreencows.entities.device :as tgc-device]
    [twogreencows.entities.growbox :as tgc-growbox]
    [twogreencows.entities.util :as tgc-util]
    [twogreencows.middleware :as middleware]
    [ring.util.http-response :as response]
    [twogreencows.middleware.formats :as formats]))

 
(defn service-routes []
  ["/api"
   {:middleware [
                 ;;query-params & form-params
                 parameters/parameters-middleware
                 ;;content negotiation
                 muuntaja/format-negotiate-middleware
                 ;;encoding respond body
                 muuntaja/format-response-middleware
                 ;;exception handling
                 exception/exception-middleware
                 ;;decoding request bodys
                 muuntaja/format-request-middleware
                 ;;coercing response bodys
                 coercion/coerce-response-middleware
                 ;;corecing request parameters
                 coercion/coerce-request-middleware
                 ;;server metatagging;
                 middleware/wrap-server-metatagging
                 ;;multipart params
                 multipart/multipart-middleware
                 ]

    :muuntaja formats/instance
    :coercion  spec-coercion/coercion
    :swagger {:id ::api}}

   ["" {:no-doc true}
    ["/swagger.json" {:get (swagger/create-swagger-handler)}]
    ["/swagger-ui*" {:get (swagger-ui/create-swagger-ui-handler {:url "/api/swagger.json"})}]
    ]
   ["/V1"
    ["/environment"
     {:get
      { :summary "Get information about the server"
        :responses {200 {:body  (tgc-util/tgc-httpanswer-metadescription  (tgc-environment/environment-description)) }}
        :handler (fn [_] (response/ok (do (tgc-environment/unique-environment))))}}]
    ["/users"
     {:get
     {:summary "Get lists of all users. For Admin only" 
      :responses
      {200 {:body 
             {:users
                [{:uuid string? :object_version int? :data_version int? :creation_date inst?
                  :display_name string? :password string? :country string? :phone string?}]
              }}}

      :handler
       (fn [_] (response/ok (do (tgc-user/user-list))))
       }}]

    ["/user"
     {:post
        {:parameters
          {:body
           {:display_name string? :password string? :confirm_password string? :phone string?}}

          :responses
          {200 {:body map?}
           400 {:body map?} 
           409 {:body map?}
           500 {:errors map?}}

          :handler
          (fn [{{params :body} :parameters}]
            (try
              (do
                (let [newuser (tgc-user/new-user! params)]
                (response/ok (assoc {:status :ok} :user newuser))))
                (catch Exception e
                  (println e)
                  (let [{id :twogreencows/error-id errors :errors} (ex-data e)]
                  (case id
                    :validation
                        (response/bad-request {:errors  errors})
                    ;;else
                        (response/internal-server-error 
                            {:errors {:server-error ["Failed to create user!"]}}))))))}}]
    ["/device"
      {:post
         {:parameters
            {:body
              {:uuid string?
               :kind string?
               :vendor_uuid string?
               :platform string?
               :os_version string?}}
            :responses
            {200
             {:body map?}
             400
              {:body map?}
             500
              {:errors map?}}
            :handler
             (fn [{{params :body} :parameters}]
               (println "calling device"))
            }}]
    ]

                

             
    ])




