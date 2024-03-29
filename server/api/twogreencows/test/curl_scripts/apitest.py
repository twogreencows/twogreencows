#!/usr/bin/env python3

import requests
import pprint as pp
import os
from os import environ
import sys
#import pytest
import datetime
import base64

#from simplejson import JSONDecodeError


#@pytest.mark.skipif(not TOKEN_RETRIEVED, reason="Couldn't retrieved the bearer token")
#def test_v1_core_sessions(endpoint="v1/core/sessions") -> None:

core_url = "http://localhost:3000/api/V1"
h = {"Content-Type":"application/json"}
user_uuid=""

pp.pprint("= Test GET environment")
r = requests.get(core_url+"/environment")
if r.status_code != 200:
    pp.pprint("  ->Test FAILED  " + str(r.status_code)+ "\n")
    pp.pprint(r.content)
else:
    pp.pprint("  ->Test SUCCEDED")
    pp.pprint(r.json())


pp.pprint("Test GET all users")
r= requests.get( core_url+ "/users")
if r.status_code != 200:
    pp.pprint("  ->Test FAILED")
    pp.pprint(r.status_code)
    pp.pprint(r.content)
else:
    pp.pprint("  ->Test SUCCEDED")
    pp.pprint(r.json())
print()

#pp.pprint("Test POST one user - missing parameters")
#r = requests.post(core_url+"/users",  headers=h , json={"display_name":"paul", "password":"yesterday","phone_number":"+33687853132"}) 
#pp.pprint(r.status_code)
#if r.status_code != 400:
#    pp.pprint("  ->Test FAILED")
#    pp.pprint(r.json())
#    pp.pprint(r.status_code)
#else:
#    pp.pprint(r.json())



#
#pp.pprint( "Test POST one user - unmatched parameters")
#r=requests.post(core_url+"/users", headers=h , json={"display_name":"paul", "password":"yesterday","confirm_password":"yerblues","phone_number":"+33687853132"}) 
#pp.pprint(r.json())
#if r.status_code != 400:
#    pp.pprint("  ->Test FAILED")
#else:
#    pp.pprint(r.json())

print()


pp.pprint("Test POST one user - good parameters")
r=requests.post(core_url+ "/users", headers = h, json= {"display_name":"paul", "password":"yesterday","confirm_password":"yesterday","phone_number":"+33687853132"})
if r.status_code != 201 and  r.status_code != 200:
    pp.pprint("  ->Test FAILED")
    pp.pprint(r.json())
    pp.pprint(r.status_code)
else:
    pp.pprint("  ->Test SUCCEDED")
    pp.pprint(r.json())
    user_uuid = r.json()["data"]["uuid"]
    pp.pprint(user_uuid)

pp.pprint("Test POST one user - conflicting passwords")
r=requests.post(core_url+ "/users", headers = h, json= {"display_name":"paul", "password":"sergeantpepper","confirm_password":"sergeantpepper","phone_number":"+33687853132"})
if r.status_code != 409:
    pp.pprint("  ->Test FAILED")
    pp.pprint(r.json())
    pp.pprint(r.status_code)
else:
    pp.pprint("  ->Test SUCCEDED")
    pp.pprint(r.json())


print()
pp.pprint("Test POST one user - good parameters with tokens")
#r=requests.post = (core_url+ "/users?withToken=true", headers={"Content-Type: application/json"}, json={"display_name":"john", "password":"yerblues","confirm_password":"yerblues","phone_number":"+33687853133"})
if r.status_code != 201 and r.status_code != 200:
    pp.pprint("  ->Test FAILED")
    pp.pprint(r.json())
else:
    pp.pprint("  ->Test SUCCEDED")
    pp.pprint(r.json())

print()

pp.pprint("Test POST one user - mismatch password on existing user")
r=requests.post( core_url+"/users", headers = {"Content-Type: application/json"}, json= {"display_name":"paul", "password":"yerblues","confirm_password":"yerblues","phone_number":"+33687853132"})
if r.status_code != 409:
    pp.pprint("  ->Test FAILED")
else:
    pp.pprint(r.status_code)
    pp.pprint("  ->Test SUCCEDED")
    pp.pprint(r.json())

print()


pp.pprint("Test GET one user: not existing")
r=requests.get(core_url+ "/users/usr-ewklewlkewkwekelwk222kl")
if r.status_code != 404:
    pp.pprint("  ->Test FAILED")
else:
    pp.pprint(r.json())

print()


#pp.pprint("Test GET one user: existing")
pp.pprint("Test GET all tokens: not existing")
r=requests.get(core_url+ "/tokens/")
if r.status_code != 200:
    pp.pprint("  ->Test FAILED")
else:
    pp.pprint(r.json())

print()



