# USAGE
# python distance_to_camera.py

# import the necessary packages
import numpy as np
import cv2
import sys
import getblue as GetB
def find_marker(image):
	# convert the image to grayscale, blur it, and detect edges
	gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)	
	#gray = cv2.GaussianBlur(gray, (5, 5), 0)
	gray = cv2.medianBlur(image,33)	
	#gray = cv2.bilateralFilter(image,9,75,75)
	edged = cv2.Canny(gray, 35, 125)

	# find the contours in the edged image and keep the largest one;
	# we'll assume that this is our piece of paper in the image
	(cnts, _) = cv2.findContours(edged.copy(), cv2.RETR_LIST, cv2.CHAIN_APPROX_SIMPLE)
	try:	
		c = max(cnts, key = cv2.contourArea)
		# compute the bounding box of the of the paper region and return it
		return cv2.minAreaRect(c)
	except:
		return 0
def distance_to_camera(knownWidth, focalLength, perWidth):
	# compute and return the distance from the maker to the camera
	return (knownWidth * focalLength) / perWidth

# initialize the known distance from the camera to the object, which
# in this case is 24 inches
KNOWN_DISTANCE = 36.0

# initialize the known object width, which in this case, the piece of
# paper is 12 inches wide
KNOWN_HEIGHT = 12.5

#focalLength = 928.170684814



# initialize the list of images that we'll be using
IMAGE_PATHS = ["images/3ft.png", "images/55in.png", "images/77in.png"]
height=0
# load the furst image that contains an object that is KNOWN TO BE 2 feet
# from our camera, then find the paper marker in the image, and initialize
# the focal length
image = cv2.imread(IMAGE_PATHS[0])
marker = find_marker(GetB.UseImage(image))
print marker[1][1]
height= max(marker[1][0],marker[1][1])
focalLength = (height * KNOWN_DISTANCE) / KNOWN_HEIGHT
#print focalLength
# loop over the images
"""for imagePath in IMAGE_PATHS:
	# load the image, find the marker in the image, then compute the
	# distance to the marker from the camera
	image = cv2.imread(imagePath)
	marker = find_marker(image)
	inches = distance_to_camera(KNOWN_WIDTH, focalLength, marker[1][0])

	# draw a bounding box around the image and display it
	box = np.int0(cv2.cv.BoxPoints(marker))
	cv2.drawContours(image, [box], -1, (0, 255, 0), 2)
	cv2.putText(image, "%.2fft" % (inches / 12),
		(image.shape[1] - 200, image.shape[0] - 20), cv2.FONT_HERSHEY_SIMPLEX,
		2.0, (0, 255, 0), 3)
	cv2.imshow("image", cv2.resize(image, (0,0), fx=0.5, fy=0.5))
	cv2.waitKey(0)
"""

#cap = cv2.VideoCapture(1)

while(True):
	#ret, image = cap.read()
	#if not ret:
	#	print("Unable to get image from webcam. Exiting")
	#	sys.exit(1)
	image= GetB.returnFrame()

	# load the image, find the marker in the image, then compute the
	# distance to the marker from the camera
	#image = cv2.imread(imagePath)
	marker = find_marker(image)
	if marker==0:
		continue
	height=max(marker[1][0],marker[1][1])	
	inches = distance_to_camera(KNOWN_HEIGHT, focalLength, height)

	# draw a bounding box around the image and display it
	box = np.int0(cv2.cv.BoxPoints(marker))
	print(max(marker[1][0],marker[1][1]))
	cv2.drawContours(image, [box], -1, (0, 255, 0), 2)
	#if (inches/12 >6):
	#	inches= inches- 0.013484158*(inches/12)*(inches/12) - 0.0222399 * (inches/12) - 0.56889 
	cv2.putText(image, "%.2fft" % (inches / 12 ),
		(image.shape[1] - 200, image.shape[0] - 20), cv2.FONT_HERSHEY_SIMPLEX,
		2.0, (0, 255, 0), 3)
	#cv2.resize(image, (0,0), fx=0.5, fy=0.5)
	cv2.imshow("image", image)
	#cv2.waitKey(0)
	#gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
	#cv2.imshow('frame', gray)
	if(cv2.waitKey(1) & 0xFF == ord('q')):
		break
cap.release()
cv2.destroyAllWindows()

