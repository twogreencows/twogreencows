(ns twogreencows.core
  (:require [reagent.core :as r]
            [reagent.dom :as dom]
            [re-frame.core :as rf]
            [ajax.core :refer [GET POST]]
            [clojure.string :as string]
            ;[twogreencows.validation :refer [validate-user]]
            [twogreencows.websockets :as ws]
            [mount.core :as mount]))

(rf/reg-event-fx :app/initialize (fn [_ _] {:db {:users/loading? true}
                                            :dispatch [:users/load]
                                            }))

(rf/reg-event-db
  :user/add
  (fn [db [_ user]]
       (update db :users/list conj user)))


(rf/reg-event-fx
  :user/send!
  (fn [{:keys [db]} [_ fields]]
    (ws/send! [:user/create! fields])
    {:db (dissoc db :form/server-errors)}))

(defn handle-response! [response]
  (if-let [errors (:errors response)]
    (rf/dispatch [:form/set-server-errors  errors])
    (do 
      (rf/dispatch [:user/add response])
      (rf/dispatch [:form/clear-fields response]))))


(defn errors-component [id]
  (when-let [error @(rf/subscribe [:form/error id])]
    [:div.notification.is-danger (string/join error)]
  ))
 

(rf/reg-event-db
  :users/set
  (fn [db [_ users]]
       (-> db
           (assoc :users/loading? false :users/list users))))

(rf/reg-event-db
  :form/set-field
  [(rf/path :form/fields)]
       (fn [fields [_ id value]]
           (assoc fields id value )))

(rf/reg-event-db
  :form/clear-fields
  [(rf/path :form/fields)]
       (fn [_ _]
           {}))


(rf/reg-sub 
  :form/fields
  (fn [db _]
    (:form/fields db )))


(rf/reg-sub 
  :form/field
  :<- [:form/fields]
  (fn [fields [_ id]]
    (get fields id)))


(rf/reg-sub 
  :users/list
  (fn [db _]
    (:users/list db [])))

(rf/reg-event-fx
  :users/load
  (fn [{:keys [db]} _]
  (GET "/api/users"
       {:headers {"Accept" "application/transit+json"}
        :handler #(rf/dispatch [:users/set (:users %)])})
    {:db (assoc db :users/loading? true)}))


(defn user-list [users]
  [:ul.users
   (for [{:keys [uuid creation_date display_name password phone]} @users]
        ^{:key uuid}
        [:li 
         [:p uuid]
         [:p (.toLocaleString creation_date)]
         [:p display_name]
         [:p password]
         [:p phone]])])


(defn user-form []
      [:div
      [:p "Name: " @(rf/subscribe [:form/field :display_name])]
      [:p "Password: " @(rf/subscribe [:form/field :password])]
      [:p "Phone: " @(rf/subscribe [:form/field :phone])]
      
      [errors-component :server-error]
       [:div.field
        [:label.label {:for :display_name} "Name"]
        [errors-component :display_name]
        [:input.input
          {:type :text 
           :name :display_name
           :on-change #(rf/dispatch [:form/set-field :display_name (.. % -target -value)])
           :values @(rf/subscribe [:form/field :display_name])}]]
      [:div.field
        [:label.label {:for :password} "Password"]
        [errors-component :password]
        [:input.input
         {:type :password
          :name :password
          :value @(rf/subscribe  [:form/field :password])
          :on-change #(rf/dispatch [:form/set-field :password (.. % -target -value)])
        }]]
      [:div.field
        [:label.label {:for :phone} "Phone"]
        [errors-component :phone]
        [:input.input
         {:type :text
          :name :phone
          :value @(rf/subscribe [:form/field :phone])
          :on-change #(rf/dispatch [:form/set-field :phone (.. % -target -value)])
        }]]
      [:input.button.is-primary
       {:type :submit
        :disabled @(rf/subscribe [:form/validation-errors?])
        :on-click #(rf/dispatch [:user/send! @(rf/subscribe [:form/fields])])
        :value "Submit"
        }]
      ])

(rf/reg-sub 
  :users/loading? 
  (fn [db _]
    (:users/loading? db)))

(defn reload-users-button []
  (let [loading? (rf/subscribe [:users/loading?])]
    [:button.button.is-info.is-fullwidth
     {:on-click  #(rf/dispatch [:users/load])
      :disabled @loading?}
     (if @loading?
       "Loading Users" "Refresh Users")])
)

(defn home []
  (let [users (rf/subscribe [:users/list])]
  (fn []   
    [:div.content>div.columns.is-centered>div.column.is-two-thirds
    (if @(rf/subscribe [:users/loading?])
      [:h3 "Loading users..."]  
        [:div
          [:div.columns>div.column
            [:h3 "Users"]]
            [user-list users]
          [:div.columns>div.column
            [reload-users-button]]
          [:div.columns>div.column
            [user-form]]])])))

(defn ^:dev/after-load mount-components []
  (rf/clear-subscription-cache!)
    (.log js/console "Mounting Components...")
    (dom/render [#'home] (.getElementById js/document "content"))
    (.log js/console "Components Mounted!"))

(defn init! []
  (.log js/console "Initialing App ")
  (mount/start)
  (rf/dispatch [:app/initialize])
  ;(ws/connect! (str "ws://" (.-host js/location) "/ws") handle-response!)
  (mount-components))



(rf/reg-event-db
  :form/set-server-errors
  [(rf/path :form/server-errors)]
  (fn [_ [_ errors]]
      errors))

(rf/reg-sub
   :form/server-errors
   (fn [db _]
     (:form/server-errors db)))

(rf/reg-sub
  :form/validation-errors
  :<- [:form/fields]
  (fn [fields _]
    (empty? fields)))

(rf/reg-sub
  :form/validation-errors?
  :<- [:form/validation-errors]
  (fn [errors _]
    (not (empty? errors))))


(rf/reg-sub
  :form/errors
  :<- [:form/validation-errors]
  :<- [:form/server-errors]
  (fn [[validation server] _]
    (merge validation server)))


(rf/reg-sub
  :form/error
  :<- [:form/errors]
  (fn [errors [_ id]]
    (get errors id)))


