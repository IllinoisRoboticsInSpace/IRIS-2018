#!python3
import socket
import json
import threading

hostname, sld, tld, port = 'www', 'integralist', 'co.uk', 80
target = '{}.{}.{}'.format(hostname, sld, tld)

# create an ipv4 (AF_INET) socket object using the tcp protocol (SOCK_STREAM)
client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)


def receive_messages():
    # receive the response data (4096 is recommended buffer size)
    while True:
        response = client.recv(4096)
        result = json.loads(response.decode('unicode-escape').strip('"'))
        print ('Received from: %s' % result['name'])
        print ('Received: %s' % result['test'])
        print ('')

def send_messages():
    while True:
        # send some data (in this case a HTTP GET request)
        user_input = input('Type something here in the console to send it to DAVE \n')
        print ('')
        str = '{"test": "%s", "name": "CASSIE"}' % user_input
        jason = json.dumps(str)
        client.sendall(jason.encode())


# connect the client
# client.connect((target, port))
client.connect(('192.168.7.105', 9999))
print ('Connected!')

server_handler = threading.Thread(
    target=receive_messages
)
server_sender = threading.Thread(
    target=send_messages
)
server_handler.start()
server_sender.start()
