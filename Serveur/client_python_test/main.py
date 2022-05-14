import time

import paho.mqtt.client as mqtt

# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, flags, rc):
    print("Connected with result code " + str(rc))
    # Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.
    client.subscribe("lora/#")

# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
    topic = str(msg.topic)
    message = str(msg.payload.decode("utf-8"))
    print(topic + " " + message)

client = mqtt.Client()


client.connect("192.168.1.156", 1883, 60)

# Blocking call that processes network traffic, dispatches callbacks and
# handles reconnecting.
# Other loop*() functions are available that give a threaded interface and a
# manual interface.
# With loop_start you have to write the while loop yourself but the programma will continue
while True:
    your_data = "/YidAMTAuMDsxNS4wOzIwLjUhMTAxMDEwMDB+Jw==/up"
    client.publish("lora/",your_data,)
    time.sleep(10)
client.loop_forever()     # loop_forever acts as a while True loop (the program will not continue past here).


# how to publish