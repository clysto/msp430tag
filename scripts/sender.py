#!/usr/bin/env python3

import sys
import serial
from struct import unpack, pack


device = sys.argv[1]
f = open(sys.argv[2], "rb")
content = f.read()
cur = 0

client = serial.Serial(device, 9600)
while True:
    data = client.read(5)
    (size,) = unpack("I", data[1:])
    data = content[cur : cur + size]
    actual_size = pack("I", len(data))
    client.write(actual_size + data)
    cur = (cur + size) % len(content)
