<template>
    <div class="loginpage">
        <div class="loginblock">
            <form id="signin">
                <div class="loginelement">
	        <label for="username">Username</label>
	        <input type="text" id="username" v-model="username">
                </div>
                <div class="loginelement">
	        <label for="password">Password</label>
	        <input type="password" id="password" v-model="password">
                </div>
                <div class="loginelement" v-if="!insigninmode">
	            <label for="confirmpassword">Confirm Password</label>
	            <input type="password" id="password" v-model="password">
                </div>

                <div class="loginelement">
                    <button id="loginbutton" @click="doLogin()">{{ insigninmode ? "Sign In" : "Sign Up" }}</button>
                </div>
                <div class ="loginelement">
                    <h4 id="loginmodeswitch" @click="doSwitchMode()" >{{ insigninmode ? "Create an account" : "Already have an account?" }} </h4>
                </div>
        </form>
        </div>
    </div>
</template>

<script setup>
import { ref } from "vue";

const props = defineProps(['username', 'password'])
const emit = defineEmits(['update:username', 'update:password'])

const username=ref(null)
const password=ref(null)
const insigninmode=ref(true)


function doSwitchMode() {
    insigninmode.value = !(insigninmode.value);
}

function doLogin() {
    const doLoginRequest = async () => {
      try {
        const settings = {
            method: 'POST',
            headers: {
                Accept: 'application/json',
                'Content-Type': 'application/json',
            }
        };

        user = {}
        device = {}

        const response = await fetch('http://localhost:3000/api/V1/sessions', settings); // Replace with your API endpoint
        const data = await response.json();
      } catch (error) {
        console.error('Error posting session', error);
      }
    };

}

</script>

<style >
.loginpage {
  height: 100vh;
  display: flex;
  justify-content: center;
  align-items: center;
}

.loginblock {
  background-color: gray;
  border:solid;
  display:flex;
  justify-content:center;
  border-radius: 10px;
  width:320px;
}

.loginelement{
    width:100%;
    padding-top:20px;
}

</style>

