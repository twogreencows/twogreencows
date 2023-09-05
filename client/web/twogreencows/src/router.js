import { createWebHistory, createRouter } from 'vue-router';
import Users  from './components/Users.vue';
import Sessions from './components/Sessions.vue';
import Tokens from './components/Tokens.vue';
import Devices from './components/Devices.vue';
//import Home from '@/components/Greenhouses.vue';
//import Home from '@/components/Growboxes.vue';
//import Home from '@/components/Plants.vue';
//import Home from '@/components/Stems.vue';

const routes = [
	{
		path: '/users',
		name: 'Users',
		component: Users,
	},	
        {
		path: '/tokens',
		name: 'Tokens',
		component: Tokens,
	},
        {
		path: '/sessions',
		name: 'Sessions',
		component: Sessions,
	},
        {
		path: '/devices',
		name: 'Devices',
		component: Devices,
	},

];

const router = createRouter({
	history: createWebHistory(),
	routes,
});

export default router;
