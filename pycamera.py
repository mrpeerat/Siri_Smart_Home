from picamera import PiCamera
from time import sleep
import paho.mqtt.client as mqtt

def on_connect(client, userdata, flags, rc):
	print("Connected with result code "+str(rc))

    # Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.
	client.subscribe("Camera_1")
def on_message(client, userdata, msg):
	print(msg.topic+" "+str(msg.payload))
	message = str(msg.payload)
	print(message)
	if(message == "take"):
		print("camera1 take picture")
        	sleep(0.1)
		camera.capture('/home/pi/capture.jpg')
		client.publish("C1/confirm","success")

camera = PiCamera()
camera.brightness = 55
camera.resolution = (2592, 1944)
print("Camera Setup Complete")

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message
client.connect("www.jamesmooth.win")
print("Mqtt setup complete")

client.loop_forever()




