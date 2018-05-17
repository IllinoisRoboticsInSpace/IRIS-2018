/*
This is supposed to be a basic h++ file with basic tcp send code and basic tcp receive code
*/

#ifndef TCP_BASIC
#define TCP_BASIC

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <iostream>
#include <arpa/inet.h>

#include "map.hpp"

#define PORT "3490" // the port client will be connecting to
#define MAXDATASIZE 100 // max number of bytes we can get at once

/*
Sends data over TCP in the specified socket in the parameter
*/
void sendTcp() {
	buf[12] = '\0';
    int height = 0;
    int width = 0;
    for(;height<3; height++) {
		for(width=0;width<4; width++)
		{
            map[height][width]= buf[height*4 + width]-48;
        }
    }
}

/*
Receies data over TCP in the specified socket in the parameter
*/
matrix_tag receiveTCP() {
	buf[12] = '\0';
    int height = 0;
    int width = 0;
    for(;height<3; height++) {
		for(width=0;width<4; width++)
		{
            map[height][width]= buf[height*4 + width]-48;
        }
    }
}

#endif