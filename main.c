
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>
#include <stdlib.h>
#include <libpq-fe.h>

#include "arduino-serial/arduino-serial-lib.c"

char AzimuthData[] = "3685:\0";
char ElevationData[] = "1834:\0" ;
char sensorData[20];
int main(int argc, char const *argv[]) {

  int fd = serialport_init("/dev/ttyACM0", 9600);

  while(1) {
     int bytesSent_1 = serialport_write(fd, AzimuthData);
     if(bytesSent_1 == -1) {
        printf("Error: Azimuth Data failed to send!\n" );
      } else {
        printf("Azimuth : %s sent\n", AzimuthData);
      }
      int bytesSent_2 = serialport_write(fd, ElevationData);
      if(bytesSent_2 == -1) {
         printf("Error: Elevation Data failed to send!\n" );
      } else {
        printf("Elevation : %s sent\n", ElevationData);
      }
     int bytesReceived = serialport_read_until(fd, sensorData, ':', 20, 2000);
     if (bytesReceived == -1){
       printf("Error: Serial Read function failed!\n");

     }

     printf("From Arduino Serial Debug : %d, %d\n", sensorData[0], sensorData[1]);

     }



  return 0;
}
