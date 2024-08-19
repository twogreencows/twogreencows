import { createWebHistory, createRouter } from 'vue-router';
import Users  from './components/Users.vue';
import Sessions from './components/Sessions.vue';
import Tokens from './components/Tokens.vue';
import Devices from './components/Devices.vue';
import LoginPage from './components/LoginPage.vue';
import MainPage from './components/MainPage.vue';
import Greenhouses from './components/Greenhouses.vue';
import Growboxes from './components/Growboxes.vue';
import Plants from './components/Plants.vue';
import Stems from './components/Stems.vue';

const routes = [
        {
		path: '/login',
		name: 'Login',
		component: LoginPage,
	},	
        {
		path: '/main',
		name: 'Main',
		component: MainPage,
                children:[
                    {
                        path:'users',
                        component:Users
                    },
                    {
                        path:'devices',
                        component:Devices
                    },
                    {
                        path:'sessions',
                        component:Sessions
                    },
                    {
                        path:'tokens',
                        component:Tokens
                    },
                    {
                        path:'plants',
                        component:Plants
                    },
                    {
                        path:'stems',
                        component:Stems
                    },
                    {
                        path:'greenhouses',
                        component:Greenhouses
                    },
                    {
                        path:'growboxes',
                        component:Growboxes
                    }
                ]
	}	

];

const router = createRouter({
	history: createWebHistory(),
	routes,
});

export default router;
