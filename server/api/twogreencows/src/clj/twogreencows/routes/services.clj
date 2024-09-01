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
   :body (tgc-error/create-error 400 "tgc.error.parameter_error")})

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
                    (do
                    (println "UAUAUA")  
                    (println data)  
                    (println (data :humanized))
                    (println "UAUAUA")  
                    (let [desc (apply str (map  (fn [[k v]] (str (name k) ": " (apply str (interpose ", " v)))) (data :humanized)))]
                      (merge w {:body  (assoc {} :data (tgc-error/create-error 400 "tgc.error.parameter" desc) :server server)}) ))
                  (identity w)
                  )))))
 

(defn service-routes []
  ["/api"
   {
    :muuntaja formats/instance
    :coercion  malli-coercion/coercion
    :swagger {:id ::api}}

   ["" {:no-doc true
        :middleware [
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
                 ;;coercing request parameters
                 coercion/coerce-request-middleware
                 ;;multipart params
                 multipart/multipart-middleware
                 ]
        }
    ["/swagger.json" {:get (swagger/create-swagger-handler)}]
    ["/swagger-ui*" {:get (swagger-ui/create-swagger-ui-handler {:url "/api/swagger.json"})}]

   ]
   ["/V1" {:middleware [     
                        ;;cors for dev 
                        middleware/wrap-server-allowcors
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
        :handler (fn [_] (let [r (tgc-environment/unique-environment)] (response/ok r))) }
     }
    ] ; environment
    ["/sessions"
     {:get  
      {:summary "Get all sessions"
       :responses {200 {:body (tgc-util/tgc-httpanswer-metadescription [:vector tgc-session/session-description]) }
                   401 {:body (tgc-util/tgc-httpanswer-metadescription tgc-error/error-description) }
                   }
       :handler (fn [_] (let [r (tgc-session/session-list [])] (response/ok r)) ) 
      }
      :post
      {:summary "Create a session"
       :parameters {:body tgc-session/session-post-description }
       :response {200 {:body (tgc-util/tgc-httpanswer-metadescription tgc-session/session-description)}
                  400 {:body (tgc-util/tgc-httpanswer-metadescription tgc-error/error-description) }
                  401 {:body (tgc-util/tgc-httpanswer-metadescription tgc-error/error-description) }
                  }
       :handler (fn [{params :body-params qparams :query-params}]
            (do   
             (println "USR") 
             (println (params :user)) 
            (println (tgc-user/check-for-user (params :user) [:devices :tokens])) 
            (println (tgc-device/check-for-device (params :device)))
            (let [[tmpuserstatus tmpuser] (tgc-user/check-for-user (params :user) [:devices :tokens])
                    mode (qparams "mode")]

                (cond
                  (= :absent tmpuserstatus)
                    (cond  
                      (= "signin" mode) 
                        (response/unauthorized (tgc-error/create-error 401 "tgc.error.unauthorized.user_notexists"))
                      (= "signup" mode)                          
                        (if (m/validate tgc-user/user-post-description (params :user))
                            (let [newuser (tgc-user/new-user! (params :user) [:devices])
                                  newdevice (tgc-device/new-device! (assoc (params :device) (newuser :uuid))  [:tokens])]
                                  (let[subobjects [:user :device :token]
                                   session_params {:user_uuid (tmpuser :uuid) :device_uuid (newdevice :uuid) :is_new_user true :is_new_device false} 
                                   newsession (tgc-session/new-session! session_params subobjects)] 
                                      (response/created (str "/api/V1/session/" (newsession :uuid)) newsession)))

                            (response/bad-request (tgc-error/create-error 400 "tgc.error.badrequest.parameter_error")))
                      :else 
                          (response/bad-request (tgc-error/create-error 400 "tgc.error.badrequest.parameter_error"))
                    )

                  (= :conflict tmpuserstatus) 
                    (if (= "signin" mode) 
                      (response/unauthorized (tgc-error/create-error 401 "tgc.error.unauthorized.user_notexists"))
                      (response/conflict (tgc-error/create-error 409 "tgc.error.conflict.user_already_exists")))
 
                  (= :exist tmpuserstatus) 
                    (let [device_params (assoc (params :device) :owner_uuid (tmpuser :uuid))
                          [tmpdevicestatus tmpdevice] (tgc-device/check-for-device device_params)]
                        (cond 
                          (= :absent tmpdevicestatus) 
                            (let [newdevice (tgc-device/new-device! device_params [:tokens])]
                              (let[subobjects [:user :device :token]
                                   session_params {:user_uuid (tmpuser :uuid) :device_uuid (newdevice :uuid) :is_new_user true :is_new_device false} 
                                   newsession (tgc-session/new-session! session_params subobjects)] 
                                      (response/created (str "/api/V1/session/" (newsession :uuid)) newsession)))
                          (= :conflict tmpdevicestatus) ()

                          (= :exist tmpdevicestatus)
                             (do 
                               (println "lalala")
                              (let[subobjects [:user :device :token]
                                   session_params {:user_uuid (tmpuser :uuid) :device_uuid (tmpdevice :uuid) :is_new_user true :is_new_device false} 
                                   newsession (tgc-session/new-session! session_params subobjects)] 
                                      (response/created (str "/api/V1/session/" (newsession :uuid)) newsession))
                              )
                        ))
                    ))))
      }
     }
    ] ; sessions
    ["/sessions/:uuid"
     {:get
      {:summary "Get a specific session"
       :responses {200 {:body (tgc-util/tgc-httpanswer-metadescription tgc-session/session-description)}
                   401 {:body (tgc-util/tgc-httpanswer-metadescription tgc-error/error-description) }
                   404 {:body (tgc-util/tgc-httpanswer-metadescription tgc-error/error-description) }
                }
       :handler (fn [_] (response/ok {})) 
       }
      :delete
      {:summary "Close/delete a specific session"
       :responses {200 {:body (tgc-util/tgc-httpanswer-metadescription tgc-session/session-description)}
                   401 {:body (tgc-util/tgc-httpanswer-metadescription tgc-error/error-description) }
                   404 {:body (tgc-util/tgc-httpanswer-metadescription tgc-error/error-description) }
                }
       :handler (fn [_] (response/ok {})) 
      }
     }
    ] ; sessions/:uuid
    ["/tokens"
     {:get
      {:summary "Get list of all tokens" 
       :responses {200 {:body (tgc-util/tgc-httpanswer-metadescription [:vector tgc-token/token-description]) }
                   401 {:body (tgc-util/tgc-httpanswer-metadescription tgc-error/error-description) }
                   }
       :handler (fn [_] (let [r (tgc-token/token-list [])] (response/ok r)) ) 
      }
      }
    ]
    ["/users"
     {:get
        {:summary "Get lists of all users. admin gets all, user gets themselves, or nothing if unauthorized" 
         :responses {200 {:body (tgc-util/tgc-httpanswer-metadescription [:vector tgc-user/user-description]) }
                     401 {:body (tgc-util/tgc-httpanswer-metadescription tgc-error/error-description)}                     
                     } 
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
                          [userstatus tmpuser] (tgc-user/check-for-user params subobjects )]
                        (cond 
                            (= :absent userstatus)  (let [newuser (tgc-user/new-user! params subobjects)] (response/created (str "/api/V1/users/" (newuser :uuid)) newuser))
                            (= :conflict userstatus)  (response/conflict (tgc-error/create-error 409 "tgc.error.conflict.user_already_exists"))
                            (= :exist userstatus)  (response/ok tmpuser))
                        ))
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
                              {:status 204 :headers {} :body (nth tmpusers 0)}
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
                                     (print tmptoken) 
                                     )))
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
                           (let [newtoken (tgc-token/new-token! {:owner_uuid uuid :kind "devc"})] 
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
      ["/devices"
      {:get  
       {:summary "Get all devices"
        :responses {200 {:body (tgc-util/tgc-httpanswer-metadescription [:vector tgc-device/device-description]) }
                   401 {:body (tgc-util/tgc-httpanswer-metadescription tgc-error/error-description) }
                   }
       :handler (fn [{qparams :query-params}] (let [subobjects (if (= (qparams "withSubObjects") "token") [:token] [])
                                                    r (tgc-device/device-list subobjects)] (response/ok r)) ) 

      }
      :post
      {:summary "Create a device"
       :parameters {:body tgc-device/device-post-description }
       :response {200 {:body (tgc-util/tgc-httpanswer-metadescription tgc-device/device-description)}
                  201 {:body (tgc-util/tgc-httpanswer-metadescription tgc-device/device-description)}
                  401 {:body (tgc-util/tgc-httpanswer-metadescription tgc-error/error-description) }
                  400 {:body (tgc-util/tgc-httpanswer-metadescription tgc-error/error-description) }
                  409 {:body (tgc-util/tgc-httpanswer-metadescription tgc-error/error-description) }
                  500 {:body (tgc-util/tgc-httpanswer-metadescription tgc-error/error-description) }
                  }
       :handler (fn [{params :body-params qparams :query-params}]
                 (let [subobjects (if (= (qparams "withSubObjects") "token") [:tokens] [])]
                      (let [[tmpdevicestatus tmpdevice] (tgc-device/check-for-device params)]
                        (cond 
                            (= :absent tmpdevicestatus) 
                              (let [newdevice (tgc-device/new-device! params [:tokens])] (response/created (str "/api/V1/device/" (newdevice :uuid)) newdevice))
                            (= :conflict tmpdevicestatus) 
                              (response/conflict (tgc-error/create-error 409 "tgc.error.conflict.device_already_exists"))
                            (= :exist tmpdevicestatus) 
                              (response/ok tmpdevice))
                        )))
            }
                                     
     }
    ] ; devices
    ["/devices/:uuid"
     {:get
      {:summary "Get a specific device"
       :responses {200 {:body (tgc-util/tgc-httpanswer-metadescription tgc-device/device-description)}
                   401 {:body (tgc-util/tgc-httpanswer-metadescription tgc-error/error-description) }
                   404 {:body (tgc-util/tgc-httpanswer-metadescription tgc-error/error-description) }
                }
       :handler  (fn [{{:keys [uuid]} :path-params qparams :query-params}]
                       (let [subobjects (if (= (qparams "withSubObjects") "token") [:token] [])
                             tmpdevice (tgc-device/get-device  uuid subobjects)]
                           (if (nil?  tmpdevice)
                              (response/not-found (tgc-error/create-error 404 "tgc.error.notfound.device_notexists"))
                              (response/ok tmpdevice)
                           )))



       }
      :delete
      {:summary "Ddelete a specific device"
       :responses {200 {:body (tgc-util/tgc-httpanswer-metadescription tgc-device/device-description)}
                   401 {:body (tgc-util/tgc-httpanswer-metadescription tgc-error/error-description) }
                   404 {:body (tgc-util/tgc-httpanswer-metadescription tgc-error/error-description) }
                }
       :handler (fn [_] (response/ok {})) 
      }
     }
    ]

    ];v1
   ];api
  )

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
   




