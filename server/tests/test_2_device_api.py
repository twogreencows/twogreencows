#!/usr/bin/env python3

import requests
import pprint as pp
import os
from os import environ
import sys
import pytest
import datetime
import base64
import uuid

#from simplejson import JSONDecodeError


#@pytest.mark.skipif(not TOKEN_RETRIEVED, reason="Couldn't retrieved the bearer token")
context = {}
core_url = "http://localhost:3000/api/V1"
h = {"Content-Type":"application/json"}
context["user_uuid"]=""


def test_v1_devices_getall_plain(endpoint="/devices", context=context) -> None:
    pp.pprint("== Test GET all devices")
    r= requests.get( core_url+ endpoint )
    if r.status_code != 200:
        pp.pprint("  ->Test FAILED  " + str(r.status_code)+ "\n")
        pp.pprint(r.content)
    else:
        pp.pprint("  ->Test SUCCEDED")
        pp.pprint(r.json())
    assert r.status_code == 200, f'Received wrong status code {r.status_code} instead of 200'
    all_devices =  r.json()["data"]
    assert type(all_devices).__name__ in ('list', 'tuple'), f'Received data for all devices is not an array'
    for a_device in all_devices:
        assert a_device["uuid"].startswith("dev") == True , f'Received an object which is not a deviceUUID' 



def test_v1_devices_getall_withtokens(endpoint="/devices", context=context) -> None:
    pp.pprint("== Test GET all device with their token")
    r= requests.get( core_url+ endpoint+"?withSubObjects=token" )
    if r.status_code != 200:
        pp.pprint("  ->Test FAILED  " + str(r.status_code)+ "\n")
        pp.pprint(r.content)
    else:
        pp.pprint("  ->Test SUCCEDED")
        pp.pprint(r.json())
    assert r.status_code == 200, f'Received wrong status code {r.status_code} instead of 200'
    all_devices =  r.json()["data"]
    assert type(all_devices).__name__ in ('list', 'tuple'), f'Received data for all devices is not an array'
    for a_user in all_devices:
        assert a_user["uuid"].startswith("dev") == True , f'Received an object which is not a deviceUUID' 
        device_token = a_user.get("token", None)
        assert device_token != None , f'Received data does not contain a token'
        assert device_token["uuid"].startswith("tok") == True , f'Received a subobject object which is not a token UUID' 



def test_v1_devices_postone_missingparams(endpoint="/devices", context=context) -> None:
    pp.pprint(" == Test POST one device- missing parameters")
    
    r = requests.post(core_url+"/devices",  headers=h , json={"display_name":"lolo's phone", "kind":"mobi", "platform":"apple"}) 
    if r.status_code != 400:
        pp.pprint("  ->Test FAILED")
        pp.pprint(r.json())
        pp.pprint(r.status_code)
    else:
        pp.pprint(r.json())

    assert r.status_code == 400, f'Received wrong status code {r.status_code} instead of 400' 
    err_uuid = r.json()["data"]["uuid"]
    assert err_uuid.startswith("err") == True, f'Received an object which is not a error UUID' 



def test_v1_devices_postone_defaultparams(endpoint="/users", context=context) -> None:
    pp.pprint( "== Test POST one device - unmatched parameters")
    r = requests.post(core_url+"/devices",  headers=h , json={"display_name":"lolo's phone", "kind":"mobi", "platform":"apple"}) 
    if r.status_code != 400:
        pp.pprint("  ->Test FAILED")
        pp.pprint(r.json())
    else:
        pp.pprint(r.json())

    assert r.status_code == 400, f'Received wrong status code {r.status_code} instead of 400' 
    err_uuid = r.json()["data"]["uuid"]
    assert err_uuid.startswith("err") == True, f'Received an object which is not a error UUID' 


def test_v1_devices_postone_plain_allparams(endpoint="/devices", context=context) -> None:

    r= requests.get( core_url+ "/users", headers=h )
    assert r.status_code == 200 or r.status_code == 201, f'Received wrong status code {r.status_code} for all users' 
    users = r.json()["data"]
    for aUser in users:
        if aUser["user_level"] == 0:
            context["user_uuid"] = aUser["uuid"]


    pp.pprint("== Test POST one device- good parameters")
    r = requests.post(core_url+"/devices",  headers=h , json={"display_name":"lolo's phone", "kind":"mobi", 
                                                              "platform":"apple", 
                                                              "os_version":"1.0.2", 
                                                              "owner_uuid":context["user_uuid"],
                                                              "vendor_uuid":str(uuid.uuid4()) } )
    if r.status_code != 201 and  r.status_code != 200:
        pp.pprint("  ->Test FAILED  " + str(r.status_code)+ "\n")
        pp.pprint(r.content)
    else:
        pp.pprint("  ->Test SUCCEEDED")
        pp.pprint(r.json())
        context["device_uuid"] = r.json()["data"]["uuid"]
        pp.pprint(context)

    assert r.status_code == 200 or r.status_code == 201, f'Received wrong status code {r.status_code} instead of 200 or 201' 
    usr_uuid = r.json()["data"]["uuid"]
    assert usr_uuid.startswith("dev") == True, f'Received an object which is not a deviceUUID' 



def test_v1_devices_getone_nonexisting(endpoint="/devices", context=context) -> None:
    pp.pprint("Test GET one user: not existing")
    r=requests.get(core_url+ "/devices/usr-ewklewlkewkwekelwk222kl")
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



def test_v1_devices_getone_existing_plain(endpoint="/users", context=context) -> None:
    pp.pprint("== Test GET one device simple version " + context["user_uuid"])
    pp.pprint(context)
    r=requests.get(core_url+ "/devices/"+context["device_uuid"])
    if r.status_code != 200:
        pp.pprint("  ->Test FAILED  " + str(r.status_code)+ "\n")
        pp.pprint(r.content)
    else:
        pp.pprint("  ->Test SUCCEEDED")
        pp.pprint(r.json())
    assert r.status_code == 200, f'Received wrong status code {r.status_code} instead of 200' 
    usr_uuid = r.json()["data"]["uuid"]
    assert usr_uuid.startswith("dev") == True, f'Received an object which is not a error UUID' 
 


def test_v1_users_getone_existing_wihtoken(endpoint="/devices") -> None:
    pp.pprint("== Test GET one device  with its token")
    r=requests.get(core_url+ "/devices/"+context["device_uuid"]+"?withSubObjects=tokens")
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


@pytest.mark.skip(reason="no way of currently testing this")
def test_v1_devices_deleteone_existing(endpoint="/devices", context=context) -> None:
    pp.pprint("== Test DELETE one device: existing")
    r=requests.delete(core_url+ "/devices/"+context["user_uuid"])
    if r.status_code != 200:
        pp.pprint("  ->Test FAILED  " + str(r.status_code)+ "\n")
        pp.pprint(r.content)
    else:
        pp.pprint(r.json())
        pp.pprint("  ->Test SUCCEEDED")
    
    assert r.status_code == 200, f'Received wrong status code {r.status_code} instead of 200' 
    usr_uuid = r.json()["data"]["uuid"]
    assert usr_uuid.startswith("usr") == True, f'Received an object which is not a user UUID' 





