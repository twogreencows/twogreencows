

<template>
    <section  id="ncontainer">
        <div id="nheader">
        <button id="visibilitytoggle" @click="toggleVisibility">{{ navState.visible ? "Hide" : "Show"}} </button>
        </div>
        <ul id="ncontent">
            <li class="nitem"><router-link to="/main/users">Users</router-link></li>
            <li class="nitem"><router-link to="/main/sessions">Sessions</router-link></li>
            <li class="nitem"><router-link to="/main/tokens">Tokens</router-link></li>
            <li class="nitem"><router-link to="/main/devices">Devices</router-link></li>
            <li class="nitem"><router-link to="/main/greenhouses">Greenhouses</router-link></li>
            <li class="nitem"><router-link to="/main/growboxes">Growboxes</router-link></li>
            <li class="nitem"><router-link to="/main/plants">Plants</router-link></li>
            <li class="nitem"><router-link to="/main/stems">Stems</router-link></li>
        </ul>
    </section>
</template>

<script setup>
    import { ref, reactive, onMounted, defineComponent } from 'vue'
    const emit = defineEmits(['paneNavigation'])
    
    const navState = reactive({
        visible:true,
        items:["Users", "Sessions", "Tokens", "Devices", "Greenhouses", "Growboxes", "Plants", "Stems"]
    })

    
     function toggleVisibility() {
            navState.visible = !(navState.visible)          
            emit('paneNavigation', navState.visible)
     } 
        
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

#visibilitytoggle {
    height:32px;
    width:64px;
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

.nav-slide-enter-active {
  transition: all 0.3s ease-out;
}

.nav-slide-leave-active {
  transition: all 0.8s cubic-bezier(1, 0.5, 0.8, 1);
}

.nav-slide-enter-from,
.nav-slide-leave-to {
  transform: translateX(20px);
}
</style>
