import serial


ser = serial.Serial('/dev/ttyACM0', 115200)

print(ser.name)

for x in range(0,127):
	#temp = '!go/'+str(x) + '/0'
	ser.write('!go/%d/0' % x)

ser.write('stop')
