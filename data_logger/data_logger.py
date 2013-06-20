'''
Data logger for recording and displaying data recorded by an
Arduino temperature and humidity sensor.
'''

import os
import sys
import time
import datetime
from serial_comms import *
import numpy as np

import matplotlib
matplotlib.use('WXAgg')
from matplotlib import pyplot as plt

import wx

# initialisation and maintainance #

def initialise():
    '''Open communications with Arduino'''
    # open comms and establist contact #
    ser = open_comms('arduino')
    time.sleep(1)
    ser.timeout = 1
    # sync time #
    while True:
        msg = ''
        # get single byte handshaking messages #
        while not ser.inWaiting():
            continue
        msg = ser.read()
        # identify initialisation codes #
        if msg == 'A':
            print 'Python: communcations requested'
            establish_contact(ser)
        if msg == '\x07':
            print 'Python: time synchronisation requested'
            sync_time(ser)
            break
        else:
            print msg
    return ser

def establish_contact(ser):
    '''Waits for communication request (A) from Arduino and responds (R)'''
    ser.write('R')
    time.sleep(1)
    print 'Python: established contact'
    return 0

def sync_time(ser):
    '''Waits for time request from Arduino (bell) and
    responds with headed time'''
    # respond to time request #
    t0 = int(time.time())
    t0_str = 'T' + str(t0)
    ser.write(t0_str)
    time.sleep(1)
    print 'Python: synchronisation sent'
    # wait for confirmation #
    while True:
        while ser.inWaiting() <= 0:
            time.sleep(0.1)
        msg = ser.readline().strip()
        if '\x06' in msg:
            print 'Python: synchronisation acknowledged'
            break
    print 'Python: time synchronised'
    return 0

def resync_time(ser):
    '''Waits for resynchronising request from Arduino (bell) and
    responds with headed time'''
    # respond to time request #
    t0 = int(time.time())
    t0_str = 'T' + str(t0)
    ser.write(t0_str)
    time.sleep(1)
    # no confirmation necessary #
    print 'Python: time synchronised'
    return 0

# monitoring and data logging #

class sensor:
    '''
    Sensor class for recording timestamp, temperature and humidity.
    '''
    def __init__(self):
        self.timestamp = np.array([])
        self.temperature = np.array([])
        self.humidity = np.array([])

def start_monitoring(period):
    # set arduino timer period #
    ser.write('delay ' + str(period) + '\r\n')
    ser.read()
    # load data arrays or create if necessary #
    try:
        npz_archive = np.load('c:\\users\\alan\\desktop\\sensor1.npz')
        sensor1 = sensor()
        sensor1.timestamp = npz_archive['t']
        sensor1.temperature = npz_archive['temp']
        sensor1.humidity = npz_archive['hum']
        print 'sensor1 loaded'
    except IOError:
        sensor1 = sensor()
        print 'sensor1 initialised'
    return sensor1

def stop_monitoring(ser, sensor):
    close_comms(ser)
    np.savez('c:\\users\\alan\\desktop\\sensor1',
             t = sensor.timestamp, temp = sensor.temperature, hum = sensor.humidity)
    return 0

def measure_data_old(ser):
    while not ser.inWaiting():
        continue
    line = ser.readline().strip()
    ## parse response ##
    if ',' in line:
        line.split(",")
        t, temp, hum = line.split(",")
        t = float(t)
        t = datetime.datetime.fromtimestamp(t)
        print t, temp, hum
    else: print line
    return 0

def log_data(ser, sensor):
    # read serial data #
    line = ''
    while ser.inWaiting() or not line.endswith('\n'):
        line += ser.read(ser.inWaiting())
        # resync request #
        if '\x07' in line:
            resync_time(ser)
            line = line.replace('\x07', '')
        if '\x06' in line:
            print 'ack?'
    # parse response #
    lines = line.splitlines()
    #lines = [line.strip('\x01') for line in lines if line != '' and '\x01' in line]
    lines = [line for line in lines if line != '']
    for line in lines:
        t, temp, hum = line.split(',')
        t = float(t)
        temp = float(temp)
        hum = float(hum)
        t = datetime.datetime.fromtimestamp(t)
        print t, temp, hum
        # append values to arrays
        sensor.timestamp = np.append(sensor.timestamp, t)
        sensor.temperature = np.append(sensor.temperature, temp)
        sensor.humidity = np.append(sensor.humidity, hum)
    return sensor

# update graphs #

period = 1
ser = initialise()
sensor = start_monitoring(period)

t0 = time.time()
while time.time() - t0 <= 20:
    sensor = log_data(ser, sensor)
    #time.sleep(30)
stop_monitoring(ser, sensor)

fig = plt.figure()
ax = fig.add_subplot(111)
ax.plot(sensor.timestamp, sensor.temperature)
plt.show()
