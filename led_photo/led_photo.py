import serial
import time

lengths = {'s': 3,
           'u': 0,
           'd': 0,
           'n': 3, 
           'x': 0}

def get_connection(port):
    ser = serial.Serial(port, timeout=1)
    return ser

def send(ser, message, mesg_len):
    ser.write(message)
    time.sleep(0.1)
    result = None
    if mesg_len != 0:
        data = ser.read(mesg_len)
        result = data.decode()
        result = result.strip()
        print(result)
    return result

val_max = 0
val_min = 1024
lst = []
stop = False
if __name__ == '__main__':
    ser = get_connection("COM6")
    while True:
        while stop:
            timeout = time.time() + 10
            cmd = 's'
            val = send(ser, cmd.encode(), lengths[cmd])
            if val:
                val = int(val)
                lst.append(val)
                if val > val_max:
                    val_max = val
                if val < val_min:
                    val_min = val
                if val < ((val_min + val_max) / 2):
                    send(ser, 'u'.encode(), 0)
                else:
                    send(ser, 'd'.encode(), 0)
                time.sleep(1)
            if (time.time() > timeout):
                break 
        inp = input("Enter command:")
        length = lengths.get(inp, 17)
        send(ser, inp.encode(), length)
        if inp == 'n':
            stop = True
        if inp == 'x':
            stop = False