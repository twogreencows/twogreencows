(ns twogreenpots.core
  (:require [reagent.core :as r]
            [reagent.dom :as dom]
            [re-frame.core :as rf]
            [ajax.core :refer [GET POST]]
            [clojure.string :as string]
            [twogreenpots.validation :refer [validate-user]] ))

(rf/reg-event-fx :app/initialize (fn [_ _] {:db {:users/loading? true}}))

(rf/reg-event-db
  :user/add
  (fn [db [_ user]]
       (update db :users/list conj user)))


(defn send-user! [fields errors]
  (if-let [validation-errors (validate-user @fields)]
    (reset! errors validation-errors)
   (POST "/api/user"        
    {:format :json
     :headers {"Accept" "application/transit+json"
               "x-csrf-token" (.-value (.getElementById js/document "token"))}
     :params @fields 
     :handler #(do
              (rf/dispatch [:user/add (:user %)]) 
              (reset! fields nil)
              (reset! errors nil)
              )
     :error-handler #(do
                        (.log js/console (str "error " %))
                        (reset! errors (get-in % [:response :errors]))
                      )
                      }))
  )

(defn errors-component [errors id]
  (when-let [error (id @errors)]
    [:div.notification.is-danger (string/join error)]
  ))
 

(rf/reg-event-db
  :users/set
  (fn [db [_ users]]
       (-> db
           (assoc :users/loading? false :users/list users))))

(rf/reg-sub 
  :users/list
  (fn [db _]
    (:users/list db [])))


(defn get-users []
  (GET "/api/users"
       {:headers {"Accept" "application/transit+json"}
        :handler #(rf/dispatch [:users/set (:users %)])
        }))


(defn user-list [users]
  [:ul.users
   (for [{:keys [uuid display_name password phone]} @users]
        ^{:key uuid}
        [:li 
         [:p uuid]
         [:p display_name]
         [:p password]
         [:p phone]])])


(defn user-form []
  (let [fields (r/atom {})
        errors (r/atom nil)]
    (fn []
      [:div
      [:p "Name: " (:display_name @fields)]
      [:p "Password: " (:password @fields)]
      [:p "Phone: " (:phone @fields)]
      [errors-component errors :server-error]
       [:div.field
        [:label.label {:for :display_name} "Name"]
        [errors-component errors :display_name]
        [:input.input
          {:type :text 
           :name :display_name
           :on-change #(swap! fields assoc :display_name (-> % .-target .-value))
           :values (:display_name @fields)}]]
      [:div.field
        [:label.label {:for :password} "Password"]
        [errors-component errors :password]
        [:input.input
         {:type :password
          :name :password
          :value (:password @fields)
          :on-change #(swap! fields assoc :password (-> % .-target .-value))
        }]]
      [:div.field
        [:label.label {:for :phone} "Phone"]
        [errors-component errors :phone]
        [:input.input
         {:type :text
          :name :phone
          :value (:phone @fields)
          :on-change #(swap! fields assoc :phone (-> % .-target .-value))
        }]]
      [:input.button.is-primary
       {:type :submit
        :on-click #(send-user! fields errors)
        :value "Submit"}]
     ] 
      )))

(rf/reg-sub 
  :users/loading? 
  (fn [db _]
    (:users/loading? db)))


(defn home []
  (let [users (rf/subscribe [:users/list]) ]
    (rf/dispatch [:app/initialize])
    (get-users)
  (fn []   
   (if @(rf/subscribe [:users/loading?])
    [:div>div.row>div.span12>h3 "Loading users..."]  
  [:div.content>div.columns.is-centered>div.column.is-two-thirds
   [:div.columns>div.column
    [:h3 "Users"]]
    [user-list users]
   [:div.columns>div.column
    [user-form users]]]
  ))))
  



(dom/render
  [home]
  (.getElementById js/document "content")
)

  

