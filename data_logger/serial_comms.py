import serial

# dictionary of hardware id to serial communications
comms = {}
comms['arduino'] = ['COM7', 9600, 8, 'N', 1]

def open_comms(hardware_id):
    comm_info = comms[hardware_id]
    port = comm_info[0]
    baud = comm_info[1]
    byte = comm_info[2]
    par = comm_info[3]
    stop = comm_info[4]
    ser = serial.Serial(port, baudrate=baud, bytesize=byte, parity=par,
                        stopbits=stop, timeout=0)
    #ser.open()
    if ser.isOpen(): return ser
    else: return -1

def close_comms(ser):
    ser.close()
    if not ser.isOpen(): return 0
    else: return -1

def check_serial():
    return 0

def write(ser, cmd):
    ser.write(cmd + '\r\n')
    return 0

def read(ser, cmd, nbytes=1000):
    ser.write(cmd + '\r\n')
    s = ser.read(nbytes)
    return s

def readline(ser, cmd):
    ser.write(cmd + '\r\n')
    s = ser.readline()
    return s

def read_only(ser, nbytes=1000):
    s = ser.read(nbytes)
    return s

def readline_only(ser):
    s = ser.readline()
    return s

def readlines_only(ser):
    s = ser.readlines()
    return s

if __name__ == '__main__':
    import time
    ser = open_comms('arduino')
    print readline_only(ser)
    print "T"+str(int(time.time()))
    write(ser, "T"+str(int(time.time())))
    i = 0
    while (True):
        time = readline_only(ser)
        print time
        i += 1
        if i > 5:
            break
    close_comms(ser)
