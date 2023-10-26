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
context["device_uuid"]=""

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
        if context["device_uuid"] == "":
            context["device_uuid"] = a_device["uuid"]



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
    for a_device in all_devices:
        assert a_device["uuid"].startswith("dev") == True , f'Received an object which is not a deviceUUID' 
        device_token = a_device["token"]
        if device_token != None:
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


def test_v1_devices_postone_wrongparams(endpoint="/devices", context=context) -> None:
    pp.pprint(" == Test POST one device- wrong parameters")
    
    r = requests.post(core_url+"/devices",  headers=h , json={"display_name":"lolo's phone", "kind":"webp", "platform":"apple"}) 
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

    r = requests.get(core_url+ "/users", headers = h)
    if r.status_code == 200:
        all_users =  r.json()["data"]
        for a_user in all_users:
            pp.pprint(a_user)
            if a_user['display_name'] != 'admin':
                context["user_uuid"] = a_user["uuid"]
                break


    pp.pprint( "== Test POST one device - default parameters: mobile and web")
    device_info = {"display_name":"lolo's phone", "kind":"mobi", "os_version":"1.0.2", "platform":"apple", "vendor_uuid":"2FE5816E-CCE3-48DE-BEEB-212F691E2E1C", "owner_uuid":context["user_uuid"]}
    r = requests.post(core_url+"/devices",  headers=h , json=device_info) 
    if r.status_code != 201 and r.status_code != 200:
        pp.pprint("  ->Test FAILED")
        pp.pprint(r.json())
    else:
        pp.pprint(r.json())
   
    if context["device_uuid"] == "":
        context["device_uuid"] = r.json()["data"]["uuid"]


    assert r.status_code == 201 or r.status_code==200, f'Received wrong status code {r.status_code} instead of 201' 
    dev_uuid = r.json()["data"]["uuid"]
    assert dev_uuid.startswith("dev") == True, f'Received an object which is not a device UUID' 

    r = requests.post(core_url+"/devices",  headers=h , json={"display_name":"Safari", "kind":"web*", "platform":"apple", "owner_uuid":context["user_uuid"] }) 
    if r.status_code != 201 and r.status_code !=201:
        pp.pprint("  ->Test FAILED")
        pp.pprint(r.json())
    else:
        pp.pprint(r.json())

    assert r.status_code == 201 or r.status_code == 200, f'Received wrong status code {r.status_code} instead of 400' 
    dev_uuid = r.json()["data"]["uuid"]
    assert dev_uuid.startswith("dev") == True, f'Received an object which is not a device UUID' 



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



def test_v1_devices_getone_existing_plain(endpoint="/devices", context=context) -> None:
    pp.pprint("== Test GET one device simple version " + context["user_uuid"])
    r=requests.get(core_url+ "/devices/"+context["device_uuid"])
    if r.status_code != 200:
        pp.pprint("  ->Test FAILED  " + str(r.status_code)+ "\n")
        pp.pprint(r.content)
    else:
        pp.pprint("  ->Test SUCCEEDED")
        pp.pprint(r.json())
    assert r.status_code == 200, f'Received wrong status code {r.status_code} instead of 200' 
    device_uuid = r.json()["data"]["uuid"]
    assert device_uuid.startswith("dev") == True, f'Received an object which is not a device UUID' 
 

def test_v1_devices_posttone_token (endpoint="/devices") -> None:
    pass


def test_v1_devices_getone_existing_wihtoken(endpoint="/devices") -> None:
    pp.pprint("== Test GET one device  with its token")
    r=requests.get(core_url+ "/devices/"+context["device_uuid"]+"?withSubObjects=token")
    if r.status_code != 200:
        pp.pprint("  ->Test FAILED  " + str(r.status_code)+ "\n")
        pp.pprint(r.content)
    else:
        pp.pprint("  ->Test SUCCEEDED")
        pp.pprint(r.json())
    assert r.status_code == 200, f'Received wrong status code {r.status_code} instead of 200' 
    device_uuid = r.json()["data"]["uuid"]
    assert device_uuid.startswith("dev") == True, f'Received an object which is not a device UUID' 
 
    d_token = r.json()["data"]["token"] 
    if d_token != None:
        assert d_token["uuid"].startswith("tok"), f'Received an object which is not a token UUID' 


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





