<template>
    <div class="mainpage">
    <header class="header">
        <main-header></main-header>
    </header>
    <div class="content">
      <div class="left-column">Left Column</div>
      
      <div class="main-zone">
          <users></users> 
      </div>
    </div>
    </div>
</template>

<script>
import Users from './Users.vue'
import MainHeader from './MainHeader.vue'

export default {
    components: {
        Users,
        MainHeader,
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

