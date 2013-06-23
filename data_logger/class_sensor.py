'''
Data logger for recording and displaying data recorded by an
Arduino temperature and humidity sensor.
'''

import os
import sys
import time
import datetime
import serial
import numpy as np

# dictionary of all current sensors and ports #
comms = {}
comms['arduino'] = 4-1

class sensor:
    '''Sensor class for recording timestamp, temperature and humidity.'''
    def __init__(self, sensor_id):
        self.id = sensor_id
        self.timestamp = np.array([])
        self.temperature = np.array([])
        self.humidity = np.array([])
    
    # communication protocols: initialisation and maintanance commands #
    def initialise(self):
        '''Open communications with Arduino'''
        # open comms and establist contact #
        self.open_comms()
        ser = self.ser
        time.sleep(1)
        ser.timeout = 1
        # sync time #
        while True:
            msg = ''
            # get single byte handshaking messages #
            while not ser.inWaiting(): continue
            msg = ser.read()
            # identify initialisation codes #
            if msg == 'A':
                print 'Python: communcations requested'
                self.establish_contact()
            if msg == '\x07':
                print 'Python: time synchronisation requested'
                self.sync_time()
                break
            else: print msg
        return 0

    def open_comms(self):
        port = comms[self.id]
        self.ser = serial.Serial(port, baudrate=9600, bytesize=8, parity='N',
                            stopbits=1, timeout=0)
        #ser.open()
        if self.ser.isOpen(): return 0
        else: return -1

    def close_comms(self):
        self.ser.close()
        if not self.ser.isOpen(): return 0
        else: return -1
    
    def establish_contact(self):
        '''Waits for communication request (A) from Arduino and responds (R)'''
        self.ser.write('R')
        time.sleep(1)
        print 'Python: established contact'
        return 0
    
    def sync_time(self):
        '''Waits for time request from Arduino (bell) and
        responds with headed time'''
        ser = self.ser
        # respond to time request #
        t0 = int(time.time())
        t0_str = 'T' + str(t0)
        ser.write(t0_str)
        time.sleep(1)
        print 'Python: synchronisation sent'
        # wait for confirmation #
        while True:
            while ser.inWaiting() <= 0: time.sleep(0.1)
            msg = ser.readline().strip()
            if '\x06' in msg:
                print 'Python: synchronisation acknowledged'
                break
        print 'Python: time synchronised'
        return 0
    
    def resync_time(self):
        '''Waits for resynchronising request from Arduino (bell) and
        responds with headed time'''
        ser = self.ser
        # respond to time request #
        t0 = int(time.time())
        t0_str = 'T' + str(t0)
        ser.write(t0_str)
        time.sleep(1)
        # no confirmation necessary #
        print 'Python: time synchronised'
        return 0
    
    # sensor monitoring and data logging #
    def start_monitoring(self, period):
        self.initialise()
        ser = self.ser
        # set arduino timer period #
        ser.write('delay ' + str(period) + '\r\n')
        ser.read()
        # load data arrays or create if necessary #
        try:
            npz_archive = np.load('c:\\users\\alan\\desktop\\sensor1.npz')
            self.timestamp = npz_archive['t']
            self.temperature = npz_archive['temp']
            self.humidity = npz_archive['hum']
            print 'sensor loaded'
        except IOError:
            print 'sensor initialised'
        return 0
    
    def stop_monitoring(self):
        self.close_comms()
        np.savez('c:\\users\\alan\\desktop\\sensor1',
                 t = self.timestamp, temp = self.temperature, hum = self.humidity)
        return 0
    
    def log_data(self):
        # read serial data #
        line = ''
        ser = self.ser
        while ser.inWaiting() or not line.endswith('\n'):
            line += ser.read(ser.inWaiting())
            # resync request #
            if '\x07' in line:
                self.resync_time()
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
            #print t, temp, hum
            # append values to arrays
            self.timestamp = np.append(self.timestamp, t)
            self.temperature = np.append(self.temperature, temp)
            self.humidity = np.append(self.humidity, hum)
        return 0

if __name__ == '__main__':
    import matplotlib.pyplot as plt
    
    period = 1
    sensor1 = sensor('arduino')
    sensor1.start_monitoring(period)

    t0 = time.time()
    while time.time() - t0 <= 20:
        sensor1.log_data()
        #time.sleep(30)
    sensor1.stop_monitoring()

    fig = plt.figure()
    ax = fig.add_subplot(111)
    ax.plot(sensor1.timestamp, sensor1.temperature)
    plt.show()
