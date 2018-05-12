#! python3
import socket
import threading
import json

bind_ip = '192.168.7.105'
bind_port = 9999

server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.bind((bind_ip, bind_port))
server.listen(5)  # max backlog of connections

print ('Listening on {}:{}'.format(bind_ip, bind_port))


def handle_client_connection(client_socket):
    while True:
        request = client_socket.recv(1024)
        result = json.loads(request.decode('unicode-escape').strip('"'))
        print ('Received from: {}'.format(result['name']))
        print ('Received: {}'.format(result['test']))
        print ('')

def send_something(client_socket):
    while True:
        user_input = input('Type something here in the console to send it to CASSIE \n')
        print ('')
        str = '{"test": "%s", "name": "DAVE"}' % user_input
        jason = json.dumps(str)
        client_socket.sendall(jason.encode())

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
