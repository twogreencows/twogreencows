<template>
    <div class="mainpage">
        <header class="header" id="header" ref="header">
            <main-header></main-header>
        </header>
        <div class="content">
            <nav class="left-column" id="leftcolumn" ref="leftcolumn"> 
                <main-navigation @pane-navigation="showHideNav"></main-navigation>
            </nav>
            <div class="main-zone" id="mainzone" ref="mainzone">
                <router-view></router-view>
            </div>
        </div>
    </div>
</template>

<script setup>
import { ref } from "vue";
import Users from './Users.vue'
import Tokens from './Tokens.vue'
import Sessions from './Sessions.vue'
import Devices from './Devices.vue'
import Plants from './Plants.vue'
import Stems from './Stems.vue'
import Growboxes from './Growboxes.vue'
import Greenhouses from './Greenhouses.vue'
import MainHeader from './MainHeader.vue'
import MainNavigation from './MainNavigation.vue'

const header=ref(null)
const leftcolumn=ref(null)
const mainzone=ref(null)

function showHideNav(event) {
   if (event == true) {
        leftcolumn.value.style.flexBasis="250px";
        leftcolumn.value.style.color="yellow";
   } else {
        leftcolumn.value.style.flexBasis="120px";
        leftcolumn.value.style.color="red";
   }
}


</script>

<style >
.mainpage {
  display: flex;
  flex-direction: column;
  background-color:red;
  height: 100vh;
}

.header {
  background-color: green;
  height:64px;
  align-items: center;
}

.content {
  display: flex;
  flex: 1;
}

.left-column {
  flex-basis: 150px;
  flex-shrink:0;
  flex-grow:0;
  background-color: #f0f0f0;
}

.main-zone {
  flex: 3;
  background-color: white;
}
</style>

