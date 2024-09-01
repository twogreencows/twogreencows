#!/usr/bin/env python3

import requests
import pprint as pp
import os
from os import environ
import sys
import pytest
import datetime
import base64

context={}
core_url = "http://localhost:3000/api/V1"
h = {"Content-Type":"application/json"}


def test_v1_sessions_post_existinguser(endpoint="/sessions?mode=signin", context=context) -> None:
    json= {"user":{"display_name":"paul", "password":"yesterday2","confirm_password":"yesterday2", "email":"paul@fabfour.co.uk","phone_number":"+442012346789"},
           "device": {"kind":"web*", "display_name":"Safari", "platform":"apple"}}
  
    pp.pprint("== Test POST one session")
    r= requests.post( core_url+ endpoint , json=json)
    #r= requests.post( core_url+ endpoint)
    if r.status_code != 200 and r.status_code != 201:
        pp.pprint("  ->Test FAILED  " + str(r.status_code)+ "\n")
        pp.pprint(r.json())
    else:
        pp.pprint("  ->Test SUCCEDED")
        pp.pprint(r.json())
    assert r.status_code == 200 or r.status_code == 201, f'Received wrong status code {r.status_code} instead of 200 or 201'


@pytest.mark.skip(reason="no way of currently testing this")
def test_v1_sessions_post_existinguser_reusefull(endpoint="/sessions", context=context) -> None:

    json= {"user":{"display_name":"paul", "password":"yesterday2","confirm_password":"yesterday2"},
           "device": {"kind":"mobi", "display_name":"Brian's phone", "platform":"apple", "vendor_uuid" :"8A110F80-95E6-44A3-9FB7-D6A14BCE6734", "os_version":"15.2.3"}}

    pp.pprint("== Test POST one session")
    r= requests.post( core_url+ endpoint )
    if r.status_code != 200 and r.status_code != 201:
        pp.pprint("  ->Test FAILED  " + str(r.status_code)+ "\n")
        pp.pprint(r.content)
    else:
        pp.pprint("  ->Test SUCCEDED")
        pp.pprint(r.json())
    assert r.status_code == 200, f'Received wrong status code {r.status_code} instead of 200 or 201'


@pytest.mark.skip(reason="no way of currently testing this")
def test_v1_sessions_post_existinguser_newdevice(endpoint="/sessions", context=context) -> None:

    json= {"user":{"display_name":"paul", "password":"yesterday2","confirm_password":"yesterday2"},
           "device": {}}

    pp.pprint("== Test POST one session")
    r= requests.post( core_url+ endpoint )
    if r.status_code != 200 and r.status_code != 201:
        pp.pprint("  ->Test FAILED  " + str(r.status_code)+ "\n")
        pp.pprint(r.content)
    else:
        pp.pprint("  ->Test SUCCEDED")
        pp.pprint(r.json())
    assert r.status_code == 200, f'Received wrong status code {r.status_code} instead of 200 or 201'


def test_v1_sessions_post_newuser_newdevice(endpoint="/sessions?mode=signup", context=context) -> None:

    json= {"user":{"display_name":"brian", "password":"babyrichman5","confirm_password":"babyrichman5"},
           "device": {"kind":"mobi", "display_name":"Brian's phone", "platform":"apple", "vendor_uuid" :"8A110F80-95E6-44A3-9FB7-D6A14BCE6734", "os_version":"15.2.3"}}

    pp.pprint("== Test POST one session")
    r= requests.post( core_url+ endpoint, json=json )
    if r.status_code != 200 and r.status_code != 201:
        pp.pprint("  ->Test FAILED  " + str(r.status_code)+ "\n")
        pp.pprint(r.content)
    else:
        pp.pprint("  ->Test SUCCEDED")
        pp.pprint(r.json())
    assert r.status_code == 200 or r.status_code == 201, f'Received wrong status code {r.status_code} instead of 200 or 201'







