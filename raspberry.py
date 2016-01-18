#!/usr/bin/env python
# Get i2c Data from Arduino as slave
# Module Imports
import os
import time
import smbus
import struct

# smbus connection Rev 2.0
bus = smbus.SMBus(1) 
# smbus connection Rev 1.0
# bus = smbus.SMBus(0)

# I2C address of Arduino Slave
address = 0x04

bus.write_byte(address,1)
time.sleep(0.5)
data = bus.read_i2c_block_data(address, 0);
bytes = data[0:4]
remoteData = struct.unpack('f', "".join(map(chr, bytes)))[0]
print remoteData
