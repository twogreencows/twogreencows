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


def test_v1_sessions_post_existinguser(endpoint="/sessions", context=context) -> None:
    json= {"user":{"display_name":"paul", "password":"yesterday","confirm_password":"yesterday"},
           "device": {"kind":"mobi"}}

    pp.pprint("== Test POST one session")
    r= requests.post( core_url+ endpoint )
    if r.status_code != 200 and r.status_code != 201:
        pp.pprint("  ->Test FAILED  " + str(r.status_code)+ "\n")
        pp.pprint(r.content)
    else:
        pp.pprint("  ->Test SUCCEDED")
        pp.pprint(r.json())
    assert r.status_code == 200, f'Received wrong status code {r.status_code} instead of 200 or 201'


def test_v1_sessions_post_existinguser_reusefull(endpoint="/sessions", context=context) -> None:

    json= {"user":{"display_name":"paul", "password":"yesterday","confirm_password":"yesterday"},
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


def test_v1_sessions_post_existinguser_newdevice(endpoint="/sessions", context=context) -> None:

    json= {"user":{"display_name":"paul", "password":"yesterday","confirm_password":"yesterday"},
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


def test_v1_sessions_post_newuser_newdevice(endpoint="/sessions", context=context) -> None:

    json= {"user":{"display_name":"brian", "password":"babyrichman","confirm_password":"babyrichman"},
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







