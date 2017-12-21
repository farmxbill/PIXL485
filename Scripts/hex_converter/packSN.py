#!/usr/bin/env python

import sys

def twoDigitHex (number):
        return '%02x' % number

if ((len(sys.argv) != 2) or (len(sys.argv[1]) != 10)):
    if (len(sys.argv) != 2):
        print "Invalid number of command line arguments.  Should be:"
        print "python packSN.py <serial number string>"
    else:
        print "serial number should be 10 character ascii string"
else:
    sn_string = sys.argv[1]
    pack = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
    i = 0
    while (i < 10):
        print sn_string[i], (ord(sn_string[i])) % 64
        pack[i] = ((ord(sn_string[i])) % 64) * 2**((i%5)*6)
        i += 1
    addr = ('%08X' % (pack[4] + pack[3] + pack[2] + pack[1] + pack[0])) + ('%08X' % (pack[9] + pack[8] + pack[7] + pack[6] + pack[5]))
    print addr

    byte7 = int(addr[0:2], 16)
    print '%02X' % byte7
    byte6 = int(addr[2:4], 16)
    print '%02X' % byte6
    byte5 = int(addr[4:6], 16)
    print '%02X' % byte5
    byte4 = int(addr[6:8], 16)
    print '%02X' % byte4
    byte3 = int(addr[8:10], 16)
    print '%02X' % byte3
    byte2 = int(addr[10:12], 16)
    print '%02X' % byte2
    byte1 = int(addr[12:14], 16)
    print '%02X' % byte1
    byte0 = int(addr[14:16], 16)
    print '%02X' % byte0

    