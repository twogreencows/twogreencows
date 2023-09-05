

<template>
    <section id="ncontainer">
        <ul>
            <li class="nitem"> <router-link to="/Users">Users</router-link></li>
            <li class="nitem"><router-link to="/Sessions">Sessions</router-link></li>
            <li class="nitem"><router-link to="/Tokens">Tokens</router-link></li>
            <li class="nitem"><router-link to="/Devices">Devices</router-link></li>
            <li class="nitem"><router-link to="/Greenhouses">Greenhouses</router-link></li>
            <li class="nitem"><router-link to="/Growboxes">Growboxes</router-link></li>
            <li class="nitem"><router-link to="/Plants">Plants</router-link></li>
            <li class="nitem"><router-link to="/Stems">Stems</router-link></li>
        </ul>
    </section>
</template>

<script>
    import { ref, reactive, onMounted } from 'vue'

    export default {
    setup() {
    const menu = reactive({
    }) 


    return {
      menu,
    };
  },
};
</script>

<style scoped>
ul {
  list-style-type: none;
  margin: 0;
  display:flex;
  padding: 0;
  flex-direction:column;
}

li  {
  height:48px;
  padding-left:12px;
  border-bottom: 1px solid white;
  width: 100%;
  display:flex;
  align-items:center;
}

li a{
  color:black;
  font-size:18px;
  font-family: 'lato';
  text-align:left;
}

li a :hover {
  background-color: #555;
  color: white;
}

</style>
