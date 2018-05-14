#!python3
import socket
import threading
import json

bind_ip = '10.192.91.11'
bind_port = 9999

server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.bind((bind_ip, bind_port))
server.listen(5)  # max backlog of connections

print ('Listening on {}:{}'.format(bind_ip, bind_port))
map=[][]

def initialize(mapCassie):
	map= mapCassie

def updateMap(mapDave):
	for i in range(mapDave.shape[0]):
		for j in range(mapDave.shape[1]):
			#0 is undetected ,1 is obstacle , 2 is free
			if(map[i][j]!=mapDave[i][j]):

				if(map[i][j]== 1 || mapDave[i][j]==1):
					map[i][j] = 1
				if(map[i][j]=0):
					map[i][j]=mapDave[i][j]


def handle_client_connection(client_socket):
    while True:
        request = client_socket.recv(1024)
        result = json.loads(request.decode('unicode-escape').strip('"'))
		mapCassie = pd.read_json(result, orient='split')
		mapCassie = np.array(mapCassie)
		updateMap(mapCassie)
        #print ('Received from: {}'.format(result['name']))
        #print ('Received: {}'.format(result['test']))
        #print ('')

def send_something(client_socket,map):
    while True:
        #user_input = input('Type something here in the console to send it to CASSIE \n')
        #print ('')
        #str = '{"test": "%s", "name": "DAVE"}' % user_input
		map = np.array(map)
		str = pd.DataFrame(your_array).to_json('data.json', orient='split')
        jason = json.dumps(str)
        client_socket.sendall(jason.encode())

#initialize([[1,1],[1,1]])
while True:
    client_sock, address = server.accept()
    print ('Accepted connection from {}:{}'.format(address[0], address[1]))
    client_handler = threading.Thread(
        target=handle_client_connection,
        args=(client_sock,)  # without comma you'd get a... TypeError: handle_client_connection() argument after * must be a sequence, not _socketobject
    )
    client_handler.start() #//I moved this to a new method to work better with asyncio???
    client_sender = threading.Thread(
        target=send_something,
        args=(client_sock,)  # without comma you'd get a... TypeError: handle_client_connection() argument after * must be a sequence, not _socketobject
    )
    client_sender.start()
