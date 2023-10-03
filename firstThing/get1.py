import time
import paho.mqtt.client as paho
import random
import serial
import time

broker = "broker.emqx.io"

responses = {'d': 7,
             'u': 6}

port = "COM5"
connection = serial.Serial(port, timeout=1)

def send_command(cmd: str, response_len: int) -> str:
    str_resp = None
    connection.write(cmd.encode())
    if response_len != 0:
        #connection.in_waiting <-> аналог availible в ардуино
        resp = connection.read(response_len)
        str_resp = resp.decode()
    return str_resp

def on_message(client, userdata, message):
    time.sleep(1)
    data = str(message.payload.decode("utf-8"))
    send_command(data, responses[data])

client = paho.Client("client-isu-00211")
client.on_message = on_message

print("Connecting to broker ", broker)
client.connect(broker)
client.loop_start()
print("Subscribing")
client.subscribe("house808/bulb1")
time.sleep(60)
client.disconnect()
client.loop_stop()