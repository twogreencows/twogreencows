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
context = {}
core_url = "http://localhost:3000/api/V1"
h = {"Content-Type":"application/json"}
context["user_uuid"]=""

#def test_v1_environment(endpoint="/environment", context=context) -> None:
#    pp.pprint("== Test GET environment")
#    r = requests.get(core_url+endpoint)
#    if r.status_code != 200:
#        pp.pprint("  ->Test FAILED  " + str(r.status_code)+ "\n")
#        pp.pprint(r.content)
#    else:
#        pp.pprint("  ->Test SUCCEEDED")
#        pp.pprint(r.json())
#    assert r.status_code == 200, f'Received wrong status code {r.status_code} instead of 200' 
#

#def test_v1_users_get_all(endpoint="/users", context=context) -> None:
#    pp.pprint("== Test GET all users")
#    r= requests.get( core_url+ endpoint )
#    if r.status_code != 200:
#        pp.pprint("  ->Test FAILED  " + str(r.status_code)+ "\n")
#        pp.pprint(r.content)
#    else:
#        pp.pprint("  ->Test SUCCEDED")
#        pp.pprint(r.json())
#    assert r.status_code == 200, f'Received wrong status code {r.status_code} instead of 200' 
#

def test_v1_users_postone_missingparams(endpoint="/users", context=context) -> None:
    pp.pprint(" == Test POST one user - missing parameters")

    r = requests.post(core_url+"/users",  headers=h , json={"display_name":"paul", "password":"yesterday","phone_number":"+33687853132"}) 
    pp.pprint(r.status_code)
    if r.status_code != 400:
        pp.pprint("  ->Test FAILED")
        pp.pprint(r.json())
        pp.pprint(r.status_code)
    else:
        pp.pprint('======')
        pp.pprint(r.json())
        pp.pprint(r.content)
        pp.pprint('======')

    assert r.status_code == 400, f'Received wrong status code {r.status_code} instead of 400' 

#def test_v1_users_postone_unmatchparams(endpoint="/users", context=context) -> None:
#    pp.pprint( "== Test POST one user - unmatched parameters")
#    assert False,  f'Test not implemented!'
#    #r=requests.post(core_url+"/users", headers=h , json={"display_name":"paul", "password":"yesterday","confirm_password":"yerblues","phone_number":"+33687853132"}) 
#    #pp.pprint(r.json())
#    #if r.status_code != 400:
#    #    pp.pprint("  ->Test FAILED")
#    #else:
#    #    pp.pprint(r.json())
#
#
#def test_v1_users_postone_goodcase(endpoint="/users", context=context) -> None:
#    pp.pprint("== Test POST one user - good parameters")
#    r=requests.post(core_url + endpoint, headers = h, json= {"display_name":"paul", "password":"yesterday","confirm_password":"yesterday","phone_number":"+33687853132"})
#    if r.status_code != 201 and  r.status_code != 200:
#        pp.pprint("  ->Test FAILED  " + str(r.status_code)+ "\n")
#        pp.pprint(r.content)
#    else:
#        pp.pprint("  ->Test SUCCEEDED")
#        pp.pprint(r.json())
#        context["user_uuid"] = r.json()["data"]["uuid"]
#
#    assert r.status_code == 200 or r.status_code == 201, f'Received wrong status code {r.status_code} instead of 200 or 201' 
#
#
#def test_v1_users_postone_conflictonexisting(endpoint="/users", context=context) -> None:
#    pp.pprint("== Test POST one user - conflicting passwords")
#    r=requests.post(core_url+endpoint, headers = h, json= {"display_name":"paul", "password":"sergeantpepper","confirm_password":"sergeantpepper","phone_number":"+33687853132"})
#    
#    if r.status_code != 409:
#        pp.pprint("  ->Test FAILED  " + str(r.status_code)+ "\n")
#        pp.pprint(r.content)
#    else:
#        pp.pprint("  ->Test SUCCEEDED")
#        pp.pprint(r.json())
#
#    assert r.status_code == 409, f'Received wrong status code {r.status_code} instead of 409' 
#
#
#def test_v1_users_postone_newonewithtokens(endpoint="/users", context=context) -> None:
#    pp.pprint("== Test POST one user - good parameters with tokens")
#    r= requests.post(core_url+ "/users?withSubObjects=tokens", headers = h, json={"display_name":"john", "password":"yerblues","confirm_password":"yerblues","phone_number":"+33687853133"})
#    
#    if r.status_code != 201 and r.status_code != 200:
#        pp.pprint("  ->Test FAILED  " + str(r.status_code)+ "\n")
#        pp.pprint(r.content)
#    else:
#        pp.pprint("  ->Test SUCCEEDED")
#        pp.pprint(r.json())
#
#    assert r.status_code == 200 or r.status_code == 201, f'Received wrong status code {r.status_code} instead of 200 or 201' 
#    
#
#def test_v1_users_getone_nonexisting(endpoint="/users", context=context) -> None:
#    pp.pprint("Test GET one user: not existing")
#    r=requests.get(core_url+ "/users/usr-ewklewlkewkwekelwk222kl")
#    if r.status_code != 404:
#        pp.pprint("  ->Test FAILED")
#        pp.pprint("  ->Test FAILED  " + str(r.status_code)+ "\n")
#        pp.pprint(r.content)
#    else:
#        pp.pprint("  ->Test SUCCEEDED")
#        pp.pprint(r.json())
#    assert r.status_code == 404, f'Received wrong status code {r.status_code} instead of 404' 
#
#
#def test_v1_users_getone_existing(endpoint="/users", context=context) -> None:
#    pp.pprint("== Test GET one user simple version ")
#    r=requests.get(core_url+ "/users/"+context["user_uuid"])
#    if r.status_code != 200:
#        pp.pprint("  ->Test FAILED  " + str(r.status_code)+ "\n")
#        pp.pprint(r.content)
#    else:
#        pp.pprint("  ->Test SUCCEEDED")
#        pp.pprint(r.json())
#    assert r.status_code == 200, f'Received wrong status code {r.status_code} instead of 200' 
#
#
##def test_v1_users_getone_existingwihtokens(endpoint="/users") -> None:
##    pp.pprint("== Test GET one user with their tokens")
##    r=requests.get(core_url+ "/users/"+user_uuid+"?withSubObjects=tokens")
##    if r.status_code != 200:
##        pp.pprint("  ->Test FAILED  " + str(r.status_code)+ "\n")
##        pp.pprint(r.content)
##    else:
##        pp.pprint("  ->Test SUCCEEDED")
##        pp.pprint(r.json())
##    assert r.status_code == 200, f'Received wrong status code {r.status_code} instead of 200' 
##
##
##def test_v1_users_getonetokens__existing(endpoint="/users") -> None:
##    pp.pprint("== Test GET one user with their tokens")
##    r=requests.get(core_url+ "/users/"+user_uuid+"/tokens")
##    if r.status_code != 200:
##        pp.pprint("  ->Test FAILED  " + str(r.status_code)+ "\n")
##        pp.pprint(r.content)
##    else:
##        pp.pprint("  ->Test SUCCEEDED")
##        pp.pprint(r.json())
##    assert r.status_code == 200, f'Received wrong status code {r.status_code} instead of 200' 
##
##
##def test_v1_users_addonetokens(endpoint="/users") -> None:
##    pp.pprint("== POST new token on user")
##    r=requests.post(core_url+ "/users/"+user_uuid+"/tokens")
##    if r.status_code != 201:
##        pp.pprint("  ->Test FAILED  " + str(r.status_code)+ "\n")
##        pp.pprint(r.content)
##    else:
##        pp.pprint("  ->Test SUCCEEDED")
##        pp.pprint(r.json())
##    assert  r.status_code == 201, f'Received wrong status code {r.status_code} or 201' 
##
##
##def test_v1_users_deleteonetokens(endpoint="/users") -> None:
##    #pp.pprint("Test GET one user: existing")
##    pp.pprint("Test GET all tokens: not existing")
##    r=requests.get(core_url+ "/users/"+user_uuid)
##    if r.status_code != 204:
##        pp.pprint("  ->Test FAILED  " + str(r.status_code)+ "\n")
##        pp.pprint(r.content)
##    else:
##        pp.pprint("  ->Test SUCCEEDED")
##        pp.pprint(r.json())
##    assert r.status_code == 204, f'Received wrong status code {r.status_code} instead of 204' 
##
##
#def test_v1_users_deleteone_nonexisting(endpoint="/users", context=context) -> None:
#    pp.pprint("== Test DELETE user : not existing")
#    r=requests.delete(core_url+ "/users/u-aaaaaaaaaaaaaaaa")
#    if r.status_code != 404:
#        pp.pprint("  ->Test FAILED  " + str(r.status_code)+ "\n")
#        pp.pprint(r.content)
#    else:
#        pp.pprint("  ->Test SUCCEEDED")
#        pp.pprint(r.json())
#    assert r.status_code == 404, f'Received wrong status code {r.status_code} instead of 404' 
#
#
#
#def test_v1_users_deleteone_existing(endpoint="/users", context=context) -> None:
#    pp.pprint("== Test DELETE one user: existing")
#    r=requests.delete(core_url+ "/users/"+context["user_uuid"])
#    if r.status_code != 200:
#        pp.pprint("  ->Test FAILED  " + str(r.status_code)+ "\n")
#        pp.pprint(r.content)
#    else:
#        pp.pprint(r.json())
#        pp.pprint("  ->Test SUCCEEDED")
#    
#    assert r.status_code == 200, f'Received wrong status code {r.status_code} instead of 200' 
#
#
#
#
