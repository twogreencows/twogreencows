

<template>
  <div>
    <h1>List of Devices</h1>
    <table>
      <thead>
        <tr>
          <th>UUID</th>
          <th>Kind</th>
          <th>Display Name</th>
          <th>Platform</th>
          <th>OS Version</th>
          <th>Vendor UUID</th>
          <th>OwnerUUID</th>
        </tr>
      </thead>
      <tbody>
        <tr v-for="object in objects" :key="object.id">
          <td>{{ object.uuid }}</td>
          <td>{{ object.kind}}</td>
          <td>{{ object.display_name }}</td>
          <td>{{ object.platform }}</td>
          <td>{{ object.os_version }}</td>
          <td>{{ object.vendor_uuid }}</td>
          <td>{{ object.owner_uuid}}</td>
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
        const response = await fetch('http://localhost:3000/api/V1/devices'); // Replace with your API endpoint
          
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
}
</style>
