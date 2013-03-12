import config

class Go:
    def __init__(self, config):
        self.config = config
 
    def reset(self):
        import serial
        import time

        dev = serial.Serial(self.config.port())
        dev.setDTR(1)
        time.sleep(0.250)
        dev.setDTR(0)
        dev.close()

    def monitor(self):
        import serial.tools.miniterm

        console=serial.tools.miniterm.Miniterm(
            port=self.config.port(),
            baudrate=self.config.baud(),
            parity=serial.PARITY_NONE, 
            rtscts=False,
            xonxoff=False)

        console.join(True)
        console.start()
        console.join()

    def scons(self,target=None):
        import sys
        import subprocess

        args=self.config.find("scons.py")
        if args != None:
            args = [sys.executable,args]
        else:
            args = ["scons"]

        args.append("-C")
        args.append("firmware")

        if target:
            args.append(target)

        print "args: " + str(args)
        proc = subprocess.Popen(args)
        proc.communicate()

    def compile(self):
        self.scons()

    def upload(self):
        self.scons('upload')

    def clean(self):
        self.scons('clean')

    def expect(self):
        if sys.platform == 'win32':
            import winpexpect
            import sys
            px = winpexpect.winspawn(
                '"' + sys.executable + '"' + ' ' + 
                '"' + sys.argv[0] + '"' + 
                ' ' + 'monitor')
        else:
            import pexpect
            import sys
            px = pexpect.spawn(
                '"' + sys.executable + '"' + ' ' + 
                '"' + sys.argv[0] + '"' + 
                ' ' + 'monitor')
        return px

    def _scrape(self):
        px=self.expect()

        commands = re.compile('^command> (.*)$')

        while 1:
            line=sys.stdin.readline()
            if not line:
                break
            line=line.rstrip()
            yield line

            result=commands.match(line)
            if result:
                command = result.group(1)
                px.expect('command> ')
                px.sendline(command)
                if command == 'run':
                    break

        px.readline()
        ends = re.compile('^test summary')

        while not px.eof():
            line=px.readline()
            line = line.rstrip()
            yield line
            if ends.match(line):
                break

    def run(self):
        for line in self._scrape():
            print line

    def test(self):
        commands = re.compile('^command> (.*)$')
        running = 0
        line = 0

        for line in self._scrape():
            line=line+1
            if not running:
                result=commands.match(line)
                if result and result.group(1) == 'run':
                    running = 1
            else:
                expected=sys.stdin.readline()
                expected=expected.rstrip()
                if (expected != line):
                    print 'expected "' + expected + '" on line ' + str(count)
                    print '     got "' + line     + '" instead'
                    raise AssertionError
        return true

def main():
    import sys
    import os

    os.chdir(os.path.dirname(sys.argv[0]))
    os.chdir("..")

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
        if arg == 'compile':
            go.compile()
        if arg == 'upload':
            go.upload()
        if arg == 'clean':
            go.clean()
        if arg == 'port?':
            print cfg.port()
        if arg == 'baud?':
            print cfg.baud()
        if arg == 'board?':
            print cfg.board()

if __name__ == '__main__':
    main()
