# A simple gateway between the SODA API used in LAX parking datase and MQTT broker "neptune.usc.edu"

import paho.mqtt.client as mqtt
import requests
import urllib
import json
import time

def on_log(client, userdata, level, buf):
    print("log: ",buf)

def on_connect(client, userdata, flags, rc):
    m="Connected flags"+str(flags)+"result code "\
    +str(rc)+"client1_id  "+str(client)
    print(m)
    
def on_message(client1, userdata, message):
    print("message received  "  ,str(message.payload.decode("utf-8")))

url='https://data.lacity.org/resource/xzkr-5anj.json'

json_data=requests.get(url,verify=False).json()          #Gets data from SODA API
t = float(raw_input("Publish time in seconds:"))
while 1:
	for i in range(0,9):
		json_freespace=json_data[i]
		client = mqtt.Client()
		client.loop_start()
		client.on_connect= on_connect              #attach function to callback
		client.on_message=on_message   
		client.on_log=on_log    
		client.connect("neptune.usc.edu",1883,60)  #connect to broker
		topic = "i3/lacity/lax/parkinglot/"+str(json_freespace['parkingid'])
		print str(json_freespace)
		client.publish(topic,str(json_freespace),2)
		client.loop_stop()
	time.sleep(t)                                  


