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
   y = trapmf(x, 0, 0, 5, 25, 0, 255)
   return f'p{y}\n'.encode()

def medio(x):
   y = trapmf(x, 10, 25, 30, 80, 0, 255)
   return f'm{y}\n'.encode()

def longe(x):
   y = trapmf(x, 30, 80, 500, 500, 0, 255)
   return f'l{y}\n'.encode()

if __name__ == '__main__':
    serial = ser.Serial('/dev/ttyUSB0', baudrate=115200)

    while True:
        arduinoData = serial.readline().decode('ascii')

        try:
            num = int(arduinoData)
                        
            print(f'perto: {perto(num)}\nmedio: {medio(num)}\nlonge: {longe(num)}\n\n')

            serial.write(perto(num))
            serial.write(medio(num))
            serial.write(longe(num))

            #serial.write(b'p')
            #serial.write(coiso)
            #serial.write(b'\n')

            #serial.write(b'm')
            #serial.write(str(medio(num)).encode())
            #serial.write(b'\n')

            #serial.write(b'l')
            #serial.write(str(longe(num)).encode())
            #serial.write(b'\n')

        except:
            print('Error converting to int!')