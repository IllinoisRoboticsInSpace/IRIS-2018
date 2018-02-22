all: test
CXX=g++
CFLAGS=-fPIC -g -Wall  `pkg-config --cflags opencv` `pkg-config --cflags libusb-1.0` 
LIBS = `pkg-config --libs opencv` 

INCLUDE = -I/usr/local/include/libfreenect
FREE_LIBS = -L/usr/local/lib -lfreenect -lusb-1.0 -l pthread


test: main.cpp kinect.cpp
	$(CXX) $(CFLAGS) $? -o $@  $(LIBS) $(FREE_LIBS) $(INCLUDE)

%.o: %.cpp
	$(CXX) -c $(CFLAGS) $< -o $@

clean:
	rm -rf *.o test
