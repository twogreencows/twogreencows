import { createApp, reactive } from 'vue'
import './style.css'
import App from './App.vue'
import router from './router' 


const loginState = reactive({
  isLoggedIn: false,  // initial state
  userUuid: null,          // user information
  sessionUuid: null,          // user information
})


const app = createApp(App).use(router)

app.config.globalProperties.$loginState = loginState

app.config.errorHandler = (err, vm, info) => {
  console.error(err, info);
};

app.mount('#app')
