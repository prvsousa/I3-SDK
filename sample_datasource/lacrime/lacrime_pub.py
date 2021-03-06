#!/usr/bin/python
"""
test_sub.py is an example of a publisher subscribing to a topic
"""

import paho.mqtt.client as mqtt
import datetime
from datetime import timedelta
import requests
import json
import os

def on_connect(client, userdata, flags, rc):
    """print out result code when connecting with the broker

    Args:
        client: publisher
        userdata:
        flags:
        rc: result code

    Returns:

    """

    m="Connected flags"+str(flags)+"result code "\
    +str(rc)+"client1_id  "+str(client)
    print(m)



def on_message(client1, userdata, message):
    """print out recieved message

    Args:
        client1: publisher
        userdata:
        message: recieved data

    Returns:

    """
    print("message received  "  ,str(message.payload.decode("utf-8")))


if __name__ == '__main__':

    #TODO: modify topic from email message
    # account : username created on I3 instance
    # pw : system generated password on the notification bell
    # topic : the product that is bought
    # clientid : this must be unique else the connection would be lost

    clientId = 'Default'
    account = 'Default'
    topic = ['Default']
    pw = 'Default'
    port = 1883
    host = 'Default'

    try:
        if os.path.exists('config.ini'):
            fread = open('config.ini', 'r')
            host = str(fread.read()).split("=")[1]
            print "Host :", host
            fread.close()
        if host == 'Default' or port == 'Default' or topic == 'Default' or account == 'Default' or clientId == 'Default':
            print "ERROR: Check host, topic, subscriber and password values"
            print "The subscriber is the username that was used to purchase the product"
            print "The topic is the product which is purchased from the I3 Data market place"
            print "The password is the system generated password when the product is purchased"
            raise Exception(" Default values not changed ")
        pub_client = mqtt.Client(clientId)
        pub_client.on_connect = on_connect
        pub_client.on_message = on_message
        pub_client.username_pw_set(account, pw)
        pub_client.connect(host, port)      #connect to broker
    
    except Exception as e:
        print "Exception" + str(e)
        exit(0)
    #pub_client.subscribe(topic)
    #pub_client.loop_start()

    url='https://data.lacity.org/resource/7fvc-faax.json?$where=date_rptd>"'
    url = url + (datetime.datetime.now() - timedelta(days=7)).isoformat() + '"'
    json_data=requests.get(url,verify=False).json()   #Gets data from SODA API

    count = 0
    while count < 2:
        count += 1
        pub_client.publish(topic[0], json.dumps(json_data))
 ##       pub_client.publish(topic[1], 'Hello World')
        time.sleep(2)

    pub_client.disconnect()


