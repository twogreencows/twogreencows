 (ns twogreencows.routes.services
   (:require
    [clojure.string :as string]     
    [malli.core :as m]
    [malli.error :as me]
    [malli.registry :as mr]
    [malli.experimental.time :as mt]
 
    [reitit.swagger :as swagger]
    [reitit.swagger-ui :as swagger-ui]
    [reitit.ring.coercion :as coercion]
    [reitit.coercion.spec :as spec-coercion]
    [reitit.coercion.malli :as malli-coercion]
    [reitit.ring.middleware.muuntaja :as muuntaja]
    [reitit.ring.middleware.exception :as exception]
    [reitit.ring.middleware.multipart :as multipart]
    [reitit.ring.middleware.parameters :as parameters]
    [twogreencows.entities.user :as tgc-user]
    [twogreencows.entities.token :as tgc-token]
    [twogreencows.entities.environment :as tgc-environment]
    [twogreencows.entities.greenhouse :as tgc-greenhouse]
    [twogreencows.entities.device :as tgc-device]
    [twogreencows.entities.session :as tgc-session]
    [twogreencows.entities.growbox :as tgc-growbox]
    [twogreencows.entities.error :as tgc-error]
    [twogreencows.entities.util :as tgc-util]
    [twogreencows.middleware :as middleware]
    [ring.util.http-response :as response]
    [twogreencows.middleware.formats :as formats]))

(defn handler-param-error [message exception request]
  {:status 400
   :body (tgc-error/create-error 400 "tgc.error.conflict.parameter_error")})

(def tgc-exception-middleware
  (exception/create-exception-middleware
    (merge
      exception/default-handlers
      { :coercion/request-coercion handler-param-error})))

(defn wrap-tgccoercion-request [handler]
  (fn [request]
        (let [xt-in (System/currentTimeMillis) w (handler request) xt-out (System/currentTimeMillis)] 
              (let [data (get w :body) server {:server_duration (- xt-out xt-in) :status (get w :status)}]
                (if (= 400 (server :status))
                  (merge w {:body  (assoc {} :data (tgc-error/create-error 400 "tgc.error.parameter") :server server)})
                  (identity w)
                  )))))
 

(defn service-routes []
  ["/api"
   {
    ;;:middleware [
    ;;             ;;query-params & form-params
     ;;            ;;parameters/parameters-middleware
     ;;            ;;content negotiation
     ;;            muuntaja/format-negotiate-middleware
     ;;            ;;encoding respond body
     ;;            muuntaja/format-response-middleware
     ;;            ;;exception handling
     ;;            exception/exception-middleware
     ;;            ;;decoding request bodys
     ;;            muuntaja/format-request-middleware
     ;;            ;;coercing response bodys
     ;;            coercion/coerce-response-middleware
     ;;            ;;coercing request parameters
     ;;            coercion/coerce-request-middleware
     ;;            ;;multipart params
      ;;           multipart/multipart-middleware
      ;;           ]

    :muuntaja formats/instance
    :coercion  malli-coercion/coercion
    :swagger {:id ::api}}

   ["" {:no-doc true}
    ["/swagger.json" {:get (swagger/create-swagger-handler)}]
    ["/swagger-ui*" {:get (swagger-ui/create-swagger-ui-handler {:url "/api/swagger.json"})}]

   ]
   ["/V1" {:middleware [     
                        ;;query-params & form-params
                        parameters/parameters-middleware
                        ;;content negotiation
                        muuntaja/format-negotiate-middleware
                        ;;encoding respond body
                        muuntaja/format-response-middleware
                        ;;exception handling
                        wrap-tgccoercion-request
                        exception/exception-middleware
                        ;;tgc-exception-middleware
                        ;;decoding request bodys
                        muuntaja/format-request-middleware
                        ;;coercing response bodys
                        coercion/coerce-response-middleware
                        ;;coercing request parameters
                        coercion/coerce-request-middleware
                        ;;multipart params
                        multipart/multipart-middleware
                        ;;server meta-tagging
                        middleware/wrap-server-metatagging
                        ]} 
    ["/environment"
     {:get
      { :summary "Get information about the server"
        :responses {200 {:body  (tgc-util/tgc-httpanswer-metadescription tgc-environment/environment-description) }}
        :handler (fn [_] (let [r (tgc-environment/unique-environment)] (response/ok r)))  }}] ; environment
    ["/sessions"
     {:get  
      {:summary "Get all sessions"
       :responses {200 {:body (tgc-util/tgc-httpanswer-metadescription [:vector tgc-session/session-description]) }}
       :handler (fn [_] (let [r (tgc-session/session-list)] (response/ok r)) ) }}] ; session
    ["/users"
     {:get
        {:summary "Get lists of all users. For Admin only" 
         :responses {200 {:body (tgc-util/tgc-httpanswer-metadescription [:vector tgc-user/user-description]) }} 
         :handler (fn [{qparams :query-params}] 
                    (let [subobjects (if (= (qparams "withSubObjects") "tokens") [:tokens] [])
                          r (tgc-user/user-list subobjects)] 
                      (response/ok r))
                    )}
      :post 
        {:summary "Create a new user"  
         :parameters {:body tgc-user/user-post-description }
         :responses
          {200 {:body (tgc-util/tgc-httpanswer-metadescription tgc-user/user-description) }
           201 {:body (tgc-util/tgc-httpanswer-metadescription tgc-user/user-description) }
           400 {:body (tgc-util/tgc-httpanswer-metadescription tgc-error/error-description) } 
           409 {:body (tgc-util/tgc-httpanswer-metadescription tgc-error/error-description) }
           500 {:body (tgc-util/tgc-httpanswer-metadescription tgc-error/error-description) }}
         :handler (fn [{params :body-params qparams :query-params}]
                    (let [subobjects (if (= (qparams "withSubObjects") "tokens") [:tokens] [])
                          tmpuser (tgc-user/check-for-user params subobjects )]
                      (do
                        (prn tmpuser)
                        (cond 
                            (nil? tmpuser)  (let [newuser (tgc-user/new-user! params subobjects)] (response/created (str "/api/V1/users/" (newuser :uuid)) newuser))
                            (false? tmpuser)  (response/conflict (tgc-error/create-error 409 "tgc.error.conflict.user_already_exists"))
                            :else (response/ok tmpuser))
                        )))
            }
        }
     ];users
     ["/users/:uuid"
        {:get
           {:summary "Get a specific user"
            :responses 
            { 404 {:body (tgc-util/tgc-httpanswer-metadescription tgc-error/error-description) }
              200 {:body (tgc-util/tgc-httpanswer-metadescription tgc-user/user-description) }}
            :handler (fn [{{:keys [uuid]} :path-params qparams :query-params}]
                       (let [subobjects (if (= (qparams "withSubObjects") "tokens") [:tokens] [])
                             tmpuser (tgc-user/get-user uuid subobjects)]
                           (if (nil?  tmpuser)
                              (response/not-found (tgc-error/create-error 404 "tgc.error.notfound.user_notexists"))
                              (response/ok tmpuser)
                           )))
            }
          :delete 
           {:summary "Delete a specific user"
            :responses 
            { 404 {:body (tgc-util/tgc-httpanswer-metadescription tgc-error/error-description) }
              204 {:body (tgc-util/tgc-httpanswer-metadescription tgc-user/user-description) }}
            :handler (fn [{{:keys [uuid]} :path-params qparams :query-params}]
                        (let [tmpusers (tgc-user/delete-user uuid)]
                           (if (empty? (seq tmpusers))
                              (response/not-found (tgc-error/create-error 404 "tgc.error.notfound.user_notexists"))
                              ;; we have to do things ourselves as ring no-content sends back no content and we decide to senfd the user
                              {:status 200 :headers {} :body (nth tmpusers 0)}
                           )))
            }
         }
      ]; user/uuid
     ["/users/:user_uuid/tokens/:token_uuid"
      {:get
           {:summary "Get a specific token for a specific user"
            :responses 
            { 404 {:body (tgc-util/tgc-httpanswer-metadescription tgc-error/error-description) }
              200 {:body (tgc-util/tgc-httpanswer-metadescription tgc-token/token-description) }}
              :handler (fn [{{:keys [user_uuid token_uuid]} :path-params qparams :query-params}]
                         (let [tmpuser (tgc-user/get-user user_uuid [:tokens])]
                              (if (nil? tmpuser)
                                 (response/not-found (tgc-error/create-error 404 "tgc.error.notfound.user_notexists"))
                                 (let [tmptoken (filter (fn [tk] (= (tk :uuid) token_uuid)) (tmpuser :tokens))]
                                   (do 
                                     (print tmptoken) 
                                     (response/ok tmptoken)
                                     ))))
                         )
              
            }
        :delete 
           {:summary "Delete a specific token for a specific user"
            :responses 
            { 404 {:body (tgc-util/tgc-httpanswer-metadescription tgc-error/error-description) }
              200 {:body (tgc-util/tgc-httpanswer-metadescription tgc-token/token-description) }}
              :handler (fn [{{:keys [user_uuid token_uuid]} :path-params qparams :query-params}]
                         (let [tmpuser (tgc-user/get-user user_uuid [:tokens])]
                              (if (nil? tmpuser)
                                 (response/not-found (tgc-error/create-error 404 "tgc.error.notfound.user_notexists"))
                                 (let [tmptoken (filter (fn [tk] (= (tk :uuid) token_uuid)) (tmpuser :tokens))]
                                     (if (empty? tmptoken)
                                          (response/not-found (tgc-error/create-error 404 "tgc.error.notfound.token_notexists"))
                                          (response/ok (nth tmptoken 0))
                                     ))))
                                                                    
                         )
              
            }
        }
      ]
     ["/users/:uuid/tokens"
        {:get
           {:summary "Get the tokens for a specific user"
            :responses 
            { 404 {:body (tgc-util/tgc-httpanswer-metadescription tgc-error/error-description) }
              200 {:body (tgc-util/tgc-httpanswer-metadescription [:vector tgc-token/token-description]) }}
            :handler (fn [{{:keys [uuid]} :path-params qparams :query-params}]
                          (let [tmpuser (tgc-user/get-user uuid [:tokens])]
                              (if (nil? tmpuser)
                                  (response/not-found (tgc-error/create-error 404 "tgc.error.notfound.user_notexists"))
                                  (response/ok (tmpuser :tokens))
                           )))
            }
          :post 
           {:summary "Creste a new token for a specific user"
            :responses 
            { 404 {:body (tgc-util/tgc-httpanswer-metadescription tgc-error/error-description) }
              201 {:body (tgc-util/tgc-httpanswer-metadescription tgc-token/token-description) }}
            :handler (fn [{{:keys [uuid]} :path-params qparams :query-params}]
                    (let [tmpuser (tgc-user/get-user uuid [])]
                        (if (nil? tmpuser)
                           (response/not-found (tgc-error/create-error 404 "tgc.error.notfound.user_notexists"))
                           (let [newtoken (tgc-token/new-token! {:owner_uuid uuid})] 
                              (response/created (str "/api/V1/users/" uuid "/tokens/" (newtoken :uuid)) newtoken)
                              )))) 
            }
           :delete
           {:summary "Delete all tokens for a specific user"
            :response
            { 404 {:body (tgc-util/tgc-httpanswer-metadescription tgc-error/error-description) }
              200 {:body (tgc-util/tgc-httpanswer-metadescription [:vector tgc-token/token-description]) }}
             :handler (fn [{{:keys [user_uuid]} :path-params qparams :query-params}]
                    (let [tmpuser (tgc-user/get-user user_uuid [])]
                        (if (nil? tmpuser)
                           (response/not-found (tgc-error/create-error 404 "tgc.error.notfound.user_notexists"))
                           (response/ok (tgc-token/delete-tokens-for-user user_uuid))
                              ))) 
           }
         }
      ]; user/uuid
    ];v1
   ];api
  )


;(comment     {:post
;        {:summary "Create a new user"
        ;{:parameters {:body (tgc-user/user-post-description) }
;
;          :handler
;          (fn [{{params :body} :parameters}]
;            (try
;              (do
;                (let [newuser (tgc-user/new-user! params)]
;               (response/ok (assoc {:status :ok} :user newuser))))
;               (catch Exception e
;                  (println e)
;                  (let [{id :twogreencows/error-id errors :errors} (ex-data e)]
;                  (case id
;                    :validation
;                        (response/bad-request {:errors  errors})
;                    ;;else
;                        (response/internal-server-error 
;                            {:errors {:server-error ["Failed to create user!"]}}))))))}}
 ;    )

    ;["/device"
     ; {:post
      ;   {:parameters
       ;     {:body
        ;      {:uuid string?
         ;      :kind string?
          ;     :vendor_uuid string?
          ;     :platform string?
          ;     :os_version string?}}
          ;  :responses
          ;  {200
          ;   {:body map?}
          ;   400
           ;   {:body map?}
           ;  500
           ;   {:errors map?}}
           ; :handler
            ; (fn [{{params :body} :parameters}]
            ;   (println "calling device"))
           ; }}]
   




