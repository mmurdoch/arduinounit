#!/usr/bin/perl

import serial
import re
import sys

class FirmwareIO:
  def __init__(self, port, baud):
    self.port = port
    self.baud = baud
    self.open()

  def open(self):
    self.io = serial.Serial(port=self.port,
                            baudrate=self.baud,
                            bytesize=serial.EIGHTBITS,
                            parity=serial.PARITY_NONE, 
                            stopbits=serial.STOPBITS_ONE, 
                            timeout=0.200, 
                            xonxoff=False, 
                            rtscts=False, 
                            writeTimeout=0.500, 
                            dsrdtr=False, 
                            interCharTimeout=0.100)
    self.eof = 0
    self.end_pattern = re.compile('/^test summary/')

  def close(self):
    self.io.close()
    self.eof = 1

  def line(self):
      if self.eof:
          return ''
      line = self.io.readline()
      if not line:
        self.eof = 1
        return ''
      line = line.rstrip()
      if self.end_pattern.match(line):
          self.eof = 1
      return line

  def find(self,pattern):
    lines = []
    matcher = re.compile(pattern)
    while not self.eof:
      line = self.line()
      if not self.eof:
        lines.append(line)
        if matcher.match(line):
          break
    return lines

  def reset(self):
    self.io.setDTR(1)
    self.io.sleep(0.250)
    self.io.setDTR(0)
    self.io.close()
    self.io.open()

  def clear(self):
    while self.io.read(1024):
      pass

  def command(self,cmd):
    self.find("/command>/");
    self.writelines(cmd+"\r\n")
    self.io.readline()

def main():
  io = FirmwareIO(sys.argv[0],sys.argv[1])
  input = open(sys.argv[2],"r")
  command_pattern = re.compile('/^command> (.*)$/')

  while 1:
    line = input.readline();
    if not line:
      break
    line = line.rtrim()
    print line + "\n"
    command_match = command_pattern.match(line)
    if command_match:
      cmd=command_match.group(1)
      scraper.command(cmd)
      if cmd == "run":
        break
  while not io.eof:
    line = io.line()
    if not line:
      break
    print line + "\n"

if __name__ == "__main__":
    main()
