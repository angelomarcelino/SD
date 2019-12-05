import serial as ser

if __name__ == '__main__':
    serial = ser.Serial('/dev/ttyUSB0', baudrate=38400)

    while True:
        try:
            string = serial.readline().decode('ascii')

            print(string)
            
        except UnicodeDecodeError:
            #print('Error converting to int!')
            pass