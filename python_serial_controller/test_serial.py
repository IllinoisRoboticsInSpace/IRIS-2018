import serial
import sys
import time
ser=serial.Serial('/dev/ttyACM0',9600, timeout=None)
ser.flushInput()
#char = raw_input("Enter String : ")
time.sleep(1)
stop = "stop#!"

for x in range(0,90):
	go = "go/0/%d#!" % (x*4%360)
	ser.write(go)

ser.write("turn/80/0#!")

ser.write(stop)

#ser.write('go/%d/0#!' %(150))
#for x in range(0,500):
#	ser.write('go/%d/0#!' %(-150))
	#time.sleep(.005)

#ser.write("stop//#!")
#received =[]
#for n in range(0, len(char)):
     #char_rec = ser.readline()
     #received.append(char_rec[0:len(char_rec)-2]) # remove \r\n characters
#sys.stdout.write(''.join(received)+'\n')
ser.close()
