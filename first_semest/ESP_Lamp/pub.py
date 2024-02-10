import paho.mqtt.client as mqtt_client 
import random 
import time
from datetime import datetime, timedelta
 
broker = "broker.emqx.io"
def on_connect(client, userdata, flags, recieve_c): 
    if recieve_c == 0: 
        print("Connected to MQTT Broker!") 
    else: 
        print("Failed to connect, return code %d\n", recieve_c) 
 
client = mqtt_client.Client(f'lab_{random.randint(10000, 99999)}') 
client.on_connect = on_connect 
client.connect(broker) 

min_time = 10
start_time = 20

while True:
    state = "d"
    client.publish("esp8266f32a/command", state)
    print(f"publish state is {state}")

    time.sleep(start_time)

    start_time = max(min_time, start_time - 1)

    state = "u"
    client.publish("esp8266f32a/command", state)
    print(f"publish state is {state}")

    current_time = datetime.utcnow()
    next_minute = (current_time + timedelta(minutes=1)).replace(second=0, microsecond=0)
    time_until_next_minute = (next_minute - current_time).total_seconds()
    sleep_time = time_until_next_minute - start_time

    time.sleep(sleep_time)