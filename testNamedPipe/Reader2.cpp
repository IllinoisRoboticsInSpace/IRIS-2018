#include <sys/types.h>
#include <sys/select.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <string>
#include <iostream>

using namespace std;

/* Currently, I don't completely understand how the reading of the named pipe works
   It seems that sometimes some data is skipped and I think the "buffer" takes in 
   the data but never prints it out... */

main()  {
 int fd; 
 fd_set set_a;

 char * myfifo = new char [12];
 strcpy(myfifo, "./tmp/myfifo1");
 char buffer[1024];

 /* Open the file and return if there is an error. */
 fd = open("./tmp/myfifo1", O_RDONLY | O_NONBLOCK);
 if (fd == -1) {
     perror("open");
     return EXIT_FAILURE;
 } 
 ssize_t bytes;

 size_t total_bytes = 0;
 printf("\nDropped into read pipe\n");
 
 /* While true, read from named pipe and print it out. */
 while(1){
     if((bytes = read(fd, buffer, sizeof(buffer))) > 0){
         std::string message(&buffer[0]);
         total_bytes += (size_t)bytes;
	 
         /* Print our continual running total of bytes. */
	 printf("\Reading now at (%d bytes)\n", (int)total_bytes);

         printf("Message: %s\n", message.c_str());
         memset(&buffer[0], 0, sizeof(buffer));
         //sleep(3);
     }else{
         /* If there is no message in the named pipe, print "no message" */
         /*if (errno == EWOULDBLOCK) {
             printf("\ndone reading (%d bytes)\n", (int)total_bytes);
             break;
         }*/
         //printf("No message\n");
         //sleep(2);
     } 
 }
 return EXIT_SUCCESS;
}
