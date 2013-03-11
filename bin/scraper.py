#!/usr/bin/env python

import sys
import re
import time


if sys.platform == 'win32':
  import json
  import winpexpect
  winconfig = json.load(open('./firmware/arscons.json'))
  px = winpexpect.winspawn('"' + winconfig.get('GIT_BASH','bash')+'" ./bin/build reset monitor')
else:
  import pexpect
  px = pexpect.spawn('./bin/build reset monitor')

time.sleep(3.00)
px.timeout = 2

commands = re.compile('^command> (.*)$')
run = 0

while 1:
  line=sys.stdin.readline()
  if not line:
    break
  line=line.rstrip()
  result=commands.match(line)
  if result:
    command = result.group(1)
    px.expect('command> ')
    px.sendline(command)
    print 'command> ' + command
    if command == 'run':
      run = 1
      break

if not run:
  px.expect('command> ')
  px.sendline('run')
  print 'command> run'
  run = 1

px.readline()

ends = re.compile('^test summary')

while not px.eof():
  line=px.readline()
  line = line.rstrip()
  print line
  if ends.match(line):
    break
