#!/usr/bin/env python

import json
import os
import re
import sys
import glob

class Config:
    def __init__(self,file=None):
        if file == None:
          file = os.path.join(
            os.path.dirname(__file__),
            '../firmware/arscons.json')

        try:
            self.config = json.load(open(file))
        except IOError:
            self.config = None

    def _cfg_get(self,var):
        return self.config.get(var,None) if self.config else None

    def _sys_get(self,var):
        assign = re.compile('^([^=]*)=(.*)$')
        for arg in sys.argv:
            result = assign.match(arg)
            if result and result.group(1) == var:
                return result.group(2)
            return None

    def _env_get(self,var):
        return os.environ.get(var,None)

    def var(self,var,default_value=None):
        ans=None
        if ans == None:
            ans=self._sys_get(var)
        if ans == None:
            ans=self._env_get(var)
        if ans == None:
            ans=self._cfg_get(var)
        if ans == None:
            ans=default_value
        return ans

    def _default_port(self):
        if sys.platform == 'darwin':
            return '/dev/cu.usb*'
        if sys.platform == 'linux':
            return '/dev/ttyACM*'
        return None

    def port(self):
        ans = self.var('ARDUINO_PORT')
        if ans == None:
            ans = self._default_port()
        if ans != None:
            globbed_ans = glob.glob(ans)
            if globbed_ans and len(globbed_ans) == 1:
                ans = globbed_ans[0]
        return ans

    def board(self):
        return self.var('ARDUINO_BOARD','uno')

    def baud(self):
        return self.var('ARDUINO_BAUD',9600)

    def find(self,file):
#
#       sys.path does NOT work --- active state leaves out the path
#       we are looking for in windows (??!)
#
        sep = ';' if sys.platform == 'win32' else ':'
        path = os.environ['PATH'];
        paths = path.split(sep)

        for dirname in paths:
            candidate = os.path.join(dirname, file)
            if os.path.isfile(candidate):
                return candidate

        return None
