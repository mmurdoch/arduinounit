#!/usr/bin/env python

import sys
import pexpect
import re
import time

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
