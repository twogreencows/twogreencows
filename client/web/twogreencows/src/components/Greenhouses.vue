

<template>
  <div>
    <h1>List of Greenhouses</h1>
    <table>
      <thead>
        <tr>
          <th>UUID</th>
          <th>Display Name</th>
          <th>Description</th>
          <th>Latitude</th>
          <th>Longitude</th>
          <th>Owner UUID</th>
        </tr>
      </thead>
      <tbody>
        <tr v-for="object in objects" :key="object.id">
          <td>{{ object.uuid }}</td>
          <td>{{ object.display_name}}</td>
          <td>{{ object.description}}</td>
          <td>{{ object.latitude}}</td>
          <td>{{ object.longitude}}</td>
          <td>{{ object.ownder_uuid }}</td>
        </tr>
      </tbody>
    </table>
  </div>
</template>

<script>
    import { ref, onMounted } from 'vue'

    export default {
    setup() {
    const objects = ref([]);

    const fetchObjects = async () => {
      try {
        const response = await fetch('http://localhost:3000/api/V1/greenhouses'); // Replace with your API endpoint
          
        const data = await response.json();
        console.log(data["data"])
        objects.value = data["data"];
      } catch (error) {
        console.error('Error fetching objects:', error);
      }
    };

    onMounted(fetchObjects);

    return {
      objects,
    };
  },
};
</script>

<style>
table {
  width: 100%;
  border-collapse: collapse;
}

th,
td {
  border: 1px solid black;
  padding: 8px;
  color:black;
}
</style>
