<template>
    <div class="loginpage">
        <div class="loginblock">
            <form id="signin">
                <div class="loginelement" id="loginusername">
                    <label for="username">Username: </label>
                    <input type="text" id="username" v-model.trim="username">
	            <h6 id="loginextra">This will be a login with: {{ loginType }}</h6>
                </div>

                <div class="loginelement" id="loginpassword">
	            <label for="password">Password</label>
	            <input type="password" id="password" v-model.trim="password">
	            <h6 id="passwordextra">{{ passwordValid ? "Password OK" : "Password should have more than 8 char and a digit"  }}</h6>
                </div>

                <div class="loginelement" id="loginconfirmpassword" v-if="!insigninmode">
	            <label for="confirmpassword">Confirm Password</label>
	            <input type="password" id="confirmpassword" v-model.trim="confirmpassword">
	            <h6 id="confirmpasswordextra">{{ confirmPasswordValid? "Confirm Password OK" : "Confirm Password should match password" }}</h6>
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
import UAParser from 'ua-parser-js';
import { useRouter, useRoute } from 'vue-router' 

const props = defineProps(['username', 'password', 'confirmpassword'])
const emit = defineEmits(['update:username', 'update:password', 'update:confirmpassword'])

const router = useRouter()

const username=ref(null)
const password=ref(null)
const confirmpassword=ref(null)
const insigninmode=ref(true)

const buttonEnabled =  computed(() => {
    console.log('buttonEnabled called');
    if (insigninmode.value == true) {
        return loginType.value != 'none' && passwordValid.value == true;
    } else {
        return loginType.value != 'none' && passwordValid.value == true  && confirmPasswordValid.value == true;
    }
})

const passwordValid  = computed(() => {
   return(password.value != null && password.value.length >= 8 && /[0-9]/.test(password.value));
})


const confirmPasswordValid  = computed(() => {
    return (password.value === confirmpassword.value);
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


function doSwitchMode() {
    insigninmode.value = !(insigninmode.value);
}

function doLogin() {
    var mode =  insigninmode.value  ? 'signin' :  'signup';
    const doLoginRequest = async () => {
      try {
        var parser = new UAParser();
        var result = parser.getResult();
        let  bdict = {"user":{ "password":password.value, "confirm_password":confirmpassword.value },
                 "device":{"kind":"web*", "platform":result.os.name, "display_name": result.browser.name, "os_version": result.os.version  }}
        bdict["user"][loginType.value] = username.value;         
        const settings = {
            method: 'POST',
            body: JSON.stringify(bdict),
            headers: {
                Accept: 'application/json',
                'Content-Type': 'application/json',
            }
        };

        const response = await fetch('http://localhost:3000/api/V1/sessions?mode='+mode, settings); // Replace with your API endpoint
        const data = await response.json();
        console.log(data);
      } catch (error) {
        console.error('Error posting session', error);
        alert(error);
      }
    };
    doLoginRequest()
    //router.push("main/users");
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

#loginbutton {
    width:80%;
}
</style>

