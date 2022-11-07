import time
import paho.mqtt.client as mqtt_client
import serial

my_id = 111

interval = 100
set = True

min = 100
max = 0

def get_connection(port):
    ser = serial.Serial(port, timeout=1)
    return ser

client = mqtt_client.Client()

broker = "broker.emqx.io"
try:
    client.connect(broker)
except Exception:
    print('Failed to connect, check network')
    exit()
ser = get_connection('COM8')
steps = 0

while True:
    if ser.in_waiting > 0:
        if ser.in_waiting >= 2 or interval <= 0:
            set = False
            interval += 1
        print("In waiting: " + str(ser.in_waiting))
        data = ser.read(1)
        if min > data[0]:
            min = data[0]
        client.publish("lab/%s/photo/min" % my_id, min)
        if max < data[0]:
            max = data[0]
        client.publish("lab/%s/photo/max" % my_id, max)

        print("Value: " + str(data[0]))
        print("Min: " + str(min) + ", max: " + str(max))
        client.publish("lab/%s/photo/stream" % my_id, data[0])
        ser.write(bytearray([int(ord("I")), int(interval)]))
        print("Interval: " + str(interval))
        if set:
            if steps % 10 == 0:
                interval -= 1
        steps += 1
        print("==============\n")
