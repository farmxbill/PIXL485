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

if (len(sys.argv) != 4):
    print "Invalid number of command line arguments.  Should be:"
    print "python DAFE-eeprom-patcher.py <input.hex> <output.hex> <sensor-type>"
else:
    inhex = open(sys.argv[1], 'r')
    line_found = False
    eeprom_data_line = False
    for line in inhex:
        if (eeprom_data_line == True):
            sys.stdout.write ('\tEEPROM initial data contents ')
            sys.stdout.write (line)
            sw_ver = line[13:19]
            hw_ver = line[9:13]
            eeprom_data_line = False
        if (line == ":0200000400F00A\n"):
            line_found = True
            eeprom_data_line = True

    if (line_found == False):
        print "\tEEPROM address not found in the input file"
    else:
        inhex.seek(0)
        sensor_type = '%02x' % (int(sys.argv[3]) & 0xFF)
        output_filename = sys.argv[2]+'_HW'+hw_ver+'_SW'+sw_ver+'_SensType'+sensor_type+'.hex'
        outhex = open(output_filename, 'w')
        eeprom_data_line = False
        for line in inhex:
            if (eeprom_data_line == True):
                print "\tPatching sensor type", sensor_type, 'and writing to output file', output_filename
                sys.stdout.write ('\tEEPROM patched data contents ')
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
                byte11 = int(sensor_type, 16)
                checksum = (0-(byte0+byte1+byte2+byte3+byte4+byte5+byte6+byte7+byte8+byte9+byte10+byte11)) & 0xFF
                line = line[:23]+sensor_type+('%02X' % checksum)+'\n'
                sys.stdout.write (line)
                outhex.write(line)
                eeprom_data_line = False
            else:
                outhex.write(line)           
            if (line == ":0200000400F00A\n"):
                eeprom_data_line = True
        outhex.close()
    inhex.close()

