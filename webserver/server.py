#!/usr/bin/python3
"""
This is a small web server for Aalto weatherserver project.
"""
import os
import sys
import serial # pip3 install pyserial
import time
import threading
from flask import Flask, render_template # pip3 install flask
 
# Global variables shared between the main and SensorRead threads
WIND = "0"
TEMP = "20"
HUMI = "40"
PRES = "1000"
 
# Set the INPUTDEVICE value to correct input device for reading
# sensors For testing use a local file (or named pipe to test the
# changing of the values)
 
INPUTDEVICE = "sensors.test"
if os.path.exists('/dev/ttyUSB0'):
    INPUTDEVICE = "/dev/ttyUSB0"
elif os.path.exists('/dev/ttyACM0'):
    INPUTDEVICE = "/dev/ttyACM0"
elif os.path.exists('/dev/ttyACM1'):
    INPUTDEVICE = "/dev/ttyACM1"
elif os.path.exists('/dev/ttyACM2'):
    INPUTDEVICE = "/dev/ttyACM2"
elif os.path.exists('/dev/ttyACM3'):
    INPUTDEVICE = "/dev/ttyACM3"
elif os.path.exists('/dev/ttyACM4'):
    INPUTDEVICE = "/dev/ttyACM4"
elif os.path.exists('/dev/ttyACM5'):
    INPUTDEVICE = "/dev/ttyACM5"
 
# Web server app instance using Flask Framework
app = Flask(__name__)
 
def is_number(s):
    try:
        float(s)
        return True
    except ValueError:
        pass
    try:
        import unicodedata
        unicodedata.numeric(s)
        return True
    except (TypeError, ValueError):
        pass
    return False
     
class Thread_SensorReader(threading.Thread):
    """
    Sensor reader thread
    """
    def __init__(self, name):
        threading.Thread.__init__(self)
        self.name = name
 
 
    def run(self):
        """
        Run the thread for sensor reader
        """
        global WIND
        global TEMP
        global HUMI
        global PRES
        print("Using inputdevice: ", INPUTDEVICE)
        weather_tag = False
        while True:
            try:
                linenumber = 0
                ser = serial.Serial(INPUTDEVICE, 9600)
                for line in ser:
                    line = line.rstrip()
                    print("#LINE:", line)
                    if len(line) == 0:
                        continue
                    values = line.split()
                    if len(values) >= 4:
                        print("#DEBUG: 4 values:", values)
                        if is_number(values[0]) and is_number(values[1]) and is_number(values[2]) and is_number(values[3]):
                            WIND = "{:.1f}".format(float(values[0])/10)
                            HUMI = "{:.1f}".format(float(values[1])/10)
                            TEMP = "{:.1f}".format(float(values[2])/100)
                            PRES = "{:.0f}".format(float(values[3]))
                    # Remove the sleep if the sensor feed is faster than 1 RPS
                    #time.sleep(1)
            except:
                print("Unexpected error reading sensors:", sys.exc_info()[0])
                time.sleep(1)
 
@app.route("/")
def main():
    """
    Serve the main html page
    """
    templateData = {
        'windspeed' : WIND,
        'temperature' : TEMP,
        'humidity': HUMI,
        'pressure': PRES
    }
    return render_template('main.html', **templateData)
 
@app.route("/sensors")
def sensors():
    """
    Serve the /sensors data page
    """
    return WIND + " " + TEMP + " " + HUMI + " " + PRES
 
if __name__ == "__main__":
    sensor_thread = Thread_SensorReader('SensorReader')
    sensor_thread.start()
    app.run(host='0.0.0.0', port=8080, debug=False)
