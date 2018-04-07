import serial


ser = serial.Serial('/dev/tty.usbserial')

print(ser.name)

if(ser.isOpen()):
	ser.write(b'!go/100/0!\n')
	print('NIPPLE')

#for x in range(0,127):
	#temp = '!go/'+str(x) + '/0'
	#ser.write('!go/%d/0!' % x)
	

#ser.write('stop')
