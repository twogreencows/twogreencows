
<template>
    <div class="app-container">
        <router-view></router-view>
    </div>
</template>

<script setup>
import MainPage from './components/MainPage.vue'
import LoginPage from './components/LoginPage.vue'
import { useRouter, useRoute } from 'vue-router' 
import { getCurrentInstance } from 'vue';

const router = useRouter()
const route = useRoute()

const instance = getCurrentInstance();
const appContext = instance.appContext;

if (false == instance.appContext.config.globalProperties.$loginState.isLoggedIn) {
   router.push("login")
} else {
   router.push("main/users")
}

</script>

<style>
.app-container {
    background-color: red;
}

</style>

