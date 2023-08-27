<template>
    <div class="app-container">
    <main-page></main-page>
    </div>
</template>

<script >
import MainPage from './components/MainPage.vue'

export default {
  components: {
    MainPage,
  },
};

</script>

<style>
.app-container {
    background-color: red;
}

</style>

