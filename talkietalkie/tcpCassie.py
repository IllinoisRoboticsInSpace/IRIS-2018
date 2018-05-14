#!python3
import socket
import json
import threading
import pandas as pd
import numpy as np

hostname, sld, tld, port = 'www', 'integralist', 'co.uk', 80
target = '{}.{}.{}'.format(hostname, sld, tld)

# create an ipv4 (AF_INET) socket object using the tcp protocol (SOCK_STREAM)
client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
map=[][]


def initialize(mapDave):
	map= mapDave

def updateMap(mapDave):
	for i in range(mapDave.shape[0]):
		for j in range(mapDave.shape[1]):
			#0 is undetected ,1 is obstacle , 2 is free
			if(map[i][j]!=mapDave[i][j]):

				if(map[i][j]== 1 || mapDave[i][j]==1):
					map[i][j] = 1
				if(map[i][j]=0):
					map[i][j]=mapDave[i][j]




def receive_messages():
    # receive the response data (4096 is recommended buffer size)
    while True:
        response = client.recv(4096)
        result = json.loads(response.decode('unicode-escape').strip('"'))
		mapDave = pd.read_json(result, orient='split')
		mapDave = np.array(mapDave)
		updateMap(mapDave)
        #print ('Received from: %s' % result['name'])
        #print ('Received: %s' % result['test'])

        #print ('')

def send_messages(map): #Add map parameter later, but probably not
    while True:
        # send some data (in this case a HTTP GET request)
        #user_input = input('Type something here in the console to send it to DAVE \n')
        #print ('')
		map = np.array(map)
		#map = np.array([[1,0,0],[0,1,0],[0,0,1]]) #Test identity array to send
		str = pd.DataFrame(your_array).to_json( orient='split')

        #str = '{"ar[0][0]": "%s", "name": "CASSIE"}' % user_input
        jason = json.dumps(str)
        client.sendall(jason.encode())


# connect the client
# client.connect((target, port))
initialize([[1,1],[1,1]])
client.connect(('10.192.91.11', 9999))
print ('Connected!')

server_handler = threading.Thread(
    target=receive_messages
)
server_sender = threading.Thread(
    target=send_messages
)
server_handler.start()
server_sender.start()
