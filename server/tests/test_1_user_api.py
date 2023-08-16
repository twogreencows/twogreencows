#!/usr/bin/env python3

import requests
import pprint as pp
import os
from os import environ
import sys
import pytest
import datetime
import base64

#from simplejson import JSONDecodeError


#@pytest.mark.skipif(not TOKEN_RETRIEVED, reason="Couldn't retrieved the bearer token")
context = {}
core_url = "http://localhost:3000/api/V1"
h = {"Content-Type":"application/json"}
context["user_uuid"]=""


def test_v1_users_getall_plain(endpoint="/users", context=context) -> None:
    pp.pprint("== Test GET all users")
    r= requests.get( core_url+ endpoint )
    if r.status_code != 200:
        pp.pprint("  ->Test FAILED  " + str(r.status_code)+ "\n")
        pp.pprint(r.content)
    else:
        pp.pprint("  ->Test SUCCEDED")
        pp.pprint(r.json())
    assert r.status_code == 200, f'Received wrong status code {r.status_code} instead of 200'
    all_users =  r.json()["data"]
    assert type(all_users).__name__ in ('list', 'tuple'), f'Received data for all users is not an array'
    for a_user in all_users:
        assert a_user["uuid"].startswith("usr") == True , f'Received an object which is not a user UUID' 



def test_v1_users_getall_withtokens(endpoint="/users", context=context) -> None:
    pp.pprint("== Test GET all users")
    r= requests.get( core_url+ endpoint+"?withSubObjects=tokens" )
    if r.status_code != 200:
        pp.pprint("  ->Test FAILED  " + str(r.status_code)+ "\n")
        pp.pprint(r.content)
    else:
        pp.pprint("  ->Test SUCCEDED")
        pp.pprint(r.json())
    assert r.status_code == 200, f'Received wrong status code {r.status_code} instead of 200'
    all_users =  r.json()["data"]
    assert type(all_users).__name__ in ('list', 'tuple'), f'Received data for all users is not an array'
    for a_user in all_users:
        assert a_user["uuid"].startswith("usr") == True , f'Received an object which is not a user UUID' 
        user_tokens = a_user.get("tokens", None)
        assert user_tokens != None , f'Received data does not contain tokens array'
        assert type(user_tokens).__name__ in ('list', 'tuple'), f'Received tokens is not a proper array type'

        for a_token in user_tokens:
            assert a_token["uuid"].startswith("tok") == True , f'Received a subobject object which is not a token UUID' 



def test_v1_users_postone_missingparams(endpoint="/users", context=context) -> None:
    pp.pprint(" == Test POST one user - missing parameters")
    
    r = requests.post(core_url+"/users",  headers=h , json={"display_name":"paul", "password":"yesterday","phone_number":"+442012346789"}) 
    if r.status_code != 400:
        pp.pprint("  ->Test FAILED")
        pp.pprint(r.json())
        pp.pprint(r.status_code)
    else:
        pp.pprint(r.json())

    assert r.status_code == 400, f'Received wrong status code {r.status_code} instead of 400' 
    err_uuid = r.json()["data"]["uuid"]
    assert err_uuid.startswith("err") == True, f'Received an object which is not a error UUID' 



def test_v1_users_postone_unmatchparams(endpoint="/users", context=context) -> None:
    pp.pprint( "== Test POST one user - unmatched parameters")
    r=requests.post(core_url+"/users", headers=h , json={"display_name":"paul", "password":"yesterday","confirm_password":"yerblues","phone_number":"+442012346789"}) 
    if r.status_code != 400:
        pp.pprint("  ->Test FAILED")
        pp.pprint(r.json())
    else:
        pp.pprint(r.json())

    assert r.status_code == 400, f'Received wrong status code {r.status_code} instead of 400' 
    err_uuid = r.json()["data"]["uuid"]
    assert err_uuid.startswith("err") == True, f'Received an object which is not a error UUID' 



def test_v1_users_postone_tooshortpassword(endpoint="/users", context=context) -> None:
    pp.pprint(" == Test POST one user - short password")
    
    r = requests.post(core_url+"/users",  headers=h , json={"display_name":"paul", "password":"rocky","confirm_password":"rocky", "phone_number":"+442012346789"}) 
    if r.status_code != 400:
        pp.pprint("  ->Test FAILED")
        pp.pprint(r.json())
        pp.pprint(r.status_code)
    else:
        pp.pprint(r.json())

    assert r.status_code == 400, f'Received wrong status code {r.status_code} instead of 400' 
    err_uuid = r.json()["data"]["uuid"]
    assert err_uuid.startswith("err") == True, f'Received an object which is not a error UUID' 



def test_v1_users_postone_plain_allparams(endpoint="/users", context=context) -> None:
    pp.pprint("== Test POST one user - good parameters")
    r=requests.post(core_url + endpoint, headers = h, json= {"display_name":"paul", "password":"yesterday","confirm_password":"yesterday","email":"paul@fabfour.co.uk" ,"phone_number":"+442012346789"})
    if r.status_code != 201 and  r.status_code != 200:
        pp.pprint("  ->Test FAILED  " + str(r.status_code)+ "\n")
        pp.pprint(r.content)
    else:
        pp.pprint("  ->Test SUCCEEDED")
        pp.pprint(r.json())
        context["user_uuid"] = r.json()["data"]["uuid"]
        pp.pprint(context)

    assert r.status_code == 200 or r.status_code == 201, f'Received wrong status code {r.status_code} instead of 200 or 201' 
    usr_uuid = r.json()["data"]["uuid"]
    assert usr_uuid.startswith("usr") == True, f'Received an object which is not a user UUID' 



def test_v1_users_postone_plain_twoparams(endpoint="/users", context=context) -> None:

    pp.pprint("== Test POST one user - good parameters but some missing")
    r=requests.post(core_url + endpoint, headers = h, json= {"display_name":"george", "password":"something","confirm_password":"something","email":"george@fabfour.co.uk" })
    if r.status_code != 201 and  r.status_code != 200:
        pp.pprint("  ->Test FAILED  " + str(r.status_code)+ "\n")
        pp.pprint(r.content)
    else:
        pp.pprint("  ->Test SUCCEEDED")
        pp.pprint(r.json())
        context["user_uuid"] = r.json()["data"]["uuid"]
        pp.pprint(context)

    assert r.status_code == 200 or r.status_code == 201, f'Received wrong status code {r.status_code} instead of 200 or 201' 
    usr_uuid = r.json()["data"]["uuid"]
    assert usr_uuid.startswith("usr") == True, f'Received an object which is not a user UUID' 
    phone_number = r.json()["data"].get("phone_number")
    assert phone_number == None, f'Received an object which has a phone number when it should not' 



def test_v1_users_postone_plain_nousernameparams(endpoint="/users", context=context) -> None:

    pp.pprint("== Test POST one user - good parameters but username missing")
    r=requests.post(core_url + endpoint, headers = h, json= { "password":"submarine","confirm_password":"submarine","email":"ringo@fabfour.co.uk" })
    if r.status_code != 201 and  r.status_code != 200:
        pp.pprint("  ->Test FAILED  " + str(r.status_code)+ "\n")
        pp.pprint(r.content)
    else:
        pp.pprint("  ->Test SUCCEEDED")
        pp.pprint(r.json())
        context["user_uuid"] = r.json()["data"]["uuid"]
        pp.pprint(context)

    assert r.status_code == 200 or r.status_code == 201, f'Received wrong status code {r.status_code} instead of 200 or 201' 
    usr_uuid = r.json()["data"]["uuid"]
    assert usr_uuid.startswith("usr") == True, f'Received an object which is not a user UUID' 



def test_v1_users_postone_withtokens_allparams(endpoint="/users", context=context) -> None:
    pp.pprint("== Test POST one user - good parameters with tokens")
    r= requests.post(core_url+ "/users?withSubObjects=tokens", headers = h, json={"display_name":"john", "password":"yerblues","confirm_password":"yerblues","email":"john@fabfour.co.uk", "phone_number":"+442098764321"})
    
    if r.status_code != 201 and r.status_code != 200:
        pp.pprint("  ->Test FAILED  " + str(r.status_code)+ "\n")
        pp.pprint(r.content)
    else:
        pp.pprint("  ->Test SUCCEEDED")
        pp.pprint(r.json())

    assert r.status_code == 200 or r.status_code == 201, f'Received wrong status code {r.status_code} instead of 200 or 201' 
    usr_uuid = r.json()["data"]["uuid"]
    assert usr_uuid.startswith("usr") == True, f'Received an object which is not a user UUID' 
    user_tokens = (r.json()["data"]).get("tokens", None)
    assert user_tokens != None , f'Received data does not contain tokens array'
    assert type(user_tokens).__name__ in ('list', 'tuple'), f'Received tokens is not a proper array type'

    for a_token in user_tokens:
        assert a_token["uuid"].startswith("tok") == True , f'Received a subobject object which is not a token UUID' 
  


def test_v1_users_postone_conflictonexisting(endpoint="/users", context=context) -> None:
    pp.pprint("== Test POST one user - conflicting passwords")
    r=requests.post(core_url+endpoint, headers = h, json= {"display_name":"paul", "password":"sergeantpepper","confirm_password":"sergeantpepper","phone_number":"+442012346789"})
    
    if r.status_code != 409:
        pp.pprint("  ->Test FAILED  " + str(r.status_code)+ "\n")
        pp.pprint(r.content)
    else:
        pp.pprint("  ->Test SUCCEEDED")
        pp.pprint(r.json())

    assert r.status_code == 409, f'Received wrong status code {r.status_code} instead of 409' 
    err_uuid = r.json()["data"]["uuid"]
    assert err_uuid.startswith("err") == True, f'Received an object which is not a error UUID' 



def test_v1_users_getone_nonexisting(endpoint="/users", context=context) -> None:
    pp.pprint("Test GET one user: not existing")
    r=requests.get(core_url+ "/users/usr-ewklewlkewkwekelwk222kl")
    if r.status_code != 404:
        pp.pprint("  ->Test FAILED")
        pp.pprint("  ->Test FAILED  " + str(r.status_code)+ "\n")
        pp.pprint(r.content)
    else:
        pp.pprint("  ->Test SUCCEEDED")
        pp.pprint(r.json())
    
    assert r.status_code == 404, f'Received wrong status code {r.status_code} instead of 404' 
    err_uuid = r.json()["data"]["uuid"]
    assert err_uuid.startswith("err") == True, f'Received an object which is not a error UUID' 



def test_v1_users_getone_existing_plain(endpoint="/users", context=context) -> None:
    pp.pprint("== Test GET one user simple version " + context["user_uuid"])
    pp.pprint(context)
    r=requests.get(core_url+ "/users/"+context["user_uuid"])
    if r.status_code != 200:
        pp.pprint("  ->Test FAILED  " + str(r.status_code)+ "\n")
        pp.pprint(r.content)
    else:
        pp.pprint("  ->Test SUCCEEDED")
        pp.pprint(r.json())
    assert r.status_code == 200, f'Received wrong status code {r.status_code} instead of 200' 
    usr_uuid = r.json()["data"]["uuid"]
    assert usr_uuid.startswith("usr") == True, f'Received an object which is not a error UUID' 
 


def test_v1_users_getone_existing_wihtokens(endpoint="/users") -> None:
    pp.pprint("== Test GET one user with their tokens")
    r=requests.get(core_url+ "/users/"+context["user_uuid"]+"?withSubObjects=tokens")
    if r.status_code != 200:
        pp.pprint("  ->Test FAILED  " + str(r.status_code)+ "\n")
        pp.pprint(r.content)
    else:
        pp.pprint("  ->Test SUCCEEDED")
        pp.pprint(r.json())
    assert r.status_code == 200, f'Received wrong status code {r.status_code} instead of 200' 
    
    user_tokens = (r.json()["data"]).get("tokens", None)
    assert user_tokens != None , f'Received data does not contain tokens array'
    assert type(user_tokens).__name__ in ('list', 'tuple'), f'Received tokens is not a proper array type'

    for a_token in user_tokens:
        assert a_token["uuid"].startswith("tok") == True , f'Received a subobject object which is not a token UUID' 



def test_v1_users_getonetokens_existing(endpoint="/users") -> None:
    pp.pprint("== Test GET tokens for a existing user - " + context["user_uuid"])
    r=requests.get(core_url+ "/users/"+context["user_uuid"]+"/tokens")
    if r.status_code != 200:
        pp.pprint("  ->Test FAILED  " + str(r.status_code)+ "\n")
        pp.pprint(r.content)
    else:
        pp.pprint("  ->Test SUCCEEDED")
        pp.pprint(r.json())
    assert r.status_code == 200, f'Received wrong status code {r.status_code} instead of 200' 
    all_tokens =  r.json()["data"]
    assert type(all_tokens).__name__ in ('list', 'tuple'), f'Received data for all tokens for a users is not an array'
    for a_token in all_tokens:
        assert a_token["uuid"].startswith("tok") == True , f'Received an object which is not a token UUID' 


def test_v1_users_getonetokens_nonexistinguser(endpoint="/users") -> None:
    pp.pprint("== Test GET tokens for a non existing user")
    r=requests.get(core_url+ "/users/"+"usr-lalalalala/tokens")
    if r.status_code != 404:
        pp.pprint("  ->Test FAILED  " + str(r.status_code)+ "\n")
        pp.pprint(r.content)
    else:
        pp.pprint("  ->Test SUCCEEDED")
        pp.pprint(r.json())
    
    assert r.status_code == 404, f'Received wrong status code {r.status_code} instead of 404' 
    err_uuid = r.json()["data"]["uuid"]
    assert err_uuid.startswith("err") == True, f'Received an object which is not a error UUID' 



def test_v1_users_addonetoken_existinguser(endpoint="/users") -> None:
    pp.pprint("== POST new token on user")
    r=requests.post(core_url+ "/users/"+context["user_uuid"]+"/tokens")
    if r.status_code != 201:
        pp.pprint("  ->Test FAILED  " + str(r.status_code)+ "\n")
        pp.pprint(r.content)
    else:
        pp.pprint("  ->Test SUCCEEDED")
        pp.pprint(r.json())
    assert  r.status_code == 201, f'Received wrong status code {r.status_code} or 201' 
    tok_uuid = r.json()["data"]["uuid"]
    context["tok_uuid"] = tok_uuid
    pp.pprint(context)
    assert tok_uuid.startswith("tok") == True, f'Received an object which is not a error UUID' 



def test_v1_users_addonetoken_nonexistinguser(endpoint="/users") -> None:
    pp.pprint("== POST new token on non existing user")
    r=requests.post(core_url+ "/users/"+"usr-lalalalala/tokens")
    if r.status_code != 404:
        pp.pprint("  ->Test FAILED  " + str(r.status_code)+ "\n")
        pp.pprint(r.content)
    else:
        pp.pprint("  ->Test SUCCEEDED")
        pp.pprint(r.json())
    
    assert r.status_code == 404, f'Received wrong status code {r.status_code} instead of 404' 
    err_uuid = r.json()["data"]["uuid"]
    assert err_uuid.startswith("err") == True, f'Received an object which is not a error UUID' 



def test_v1_users_deleteonetoken_nonexistinguser(endpoint="/users", context=context) -> None:
    pp.pprint("== Test DELETE token for a non existing user")
    pp.pprint(context)
    r=requests.delete(core_url+ "/users/u-aaaaaaaaaaaaaaaa/tokens/"+context["tok_uuid"])
    if r.status_code != 404:
        pp.pprint("  ->Test FAILED  " + str(r.status_code)+ "\n")
        pp.pprint(r.content)
    else:
        pp.pprint("  ->Test SUCCEEDED")
        pp.pprint(r.json())
    
    assert r.status_code == 404, f'Received wrong status code {r.status_code} instead of 404' 
    err_uuid = r.json()["data"]["uuid"]
    assert err_uuid.startswith("err") == True, f'Received an object which is not a error UUID' 



def test_v1_users_deleteonenonexistingtoken_existinguser(endpoint="/users") -> None:
    pp.pprint("Test DELETE one non existing token of an existing user")
    r=requests.delete(core_url+ "/users/"+context["user_uuid"]+"/tokens/tok-989898989")
    if r.status_code != 404:
        pp.pprint("  ->Test FAILED  " + str(r.status_code)+ "\n")
        pp.pprint(r.content)
    else:
        pp.pprint("  ->Test SUCCEEDED")
        pp.pprint(r.json())

    assert r.status_code == 404, f'Received wrong status code {r.status_code} instead of 404' 
    err_uuid = r.json()["data"]["uuid"]
    assert err_uuid.startswith("err") == True, f'Received an object which is not a err UUID' 



@pytest.mark.skip(reason="no way of currently testing this")
def test_v1_users_deleteonetoken_existinguser(endpoint="/users") -> None:
    pp.pprint("Test DELETE one token of an existing user")

    #we send back 200 and not 204 because 204 allows no body content and we want to send back the deleted user
    r=requests.delete(core_url+ "/users/"+context["user_uuid"]+"/tokens/"+context["tok_uuid"])
    if r.status_code != 200:
        pp.pprint("  ->Test FAILED  " + str(r.status_code)+ "\n")
        pp.pprint(r.content)
    else:
        pp.pprint("  ->Test SUCCEEDED")
        pp.pprint(r.json())

    assert r.status_code == 200, f'Received wrong status code {r.status_code} instead of 200' 
    tok_uuid = r.json()["data"]["uuid"]
    assert tok_uuid.startswith("tok") == True, f'Received an object which is not a token UUID' 



def test_v1_users_deleteone_nonexisting(endpoint="/users", context=context) -> None:
    pp.pprint("== Test DELETE one user: non existing")
    r=requests.delete(core_url+ "/users/usr-lalalalala")
    if r.status_code != 404:
        pp.pprint("  ->Test FAILED  " + str(r.status_code)+ "\n")
        pp.pprint(r.content)
    else:
        pp.pprint(r.json())
        pp.pprint("  ->Test SUCCEEDED")
    
    assert r.status_code == 404, f'Received wrong status code {r.status_code} instead of 404' 
    err_uuid = r.json()["data"]["uuid"]
    assert err_uuid.startswith("err") == True, f'Received an object which is not a error UUID' 



@pytest.mark.skip(reason="no way of currently testing this")
def test_v1_users_deleteone_existing(endpoint="/users", context=context) -> None:
    pp.pprint("== Test DELETE one user: existing")
    r=requests.delete(core_url+ "/users/"+context["user_uuid"])
    if r.status_code != 200:
        pp.pprint("  ->Test FAILED  " + str(r.status_code)+ "\n")
        pp.pprint(r.content)
    else:
        pp.pprint(r.json())
        pp.pprint("  ->Test SUCCEEDED")
    
    assert r.status_code == 200, f'Received wrong status code {r.status_code} instead of 200' 
    usr_uuid = r.json()["data"]["uuid"]
    assert usr_uuid.startswith("usr") == True, f'Received an object which is not a user UUID' 





