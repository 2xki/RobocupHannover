import serial
import msvcrt
import time

port = '27'

while True:
    try:
        ser = serial.Serial('COM' + port, 9600)
        break
    except:
        print('wrong port!' + ' (' + 'COM' + port + ')')
        port = input('enter port:  COM')

print(ser.name)

while True:
    if msvcrt.kbhit():
        key = msvcrt.getch()

        if key == b'w':
            out = b'150150\n'
        elif key == b's':
            out = b'050050\n'
        elif key == b'a':
            out = b'140180\n'
        elif key == b'd':
            out = b'180140\n'
        else:
            out = b'100100\n'
    else:
        out = b'100100\n'
    print(out)
    ser.write(out)
    time.sleep(0.1)
ser.close()
