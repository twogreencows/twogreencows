 (ns twogreenpots.routes.services
   (:require
    [reitit.swagger :as swagger]
    [reitit.swagger-ui :as swagger-ui]
    [reitit.ring.coercion :as coercion]
    [reitit.coercion.spec :as spec-coercion]
    [reitit.ring.middleware.muuntaja :as muuntaja]
    [reitit.ring.middleware.exception :as exception]
    [reitit.ring.middleware.multipart :as multipart]
    [reitit.ring.middleware.parameters :as parameters]
    [twogreenpots.users :as usrs]
    [twogreenpots.middleware :as middleware]
    [ring.util.http-response :as response]
    [twogreenpots.middleware.formats :as formats]))
 
(defn service-routes []
  ["/api"
   {:middleware [;;query-params & form-params
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
                 ;;multipart params
                 multipart/multipart-middleware
                 middleware/wrap-formats]
    :mmuntaja formats/instance
    :coercion  spec-coercion/coercion
    :swagger {:id ::api}}

   ["" {:no-doc true}
    ["/swagger.json" 
     {:get (swagger/create-swagger-handler)}]
    ["/swagger-ui*"
     {:get (swagger-ui/create-swagger-ui-handler {:url "/api/swagger.json"})}]
    ]
   ["/users"
    {:get
       (fn [_] (response/ok (usrs/user-list)))}]
       ;(fn [_] (response/ok {:status :ok}))}]
    ["/user"
     {:post
        (fn [{:keys [params]}]
          (try
            (do
              (let [newuser (usrs/new-user! params)]
               (response/ok (assoc {:status :ok} :user newuser))))
          (catch Exception e
            (let [{id :twogreenpots/error-id errors :errors} (ex-data e)]
            (case id
              :validation
                (response/bad-request {:errors  errors})
              ;;else
                (response/internal-server-error 
                {:errors {:server-error ["Failed to create user!"]}}))))))}]
    ])




