import time
import paho.mqtt.client as paho
import random

broker = "broker.emqx.io"

client = paho.Client("client-isu-00210")

print("connecting to broker ", broker)
client.connect(broker)
client.loop_start()
print("Publishing")
state = "d"

for _ in range(10):
    state = "u" if state == "d" else "d"
    print(f"{state}")
    client.publish("house808/bulb1", state)
    time.sleep(1)

client.disconnect()
client.loop_stop()