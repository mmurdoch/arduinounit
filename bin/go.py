#!/usr/bin/env python

import os
import re
import sys
import glob
import time
import config
import serial
import subprocess

class Go:
    def __init__(self, config):
        self.config = config
 
    def reset(self):
        dev = serial.Serial(self.config.port())
        dev.setDTR(1)
        time.sleep(0.250)
        dev.setDTR(0)
        dev.close()

    def monitor(self):
        import serial.tools.miniterm
        print 'monitor: type ^] to exit...'
        console=serial.tools.miniterm.Miniterm(
            port=self.config.port(),
            baudrate=self.config.baud(),
            parity=serial.PARITY_NONE, 
            rtscts=False,
            xonxoff=False)

        console.start()
        console.join()

    def scons(self,target=None):
        args=self.config.find("scons.py")
        if args != None:
            args = [sys.executable,args]
        else:
            args = ["scons"]

        args.append("-C")
        args.append(os.path.join(os.path.dirname(__file__),'../firmware'))

        if target:
            args.append(target)

        proc = subprocess.Popen(args)
        proc.communicate()

    def compile(self):
        self.scons()

    def upload(self):
        self.scons('upload')

    def _expect(self):
        if sys.platform == 'win32':
            import winpexpect
            px = winpexpect.winspawn(
                '"' + sys.executable + '"' + ' ' + 
                '"' + sys.argv[0] + '"' + 
                ' ' + 'monitor')
        else:
            import pexpect
            px = pexpect.spawn(
                '"' + sys.executable + '"' + ' ' + 
                '"' + sys.argv[0] + '"' + 
                ' ' + 'monitor')
        return px

    def _scrape(self,input=sys.stdin,output=sys.stdout):
        px=self._expect()

        commands = re.compile('^command> (.*)$')
        command = ''
        while command != 'run':
            line=input.readline()
            if not line:
                break
            line=line.rstrip()
            result=commands.match(line)
            if result:
                command = result.group(1)
                px.expect('command> ')
                px.send(command+'\n')
            yield line

        px.readline()
        ends = re.compile('^test summary')

        while not px.eof():
            line=px.readline()
            line = line.rstrip()
            yield line
            if ends.match(line):
                break

    def run(self,input=sys.stdin,output=sys.stdout):
        px=self._expect()

        commands = re.compile('^command> (.*)$')
        command = ''
        while command != 'run':
            line=input.readline()
            if not line:
                break
            line=line.rstrip()
            result=commands.match(line)
            if result:
                command = result.group(1)
                px.expect('command> ')
                px.send(command+'\n')
            print >>output, line

        px.readline()
        ends = re.compile('^test summary')

        while not px.eof():
            line=px.readline()
            line = line.rstrip()
            print >>output, line
            if ends.match(line):
                break

    def test(self,input=sys.stdin,output=sys.stdout):
        commands = re.compile('^command> (.*)$')
        running = 0
        count = 0
        ok = 1

        for line in self._scrape(input,output):
            count=count+1
            if not running:
                result=commands.match(line)
                if result and result.group(1) == 'run':
                    running = 1
            else:
                expected=input.readline()
                expected=expected.rstrip()
                if (expected != line):
                    print >>output, 'expected "' + expected + '" on line ' + str(count)
                    print >>output, '     got "' + line     + '" instead'
                    ok = 0

        if ok:
            print >>output, 'meta test passed.'
        else:
            print >>output, 'meta test failed.'
        return ok

    def tests(self,pattern='$0/../../tests/*.in'):
        pattern=pattern.replace('$0/../',os.path.dirname(__file__)+os.path.sep)
        filenames = glob.glob(pattern)

        ok = 0
        bad = 0

        for filename in filenames:
            print 'testing ' + filename
            input=open(filename,'r')
            if self.test(input):
                ok = ok + 1
            else:
                bad = bad + 1
            input.close()
        print 'meta test summary: ' + str(ok) + ' passed and ' + \
            str(bad) + ' failed, out of ' + str(ok+bad) + ' test(s)'

def main():
    # in win32, HOME defaults to 'C:\Users\home\Roaming\AppData' !!
    if sys.platform == 'win32':
        os.environ['HOME']=os.environ['HOMEDRIVE'] + os.environ['HOMEPATH']

    cfg = config.Config()
    go = Go(cfg)

    for arg in sys.argv:
        if arg == 'reset':
            go.reset()
        if arg == 'monitor':
            go.monitor()
        if arg == 'run':
            go.run()
        if arg == 'test':
            go.test()
        if arg == 'tests':
            go.tests()
        if arg == 'compile':
            go.compile()
        if arg == 'upload':
            go.upload()
        if arg == 'port?':
            print cfg.port()
        if arg == 'baud?':
            print cfg.baud()
        if arg == 'board?':
            print cfg.board()

if __name__ == '__main__':
    main()
