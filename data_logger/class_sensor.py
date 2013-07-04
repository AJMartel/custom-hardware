'''
Data logger for recording and displaying data recorded by an
Arduino temperature and humidity sensor.
'''

import os
import sys
import socket
import itertools

import time
import datetime
import serial
import numpy as np

# dictionary of all current sensors and ports #
host_name = socket.gethostname()
comms = {}
if host_name == "Alan-VAIO":
    data_dir = 'C:\\users\\alan\\desktop'
    comms['arduino'] = 'COM4'
elif host_name == "NP-ASTATINE":
    data_dir = 'C:\\users\\alan\\desktop'
    comms['arduino'] = 'COM7'
elif host_name == "np-magnesium":
    data_dir = 'C:\\users\\hera\\desktop'
    comms['arduino'] = 'COM28'
else:
    data_dir = ''
    comms["arduino"] = 'dev\ttylACM0'

class sensor:
    '''Sensor class for recording timestamp, temperature and humidity.'''
    def __init__(self, sensor_id):
        self.id = sensor_id
        self.timestamp_s = np.array([])
        self.timestamp = np.array([], dtype='datetime64')
        self.temperature = np.array([])
        self.humidity = np.array([])
        self.timestamp_s_temp = np.array([])
        self.timestamp_temp = np.array([], dtype='datetime64')
        self.temperature_temp = np.array([])
        self.humidity_temp = np.array([])
        self.seconds = 0
        self.time = 0
        self.temp = 0
        self.hum = 0
        self.max_size = 104857600 / 8 # 100 mB per array / 8 bytes per float entry
        self.fname = os.path.join(data_dir, self.id +".txt")
    
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
            with open(self.fname, 'r') as f:
                print 'sensor found'
        except IOError:
            self.initialise_datafile()
            print 'sensor initialised'
        return 0
    
    def stop_monitoring(self):
        self.close_comms()
        return 0
    
    def log_data(self):
        # read serial data #
        line = ''
        ser = self.ser
        if not ser.inWaiting(): return 0
        while ser.inWaiting() or not line.endswith('\n'):
            line += ser.read(ser.inWaiting())
            # resync request #
            if '\x07' in line:
                self.resync_time()
                line = line.replace('\x07', '')
            if '\x06' in line: print 'ack?'
        # parse response #
        lines = line.splitlines()
        #lines = [line.strip('\x01') for line in lines if line != '' and '\x01' in line]
        lines = [line for line in lines if line != '']
        for line in lines:
            t, temp, hum = line.split(',')
            self.seconds = float(t)
            self.time = datetime.datetime.fromtimestamp(self.seconds)
            self.temp = float(temp)
            self.hum = float(hum)
            # append values to arrays #
            self.timestamp_s_temp = np.append(self.timestamp_s_temp, self.seconds)
            self.timestamp_temp = np.append(self.timestamp_temp, self.time)
            self.temperature_temp = np.append(self.temperature_temp, self.temp)
            self.humidity_temp = np.append(self.humidity_temp, self.hum)
        self.save_data()
        return 1
    
    def initialise_datafile(self):
        self.fname = os.path.join(data_dir, self.id + ".txt")
        with open(self.fname, 'w') as f:
            f.write("time (s)\ttimestamp\ttemperature (degC)\thumidity (% RH)\n")
            #header = "time (s)\ttimestamp\ttemperature (degC)\thumidity (% RH)"
            #np.savetxt(f, np.array([]), delimiter='\t', header=header)
        return 0
    
    def save_data(self):
        #latest_data = np.concatenate((self.timestamp.T, self.temperature.T, self.humidity.T), axis=1)
        with open(self.fname, 'a') as f:
            np.savetxt(f, np.c_[self.timestamp_s_temp, self.timestamp_temp, self.temperature_temp, self.humidity_temp],
                       fmt='%.3f\t%s\t%.2f\t%.2f', delimiter='\t')
        self.timestamp_s_temp = np.array([])
        self.timestamp_temp = np.array([])
        self.temperature_temp = np.array([])
        self.humidity_temp = np.array([])
        return 0
    
    def load_data(self, t_min, t_max):
        # determine limits #        
        with open(self.fname, 'r') as f:
            i = 1   # start at 1 to ignore header
            temp = np.array([])
            if t_max <= t_min:
                i_max = np.inf
                t_max = np.inf
            for line in f:
                if "time" in line: continue
                t = float(line.split('\t')[0])
                if t >= t_min and t <= t_max: temp = np.append(temp, i)
                elif t > t_max: break
                i += 1
            i_min = temp[0]; i_max = temp[-1]
        
        # determine memory constraints #
        i_step = 1
        if i_max - i_min > self.max_size:
            i_step  = round((i_max - i_min)/self.max_size +0.5)
                    
        # get data #
        with open(self.fname, 'r') as f:
            data = np.genfromtxt(itertools.islice(f, i_min, i_max+1, i_step),
                                 delimiter='\t', names=("timestamp_s", "timestamp", "temperature", "humidity"),
                                 dtype=(None))
            self.timestamp_s = data['timestamp_s']
            self.timestamp = np.array([datetime.datetime.fromtimestamp(x) for x in self.timestamp_s])
            self.temperature = data['temperature']
            self.humidity = data['humidity']
        return 0

if __name__ == '__main__':    
    period = 1
    sensor1 = sensor('arduino')
    sensor1.start_monitoring(period)

    t0 = time.time()
    while time.time() - t0 <= 10:
        sensor1.log_data()
        time.sleep(1)
    sensor1.stop_monitoring()
    print "test complete"
