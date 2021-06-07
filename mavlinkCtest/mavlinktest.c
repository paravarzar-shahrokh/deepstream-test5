#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>

#include <termios.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <fcntl.h>
int fd ;/*File Descriptor*/
int latlocal  = 10000000;
int lonlocal  = -10000000;
int altlocal  = 1000000;
int bytes_received = 0;
char recv_chr;
int mavSerialInput;

#if (defined __QNX__) | (defined __QNXNTO__)
/* QNX specific headers */
#include <unix.h>
#else
/* Linux / MacOS POSIX timer headers */
#include <sys/time.h>
#include <time.h>
#include <arpa/inet.h>
#include <stdbool.h> /* required for the definition of bool in C99 */
#endif
/* 0: Include MAVLink types */
#include "../c_library_v2/mavlink_types.h"
#include "../c_library_v2/common/mavlink.h"
#define SERIAL_PORT         "/dev/ttyTHS0"    /* Change /dev/ttyUSB0 to the one corresponding to your system */
#define SERIAL_BAUD         B57600
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

int main (int argc, char *argv[])
{
  /* --------------------------------------------
            Mavlink - Open serial port 
  ---------------------------------------------- */
  printf("\n +----------------------------------+");
  printf("\n |    Serial Port MAVLink Receive   |");
  printf("\n +----------------------------------+");
  /*------------------------------- Opening the Serial Port -------------------------------*/
  /* O_RDWR   - Read/Write access to serial port        */
  /* O_NOCTTY - No terminal will control the process    */
  /* O_NDELAY - Non Blocking Mode, do not care about the status of DCD line,open() returns immediatly */ 
  fd = open(SERIAL_PORT, O_RDWR | O_NOCTTY | O_NDELAY);	
                                                  
  if(fd == -1){						/* Error Checking */
      printf("\n the fd value is %d",fd);
      printf("\n  Error in opening %s \n", SERIAL_PORT);
      exit(-1);
    }

  if(fd > -1) {
      printf("\n  Open %s successfully \n", SERIAL_PORT);
  }
mavlink_message_t msg;
mavlink_status_t status;
mavlink_global_position_int_t global_position; 

printf("Mavlink Serial Value %d \n", mavSerialInput);

while (1){
    mavSerialInput = read(fd, &recv_chr, 1);
    if( mavSerialInput==1) bytes_received++;
    if (mavlink_parse_char(MAVLINK_COMM_0, recv_chr, &msg, &status))
        {
            // Packet received
			printf("\nReceived packet: SYS: %d, COMP: %d, LEN: %d, MSG ID: %d\n", msg.sysid, msg.compid, msg.len, msg.msgid);
            mavlink_msg_global_position_int_decode(&msg, &global_position);
                latlocal  = global_position.lat;
                lonlocal  = global_position.lon;
                altlocal  = global_position.alt;
                printf(ANSI_COLOR_RED "lat: %d" ANSI_COLOR_RESET "\n" ,latlocal );
        } 
}
return 0;
}