<template>
    <div class="loginpage">
        <div class="loginblock">
            <form id="signin">
                <div class="loginelement">
	        <label for="username">Username</label>
	            <input type="text" id="username" v-model.trim="username">
                    <h6 id="logintype">{{ loginType }}</h6>
                </div>
                <div class="loginelement">
	            <label for="password">Password</label>
	            <input type="password" id="password" v-model.trim="password">
                </div>
                <div class="loginelement" v-if="!insigninmode">
	            <label for="confirmpassword">Confirm Password</label>
	            <input type="password" id="confirmpassword" v-model.trim="confirmpassword">
                </div>

                <div class="loginelement">
                    <button id="loginbutton" :disabled="!buttonEnabled"  @click.stop.prevent="doLogin()" >{{ insigninmode ? "Sign In" : "Sign Up" }}</button>
                </div>
                <div class ="loginelement">
                    <h4 id="loginmodeswitch" @click="doSwitchMode()" >{{ insigninmode ? "No Account? Create one" : "Already have an account?" }} </h4>
                </div>
        </form>
        </div>
    </div>
</template>

<script setup>
import { ref, computed } from "vue";
import * as libphonenumber from "libphonenumber-js";

const props = defineProps(['username', 'password', 'confirmpassword'])
const emit = defineEmits(['update:username', 'update:password', 'update:confirmpassword'])

const username=ref(null)
const password=ref(null)
const confirmpassword=ref(null)
const insigninmode=ref(true)

const buttonEnabled =  computed(() => {
        return  insigninmode.value == true
})
    

const loginType  = computed(() => {
  let isEmail = String(username.value) .toLowerCase().match(
      /^(([^<>()[\]\\.,;:\s@"]+(\.[^<>()[\]\\.,;:\s@"]+)*)|.(".+"))@((\[[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\])|(([a-zA-Z\-0-9]+\.)+[a-zA-Z]{2,}))$/
    );

  
  let isPhoneNumber = false 
  if (username.value != null) {
     isPhoneNumber = libphonenumber.isPossiblePhoneNumber(username.value);
     try {
        let parsedPhoneData = libphonenumber.parsePhoneNumber(username.value);
    }
    catch(err) { 
    }
   }
  let isDisplayName = !isEmail && !isPhoneNumber;
  if (isDisplayName == true) {
    if (username == null || username.value == null || username.value.length < 4) {
        isDisplayName = false;
    }
  }

  return isEmail  ? 'email' : (isPhoneNumber ? "phone_number" : (isDisplayName ? 'display_name': 'none'));
})

//const computed

function doSwitchMode() {
    insigninmode.value = !(insigninmode.value);
}

function doLogin() {
    if (insigninmode) {
            console.log("doLogin");
    } else {
            console.log("doSignup");
    }
    var mode =  insigninmode.value  ? 'signin' :  'signup';
    const doLoginRequest = async () => {
      try {
        const settings = {
            method: 'POST',
            body: JSON.stringify({"user":{ loginType: username.value, "password":password.value, "confirm_password":confirmpassword.value }, "device":{}}),
            headers: {
                Accept: 'application/json',
                'Content-Type': 'application/json',
            }
        };


        console.log('Posting session' );
        const response = await fetch('http://localhost:3000/api/V1/sessions?mode='+mode, settings); // Replace with your API endpoint
        const data = await response.json();
      } catch (error) {
        console.error('Error posting session', error);
      }
    };
    doLoginRequest()
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
    padding-top:10px;
    padding-bottom:10px;
}

#signin {
    width:100%;
    padding:20px 0px 20px 0px; 
}

</style>

