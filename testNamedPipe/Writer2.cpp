#include <sys/types.h>
#include <sys/select.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <string>

using namespace std;

/* Named pipes are  */

main()  {
 int fd;
 char * myfifo = new char [12];
 strcpy(myfifo, "./tmp/myfifo1");

 int N(1000);
 float writer[N];

 /* create the FIFO (named pipe) FIFO is First In First Out*/
 mkfifo(myfifo, 0666);
 
 fd = open("./tmp/myfifo1", O_WRONLY ); //open(myfifo, O_WRONLY | O_NONBLOCK);
 /* return if there is a failure opening the file */
 if (fd == -1) {
     perror("open");
     return EXIT_FAILURE;
 } 
 printf("File open\n");
 

 int idx = 1;
 /* test by writing integers from 1->1000 to the named pipe waiting 1 second between each. */
 while( idx <= 1000 ) {
   /* write to the named pipe (fd) telling the function what to write and the size. */
   write(fd, &idx, sizeof(int));
   sleep(1);
   idx++;
 }

 /* Write messages to the named pipe and wait between each write. */
 /*for(int i = 0; i < 10; i++) {
 write(fd, "entry [1]", sizeof("entry [1]"));
 sleep(1);
 write(fd, "entry [2]", sizeof("entry [2]"));
 sleep(1);
 write(fd, "entry [3]", sizeof("entry [3]"));
 sleep(1);
 }*/
 printf("Content written\n");
 
 /* Close the file connection*/
 close(fd);
 printf("Connection closed\n");
 sleep(10);
 /* remove the FIFO */
 unlink(myfifo);
 return 0;
}
