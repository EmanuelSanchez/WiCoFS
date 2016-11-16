#####################################################################
#
#  Software to emulate mouse and control the Flight Simulator
#   of Google Earth
#
#   More Information:
#      PyMouse: https://github.com/pepijndevos/PyMouse/wiki/Documentation
#      PyKeyboard: https://pypi.python.org/pypi/PyUserInput/
#####################################################################

import serial
import pykeyboard
import sys
from pymouse import PyMouse

# global variables to screen limits
x_limit = 0
y_limit = 0

# class to print with colors
class pcolors:
    HEADER = '\033[95m'     # violet
    OKBLUE = '\033[94m'     # blue
    OKGREEN = '\033[92m'    # green
    WARNING = '\033[93m'    # yellow
    FAIL = '\033[91m'       # red
    ENDC = '\033[0m'        # return to default color
    BOLD = '\033[1m'        # brown
    UNDERLINE = '\033[4m'

# code to remenber some functions (trash)
def backup ():
    m = PyMouse()
    m.click(500,300,1)
    a, b = m.screen_size()
    print(a)
    print(b)
    print(type(a),type(b))
    a = int(a/2)
    b = int(b/2)
    print(type(a))
    print(type(b))
    m.move(a,b)
    print(m.position())

# Open and configure the seial comunication
def openSerial():
    # while(1):
    #         port = serial.Serial()
    #         port.baudrate = 9600        # set the baudrate
    #         port.timeout = 1            # timeout = 1s
    #         port.port = '/dev/ttyUSB0'  # set port
    #         port.open()
    #         print(port.readline())
    while(1):
        try:    # try to open the serial port
            port = serial.Serial()
            port.baudrate = 9600        # set the baudrate
            port.timeout = 1            # timeout = 1s
            port.port = '/dev/ttyUSB0'  # set port
            port.open()
            # print(port.readline())
            print("Serial Port: " + port.portstr + pcolors.OKGREEN + "\t\tConected" + pcolors.ENDC)
            break
        except:
            print("Waiting for Serial Port...")

    return port

# function to maps a number from one range to another. (like "map" in Arduino)
def map(x, in_min, in_max, out_min, out_max):
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min

#read new position from the Arduino and update it
def read_data(port):
    print("Waiting synchronization command...")
    # print(port.readline().decode('utf-8'))
    while(port.readline().decode('utf-8') !=  "c\n"):
        pass
    print("Synchronization command ... " + pcolors.OKGREEN + "\t OK" + pcolors.ENDC)
    values = port.readline().decode('utf-8').split(',') # read tilts from the serial port (format: strings)
    roll = float(values[0])  # convert tilts from strings to int
    pitch = float(values[1])
    sharp = float(values[2])

    # while(1):
    #     try:    # try to read the complete line
    #         values = port.readline().decode('utf-8').split(',') # read tilts from the serial port (format: strings)
    #         roll = float(values[0])  # convert tilts from strings to int
    #         pitch = float(values[1])
    #         sharp = float(values[2])
    #         #print("Roll: " + str(roll) + "\tPitch: " + str(pitch) + "\tSharp: " + str(sharp))
    #         break
    #     except: # if not read the complete line, try again
    #         pass

    # if(roll < -90.0 | roll > 90.0):       # limits evaluation
    #     print(pcolors.FAIL + "Error in roll value" + pcolors.ENDC)
    # if(pitch < -90.0 | pitch > 90.0):
    #     print(pcolors.FAIL + "Error in Y value" + pcolors.ENDC)

    return roll, pitch, sharp

def main():
    global x_limit, y_limit # declare global variables to change in this function

    print("\n" + pcolors.OKBLUE + "Begining..." + pcolors.ENDC +"\n")   # initial message

    port = openSerial() # open serial port comunication
    virtual_mouse = PyMouse()   # vitual mouse declaration
    x_limit, y_limit = virtual_mouse.screen_size()  # set the screen limits

    print(pcolors.WARNING + "WARNING: screen size: " + str(x_limit) + "x" + str(y_limit) + pcolors.ENDC)

    x_limit, y_limit = x_limit-1, y_limit-1

    x_center = int(x_limit/2)    # initial position at screen center
    y_center = int(y_limit/2)
    virtual_mouse.move(x_center, y_center)

    print("\nMouse Initial Position: " + str(virtual_mouse.position()) + "\n")     # show the mouse position

    while(1):   # loop to read serial port and update the mouse position
        try:
            roll, pitch, sharp = read_data(port)

            x_value = map(roll, -90.0, 90.0, 0, x_limit-1)
            y_value = map(pitch, -90.0, 90.0, 0, y_limit-1)

            x = int(x_value)
            y = int(y_value)

            # if(x>=x_limit):      # verify screen limits and correct 'x' and 'y' position
            #     x = x_limit-1
            # elif(x<0):
            #     x = 0
            # if(y>=y_limit):
            #     y = y_limit-1
            # elif(y<0):
            #     y=0

            print(x,y)

            virtual_mouse.move(x,y)
        except:
            print("\n\n" + pcolors.WARNING + "Exit From Virtual Mouse...\n\n" + pcolors.ENDC)
            print("Created by:")
            print(pcolors.OKBLUE + "\tEmanuel Sánchez\n\tAlejandro Cabezas" + pcolors.ENDC+"\n\n")
            sys.exit(1)

    port.close()

# function to proof the set virtual mouse position
def proof():
    m = PyMouse()
    a, b = m.screen_size()
    print(m.screen_size())
    m.move(0,0)

if __name__ == "__main__":
    main()
