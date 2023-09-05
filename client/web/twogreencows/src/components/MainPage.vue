<template>
    <div class="mainpage">
    <header class="header">
        <main-header></main-header>
    </header>
    <div class="content">
      <nav class="left-column">
        <main-navigation></main-navigation>
      </nav>
      <div class="main-zone">
          <router-view></router-view>
      </div>
    </div>
    </div>
</template>

<script>
import Users from './Users.vue'
import Tokens from './Tokens.vue'
import Sessions from './Sessions.vue'
import Devices from './Devices.vue'
import MainHeader from './MainHeader.vue'
import MainNavigation from './MainNavigation.vue'

export default {
    components: {
        Users,
        Tokens,
        Sessions,
        Devices,
        MainHeader,
        MainNavigation,
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
  flex: 1;
  background-color: #f0f0f0;
}

.main-zone {
  flex: 3;
  background-color: white;
}
</style>

