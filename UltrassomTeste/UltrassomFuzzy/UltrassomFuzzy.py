import serial as ser
import time

def trapmf(x, a, b, c, d, minimo, maximo):
    y = 0

    if x <= a:
        y = minimo
    elif (a <= x) & (x < b):
        y = maximo * (x - a)/(b - a)
    elif (b <= x) & (x < c):
        y = maximo
    elif (c <= x) & (x < d):
        y = maximo * (d - x)/(d - c)
    elif d <= x:
        y = minimo
    
    return int(y)

def perto(x):
   return trapmf(x, 0, 0, 5, 25, 0, 255)

def medio(x):
   return trapmf(x, 5, 20, 30, 35, 0, 255)

def longe(x):
   return trapmf(x, 30, 35, 500, 500, 0, 255)

def motor(x):
    return trapmf(x, 0, 30, 500, 500, 0, 255)


if __name__ == '__main__':
    serial = ser.Serial('/dev/ttyUSB0', baudrate=115200)

    while True:
        try:
            arduinoData = int(serial.readline().decode('ascii'))

            serial.write(f'd{perto(arduinoData)};{medio(arduinoData)};{longe(arduinoData)};{motor(arduinoData)}'.encode())
            
        except UnicodeDecodeError:
            #print('Error converting to int!')
            pass