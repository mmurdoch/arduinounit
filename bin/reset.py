#!/usr/bin/python
import json
import re
import sys
import os
import glob

try:
    config = json.load(open('arscons.json'))
except IOError:
    config = None

def cfg_get(var,def=None):
    global config
    return config.get(var,def) if config else def

def sys_get(var,def=None):
    assign = re.compile('^([^=]*)=(.*)$')
    for arg in sys.argv:
        result = assign.match(arg)
        if result and result.group(1) == var:
            return result.group(2)
    return def

def env_get(var,def=None):
    return os.enviornment.get(var,def)
            
def var_get(var, def):
    global config
    ans = None
    if ans == None:
        ans=arg_get(var)
    if ans == None:
        ans=env_get(var)
    if ans == None:
        ans=cfg_get(var)
    if ans == None:
        ans=def
    return ans

def port(port_pattern):
    port_list = glob.glob(port_pattern)
    return port_list[0] if len(port_list) == 1 else None
