

<template>
  <div>
    <h1>List of Sessions</h1>
    <table>
      <thead>
        <tr>
          <th>UUID</th>
          <th>Terminated at</th>
          <th>New User</th>
          <th>New Device</th>
          <th>Device UUID</th>
          <th>User UUID</th>
          <th>Token UUID</th>
        </tr>
      </thead>
      <tbody>
        <tr v-for="object in objects" :key="object.id">
          <td>{{ object.uuid }}</td>
          <td>{{ object.terminated_at}}</td>
          <td>{{ object.is_new_user}}</td>
          <td>{{ object.is_new_device}}</td>
          <td>{{ object.device_uuid }}</td>
          <td>{{ object.user_uuid }}</td>
          <td>{{ object.token_uuid }}</td>
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
        const response = await fetch('http://localhost:3000/api/V1/sessions'); // Replace with your API endpoint
          
        const data = await response.json();
          debugger;
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
