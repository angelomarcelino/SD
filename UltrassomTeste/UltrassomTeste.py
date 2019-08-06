import serial
import time

coisa = serial.Serial('/dev/ttyACM0', baudrate = 9600, timeout = 1)

while True:
    arduinoData = coisa.readline().decode('ascii')

    try:
        print(int(arduinoData))
        num = int(arduinoData)
        if num <= 10:
            coisa.write(b'a')
    except:
        pass