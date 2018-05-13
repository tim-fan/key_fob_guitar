#!/usr/bin/env python 

##############
## Script listens to serial port and writes contents to std out 
##############
## requires pySerial to be installed 
import serial
import argparse
import sys

parser = argparse.ArgumentParser(description='Cat serial port to std out')
parser.add_argument('--port', help='serial port to read from', default='/dev/ttyACM0')
parser.add_argument('--baud', help='serial port baud rate', default='9600')
args = parser.parse_args()

serial_port = args.port;
baud_rate = args.baud; 

with serial.Serial(serial_port, baud_rate) as ser:
    while True:
        sys.stdout.write(ser.read())
#    line = ser.readline();
#    line = line.decode("utf-8") #ser.readline returns a binary, convert to string
#    sys.stdout.write()
#    print(line);
