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

def test_v1_environment(endpoint="/environment", context=context) -> None:
    pp.pprint("== Test GET environment")
    r = requests.get(core_url+endpoint)
    if r.status_code != 200:
        pp.pprint("  ->Test FAILED  " + str(r.status_code)+ "\n")
        pp.pprint(r.content)
    else:
        pp.pprint("  ->Test SUCCEEDED")
        pp.pprint(r.json())
    assert r.status_code == 200, f'Received wrong status code {r.status_code} instead of 200' 
    env_uuid = r.json()["data"]["uuid"]
    assert env_uuid.startswith("env") == True, f'Received an object which is not a environment UUID' 






