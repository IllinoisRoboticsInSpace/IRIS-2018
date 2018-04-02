import cv2
import numpy as np

cap = cv2.VideoCapture(1)

def returnFrame():
	ret, frame = cap.read()
	hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
	lower_red = np.array([110,50,50])
	upper_red = np.array([130, 255, 255])

	#lower_black = np.array([110,50,50])
	#upper_black = np.array([130, 255, 255])
	
	#orange = np.uint8([[[255,144,69]]])
	#print( cv2.cvtColor(upper_red, cv2.COLOR_HSV2BGR))
	#hsv_orange = cv2.cvtColor(black, cv2.COLOR_BGR2HSV)

	#print hsv_orange

	masko = cv2.inRange(hsv,np.array([0,50,50]),np.array([3,255,255]))
	#masku = cv2.inRange(hsv, np.array([98,50,50]), np.array([118,255,255]))
	masku = cv2.inRange(hsv, np.array([100,45,100]), np.array([150,255,255]))
	
	resb = cv2.bitwise_and(frame, frame, mask=masku)
	reso = cv2.bitwise_and(frame,frame,mask=masko)

	return resb
	#cv2.imshow('frame', frame)
	#cv2.imshow('mask', masku)
	#cv2.imshow('reso', reso)
	#cv2.imshow('resb', resb)
	#if(cv2.waitKey(1) & 0xFF == ord('q')):
	#	break


