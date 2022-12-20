import time
import paho.mqtt.client as mqtt_client
import random
from statistics import mean
import serial

ser = serial.Serial("COM6", timeout=1)

def on_message(client, userdata, message):
    data = str(message.payload.decode("utf-8"))
    topic = str(message.topic.encode("utf-8"))
    print(f"Received message on {topic}: {data}")
    if float(data) > 50.0:
        ser.write(bytearray([ord('0')]))
    else:
        ser.write(bytearray([ord('1')]))

broker = "broker.emqx.io"

client = mqtt_client.Client(f'dz_{random.randint(10000, 99999)}')
client.on_message = on_message

try:
    client.connect(broker)
except Exception:
    print('Failed to connect. Check network')
    exit()

client.loop_start()

print('Subscribing')
client.subscribe("esp8266/range")
time.sleep(600)
client.disconnect()
client.loop_stop()
print('Stop')
