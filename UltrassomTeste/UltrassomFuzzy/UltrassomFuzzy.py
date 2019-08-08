import serial
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
   y = trapmf(x, 0, 0, 5, 25, 0, 255)
   return int(y)

def medio(x):
   y = trapmf(x, 10, 25, 30, 80, 0, 255)
   return int(y)

def longe(x):
   y = trapmf(x, 30, 80, 500, 500, 0, 255)
   return int(y)

coisa = serial.Serial('/dev/ttyACM0', baudrate = 9600, timeout = 1)

while True:
    arduinoData = coisa.readline().decode('ascii')

    try:
        num = int(arduinoData)

        coisa.write(b'p')
        coisa.write(str(perto(num)).encode())
        coisa.write(b'\n')

        coisa.write(b'm')
        coisa.write(str(medio(num)).encode())
        coisa.write(b'\n')

        coisa.write(b'l')
        coisa.write(str(longe(num)).encode())
        coisa.write(b'\n')

    except:
        pass