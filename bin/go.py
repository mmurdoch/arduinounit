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

    def _serial(self):
        dev=serial.Serial(
            port=self.config.port(),
            baudrate=self.config.baud(),
            bytesize=serial.EIGHTBITS,
            parity=serial.PARITY_NONE,
            stopbits=serial.STOPBITS_ONE,
            timeout=2.00,
            xonxoff=0,
            rtscts=0,
            interCharTimeout=0.100)
        try:
            buf = ''

            while True:
                buf += dev.read(dev.inWaiting())
                if (buf.find('\n') == -1) and (not buf.endswith('command> ')):
                    buf += dev.read(65535)
                eol = buf.find('\n')
                if eol == -1:
                    eol = len(buf)-1
                out = buf[0:eol+1]
                buf = buf[eol+1:]
                inp=(yield out)
                if inp != None:
                    dev.write(inp)
        finally:
            dev.close()

    def run(self,input=sys.stdin,output=sys.stdout):
        commands = re.compile('^command> (.*)$')
        command = ''
        dev = self._serial()

        while command != 'run':
            line=input.readline()
            line=line.rstrip()
            result=commands.match(line)
            if result:
                command = result.group(1)
                while 1:
                    line=dev.next()
                    if line == '':
                        continue
                    if line == 'command> ': 
                        break
                    line=line.rstrip()                    
                    print >>output,line
                dev.send(command+'\n')
                print >>output,'command> ' + command

        ends = re.compile('^Test summary')

        while 1:
            while 1:
                line=dev.next()
                if line != '':
                    break
            line = line.rstrip()
            print >>output, line
            if ends.match(line):
                break

        dev.close()

    def test(self,input=sys.stdin,output=sys.stdout):
        count = 0
        ok = 1

        commands = re.compile('^command> (.*)$')
        command = ''
        dev = self._serial()

        while command != 'run':
            expected=input.readline()
            expected=expected.rstrip()
            count=count+1
            result=commands.match(expected)
            if result:
                command = result.group(1)
                while dev.next() != 'command> ':
                    pass
                dev.send(command+'\n')

        ends = re.compile('^Test summary')

        while 1:
            expected=input.readline()
            expected=expected.rstrip()
            count=count+1
            while 1:
                line=dev.next()
                if line != '':
                    break
            line=line.rstrip()
            if (expected != line):
                print >>output, 'Expected "' + expected + '" on line ' + str(count)
                print >>output, '     got "' + line     + '" instead.'
                ok = 0
            if ends.match(line):
                break

        dev.close()

        if ok:
            print >>output, 'Meta test passed.'
        else:
            print >>output, 'Meta test failed.'
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

        print 'Meta test summary: ' + str(ok) + ' passed and ' + \
            str(bad) + ' failed, out of ' + str(ok+bad) + ' test(s).'

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
