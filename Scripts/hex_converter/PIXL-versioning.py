#!/usr/bin/env python

# reads a hex file for the DAFE sensor, and creates multiple hex files 
# with patched sensor types for programming
#
# the default without change is a sensor type of 0x0, but this tool enables
# creating hex files that show the sensor types:
#   0x01: Water Pressure Sensor
#   0x02: Tree Diameter (Dendrometer) Sensor
#   0x03: Infrared Radiometer Sensor
#   0x04: Bud/Leaf Frost Sensor
#
# Read in the input file from the command line, then patch the sensor type
# into the hex file, insuring the checksum is correct as well
#
# To invoke: python sensor-eeprom-patcher <input-file> <output-file> <sensor-type>
#
# this works by opening the input file for reading.  It reads the file through to
# find the line that has the address of the EEPROM for the PIC 18LF14K22, which is
# :0200000400F00A.  
# 
# When this line is found, we know it's a valid file, and we rewind - and then we
# open a file for writing.  We read a line, write a line, read a line write a line
# until we find the address line for the EEPROM: then we patch it, and keep going.
#
# The line afterwards has the content of the EEPROM that we want
# to patch, and looks like this:  :080000000100010000010C00E9.  We take that line,
# and change the next to last byte to the input sensor type from the command line,
# and re-write the line with a new sensor type, and the correct checksum.
# we then continue reading and writing the file until the input file is cloned with
# the patched value for sensor type.

import sys

if ((len(sys.argv) != 4) or (len(sys.argv[3]) != 10)):
    if (len(sys.argv) != 4):
        print "Invalid number of command line arguments.  Should be:"
        print "python PIXL-versioning.py <input.hex> <output.hex> <version_string>"
    else:
        print "serial number should be 10 character ascii string"
else:
    version_string = sys.argv[3]
    inhex = open(sys.argv[1], 'r')
    line_found = False
    eeprom_data_line = False
    for line in inhex:
        if (eeprom_data_line == True):
            sw_ver = line[13:19]
            hw_ver = line[9:13]
            eeprom_data_line = False
        if (line == ":0200000400F00A\n"):
            line_found = True
            eeprom_data_line = True

    line_no = 1
    if (line_found == False):
        print "\tEEPROM address not found in the input file"
    else:
        inhex.seek(0)
        output_filename = sys.argv[2]+'_HW'+hw_ver+'_SW'+sw_ver+'.hex'
        outhex = open(output_filename, 'w')
        eeprom_data_line = False
        for line in inhex:
            if (eeprom_data_line == True):
                if (line_no == 1):
                    print "\tVersioning PIXL sensor to version", version_string, "and writing to output file", output_filename
                    sys.stdout.write (line)                
                    byte0 = int(line[1:3], 16)
                    byte1 = int(line[3:5], 16)
                    byte2 = int(line[5:7], 16)
                    byte3 = int(line[7:9], 16)
                    byte4 = int(line[9:11], 16)
                    byte5 = int(line[11:13], 16)
                    byte6 = int(line[13:15], 16)
                    byte7 = int(line[15:17], 16)
                    byte8 = int(line[17:19], 16)
                    byte9 = int(line[19:21], 16)
                    byte10 = int(line[21:23], 16)
                    byte11 = int(line[23:25], 16)
#                    byte12 = int(line[25:27], 16)
#                    byte13 = int(line[27:29], 16)
#                    byte14 = int(line[29:31], 16)
#                    byte15 = int(line[31:33], 16)
#                    byte16 = int(line[33:35], 16)
#                    byte17 = int(line[35:37], 16)
#                    byte18 = int(line[37:39], 16)
#                    byte19 = int(line[39:41], 16)
                    byte12 = ord(version_string[0])
                    byte13 = ord(version_string[1])
                    byte14 = ord(version_string[2])
                    byte15 = ord(version_string[3])
                    byte16 = ord(version_string[4])
                    byte17 = ord(version_string[5])
                    byte18 = ord(version_string[6])
                    byte19 = ord(version_string[7])
                    checksum = (0-(byte0+byte1+byte2+byte3+byte4+byte5+byte6+byte7+byte8+byte9+byte10+byte11+byte12+byte13+byte14+byte15+byte16+byte17+byte18+byte19)) & 0xFF
                    line = line[:25]
                    line += '%02X' % byte12
                    line += '%02X' % byte13
                    line += '%02X' % byte14
                    line += '%02X' % byte15
                    line += '%02X' % byte16
                    line += '%02X' % byte17
                    line += '%02X' % byte18
                    line += '%02X' % byte19
                    line += '%02X' % checksum
                    line += '\n'
                    print line
                    outhex.write(line)
                    line_no += 1
                else:
                    sys.stdout.write (line)
                    byte0 = int(line[1:3], 16)
                    byte1 = int(line[3:5], 16)
                    byte2 = int(line[5:7], 16)
                    byte3 = int(line[7:9], 16)
#                    byte4 = int(line[9:11], 16)
#                    byte5 = int(line[11:13], 16)
#
# serial number from command line
                    byte4 = ord(version_string[8])
                    byte5 = ord(version_string[9])
#
                    byte6 = int(line[13:15], 16)
                    byte7 = int(line[15:17], 16)
                    byte8 = int(line[17:19], 16)
                    byte9 = int(line[19:21], 16)
                    byte10 = int(line[21:23], 16)
                    byte11 = int(line[23:25], 16)
                    checksum = (0-(byte0+byte1+byte2+byte3+byte4+byte5+byte6+byte7+byte8+byte9+byte10+byte11)) & 0xFF
                    line = line[:9]
                    line += '%02X' % byte4
                    line += '%02X' % byte5
                    line += '%02X' % byte6
                    line += '%02X' % byte7
                    line += '%02X' % byte8
                    line += '%02X' % byte9
                    line += '%02X' % byte10
                    line += '%02X' % byte11
                    line += '%02X' % checksum
                    line += '\n'
                    print line
                    outhex.write(line)
                    eeprom_data_line = False
            else:
                outhex.write(line)           
            if (line == ":0200000400F00A\n"):
                eeprom_data_line = True
        outhex.close()
    inhex.close()

