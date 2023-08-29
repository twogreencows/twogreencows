

<template>
    <section id="hcontainer">
            <div id="htitle"> Twogreencows </div>
            <div id="hserver">{{server.name}} - {{server.version}}</div>
    </section>
</template>

<script>
    import { ref, reactive, onMounted } from 'vue'

    export default {
    setup() {
    const server = reactive({
        name : '--',
        version :'0.0.0',
    }) 

    const fetchObjects = async () => {
      try {
        const response = await fetch('http://localhost:3000/api/V1/environment'); // Replace with your API endpoint
          
        const data = await response.json();
        server.name = data["data"]["name"];
        server.version = data["data"]["sem_version"];
      } catch (error) {
        console.error('Error fetching environment:', error);
      }
    };

    onMounted(fetchObjects);

    return {
      server,
    };
  },
};
</script>

<style scoped>

#hcontainer{
    display:flex;
    padding-left:32px;
    padding-right:32px;
    flex-direction:row;
    align-items: center;
    height:100%;
    justify-content: space-between;
}

#htitle{
    text-align:left;
    color:white;
    font-size:24px;
    font-weight:bold;
    font-family: 'lato';
    order: 1;
}


#hserver{
    text-align:left;
    color:white;
    font-size:16px;
    font-weight:thin;
    font-family: 'lato';
    order: 2;
}



</style>
