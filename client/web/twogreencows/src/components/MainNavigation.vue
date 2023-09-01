

<template>
    <section id="ncontainer">
        <ul>
            <li class="nitem"><a>Users</a></li>
            <li class="nitem"><a>Sessions</a></li>
            <li class="nitem"><a>Tokens</a></li>
            <li class="nitem"><a>Devices</a></li>
            <li class="nitem"><a>Greenhouses</a></li>
            <li class="nitem"><a>Growboxes</a></li>
            <li class="nitem"><a>Plants</a></li>
            <li class="nitem"><a>Stems</a></li>
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

</style>
