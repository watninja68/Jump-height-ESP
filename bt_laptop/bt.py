import serial
import time
port = "COM7"
bluetooth = serial.Serial(port, 115200, timeout=1)
bluetooth.flushInput()
bluetooth_data = []
for i in range(5):
    bluetooth.write(b"BOOP " + str.encode(str(i)))
    input_data = bluetooth.readline().decode().strip()
    print(input_data)
    bluetooth_data.append(input_data)
