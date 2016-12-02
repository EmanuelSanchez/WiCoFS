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
import sys
import subprocess
import argparse
import time
from pymouse import PyMouse
from pykeyboard import PyKeyboard

# global variables
x_limit = 0
y_limit = 0
virtual_mouse = 0
virtual_keyboard = 0
port = 0

count1 = 0
count2 = 0

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

        # except:
        #     exitMessage()

# Open and configure the seial comunication
def openSerial():
    timeout = 20
    cycles_counter = 0
    warnig_flag = 1
    while(1):
        try:    # try to open the serial port
            port = serial.Serial()
            port.baudrate = 9600        # set the baudrate
            port.timeout = 1            # timeout = 1s
            port.port = '/dev/ttyUSB0'  # set port
            port.open()
            break
        except (KeyboardInterrupt, SystemExit):
            raise
        except:
            if warnig_flag:
                print(pcolors.WARNING+"Waiting for Serial Port..."+pcolors.ENDC)
            cycles_counter += 1
            warnig_flag = 0
            if(cycles_counter > timeout):
                raise SystemExit(pcolors.FAIL+"Serial port does not respond"+pcolors.ENDC)

    return port

# function to maps a number from one range to another. (like "map" in Arduino)
def map(x, in_min, in_max, out_min, out_max):
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min

#read new position from the Arduino and update it
def read_data():
    while(port.readline().decode('utf-8') !=  "s\n"):
        print("Waiting synchronization command...")
        pass
    print(pcolors.OKGREEN + "\t OK: " + pcolors.ENDC, end='')
    values = port.readline().decode('utf-8').split(',') # read tilts from the serial port (format: strings)
    roll = int(values[0])  # convert tilts from strings to int
    pitch = int(values[1])
    ultrasoundValue = int(values[2])

    return roll, pitch, ultrasoundValue

def read_4data():
    request()
    values = port.readline().decode('utf-8').split(',') # read tilts from the serial port (format: strings)
    values[0] = float(values[0])
    values[1] = float(values[1])
    values[2] = float(values[2])
    values[3] = float(values[3])
    waitResponse()
    return values[0], values[1], values[2], values[3]

def exitMessage():
    print("\n\n" + pcolors.FAIL + "Exit From Virtual Mouse..." + pcolors.ENDC)
    print("Help: virtualmouse.py -h\n")
    print("Created by:")
    print("\tEmanuel Sánchez\n\tAlejandro Cabezas" + pcolors.ENDC+"\n\n")
    sys.exit(1)

def slaveInit():
    msg_out = 'b'
    port.write(msg_out.encode())
    while(port.readline().decode('utf-8') !=  "y\n"):
        port.write(msg_out.encode())

def request():
    msg_out = 'r'
    port.write(msg_out.encode())
    while(port.readline().decode('utf-8') !=  "o\n"):
        port.write(msg_out.encode())

def waitResponse():
    msg_out = 'w'
    port.write(msg_out.encode())
    while(port.readline().decode('utf-8') !=  "e\n"):
        port.write(msg_out.encode())

def operatingModeNormal():
    msg_out = 'n'
    port.write(msg_out.encode())
    while(port.readline().decode('utf-8') !=  "n\n"):
        port.write(msg_out.encode())

def operatingModeCalibrate():
    msg_out = 'c'
    port.write(msg_out.encode())
    while(port.readline().decode('utf-8') !=  "c\n"):
        port.write(msg_out.encode())

def barLoad(bar_width):
    # setup toolbar
    sys.stdout.write("[%s]" % (" " * bar_width))
    sys.stdout.flush()
    sys.stdout.write("\b" * (bar_width+1)) # return to start of line, after '['
    for i in range(bar_width):
        time.sleep(1) # wait a secod (time to wait the slave calibration)
        # update the bar
        sys.stdout.write("-")
        sys.stdout.flush()

    sys.stdout.write("\n")

def initialRoutine():
    global x_limit, y_limit # declare global variables to change in this function
    global virtual_mouse, virtual_keyboard
    global port

    print(pcolors.HEADER+"In Initial Routine:\t"+pcolors.ENDC)

    timeout = 5 # 100 attempts (cycles)
    cycles_counter = 0
    warnig_flag = 1

    port = openSerial() # open serial port comunication
    print("\tSerial Port: " + port.portstr + pcolors.OKGREEN + "\tConected" + pcolors.ENDC)
    # Send initial command
    print("\tSending initial command to slave...")
    while(1):
        try:
            slaveInit()
            break
        except (KeyboardInterrupt, SystemExit):
            raise
        except:
            if warnig_flag:
                print(pcolors.WARNING+"\tWaiting slave response"+pcolors.ENDC)
            warnig_flag = 0
            cycles_counter += 1
            if(cycles_counter > timeout):
                raise SystemExit(pcolors.FAIL+"Slave does not reponded"+pcolors.ENDC)
    print("  \tSlave response:" + pcolors.OKGREEN + " OK" + pcolors.ENDC)

    virtual_mouse = PyMouse()   # vitual mouse declaration
    virtual_keyboard = PyKeyboard() # virtual keyboard declaration
    x_limit, y_limit = virtual_mouse.screen_size()  # set the screen limits

    print(pcolors.WARNING + "\n\tWARNING: screen size: " + str(x_limit) + "x" + str(y_limit))

    x_limit, y_limit = x_limit-1, y_limit-1

    x_center = int(x_limit/2)    # initial position at screen center
    y_center = int(y_limit/2)
    virtual_mouse.move(x_center, y_center)

    print("\tMouse Initial Position: " + str(virtual_mouse.position()) + pcolors.ENDC + "\n")     # show the mouse position

    return port, x_limit, y_limit

def calibrationRoutine():
    print(pcolors.HEADER+"In Calibration Routine:\t"+pcolors.ENDC)
    print("\tCalibrating X, Y axis and ultrasound:")
    #print("\tKeep both hands horizontally")
    print(pcolors.OKBLUE+"\nMANTENGA LAS MANOS HORIZONTALMENTE"+pcolors.ENDC)
    request()
    barLoad(5)
    waitResponse()
    print(pcolors.OKGREEN + "\tOK" + pcolors.ENDC)
    print("\tCalibrating Accelerometer: Z axis:")
    #print("\tTilt your right hand to the right (vertically)")
    print(pcolors.OKBLUE+"\nINCLINE SU MANO A LA DERECHA (VERTICALMENTE)"+pcolors.ENDC)
    request()
    barLoad(5)
    waitResponse()
    print(pcolors.OKGREEN + "\tOK" + pcolors.ENDC)
    xZero, yZero, zZero, ultrasoundZero = read_4data()
    print("\tx-zero: " + str(xZero))
    print("\ty-zero: " + str(yZero))
    print("\tz-zero: " + str(zZero))
    print("\tultrasound-zero: " + str(0))

def adquisitionLoop(x_limit, y_limit):
    global count1, count2
    print(pcolors.HEADER+"In Adquisition Loop:\t"+pcolors.ENDC)
    while(1):   # loop to read serial port and update the mouse position
        try:
            roll, pitch, ultrasoundValue = read_data()
            print(roll, pitch, ultrasoundValue)
            x_value = map(roll, -90.0, 90.0, 0, x_limit-1)
            y_value = map(pitch, -90.0, 90.0, 0, y_limit-1)

            x = int(x_value)
            y = int(y_value)

            virtual_mouse.move(x,y)
            if(ultrasoundValue>1):
                count1 = count1 + 1
                print(count1)
                if(count1 > 20):
                    count1 = 18
                else:
                    virtual_keyboard.press_key("Page_Up")
                    pass
            elif(ultrasoundValue<-1):
                print(count1)
                count1 = count1 - 1
                if(count1 < -20):
                    count1 = -18
                else:
                    virtual_keyboard.press_key("Page_Down")
                    pass
            # if(ultrasoundValue>2):
            #     count2 = 0
            #     count1 = count1 + 1
            #     if(count1 > 5):
            #         virtual_keyboard.press_key("Page_Up")
            #         count = 0
            # elif(ultrasoundValue<-2):
            #     count1 = 0
            #     count2 = count2 + 1
            #     if(count2 < 5):
            #         virtual_keyboard.press_key("Page_Down")
            #         count = 0
        except (KeyboardInterrupt, SystemExit):
            exitMessage()
            raise
        except:
            pass

    port.close()

parser = argparse.ArgumentParser(description='This is a Flight Simulator Control Program')
parser.add_argument('-r', action='store_false', default=False,
                    dest='boolean_switch',
                    help='Run controls with default calibration')
parser.add_argument('-c', action='store_true', default=False,
                    dest='boolean_switch',
                    help='Run controls with a calibration routine')
args = parser.parse_args()
if not(args.boolean_switch):
    print(pcolors.OKBLUE + "\nNORMAL-MODE" + pcolors.ENDC)
    port, x_limit, y_limit = initialRoutine()
    operatingModeNormal()
    adquisitionLoop(x_limit, y_limit)
else:
    print(pcolors.OKBLUE + "\nCALIBRATE-MODE" + pcolors.ENDC)
    port, x_limit, y_limit = initialRoutine()
    operatingModeCalibrate()
    calibrationRoutine()
    adquisitionLoop(x_limit, y_limit)
