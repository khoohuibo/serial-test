
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>
#include <stdlib.h>
#include <libpq-fe.h>

#include "arduino-serial/arduino-serial-lib.h"
#include "config.h"


char AzimuthData[100];
char ElevationData[100];
char sensorData[2000];


void recieve_azi_el(PGconn *conn)
{
    PGresult *res = PQexec(conn
        , "SELECT schedule.time, spacecraft.name, round(degrees(observations.azimuth)), round(degrees(observations.elevation)), spacecraft.frequency_downlink-(spacecraft.frequency_downlink*(observations.relative_velocity/3e8)) AS downlink, spacecraft.frequency_uplink-(spacecraft.frequency_uplink*(observations.relative_velocity/3e8)) AS uplink FROM (SELECT * FROM schedule WHERE time > NOW() ORDER BY time LIMIT 1) AS schedule INNER JOIN observations ON schedule.spacecraft=observations.spacecraft AND schedule.time=observations.time INNER JOIN spacecraft ON schedule.spacecraft=spacecraft.id;");
    if(PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        printf("Error: Next Point Select query failed!\n");
        printf("%s\n", PQresStatus(PQresultStatus(res)));
        printf("%s\n", PQresultErrorMessage(res));
        return;
    }

    int rows = PQntuples(res);
    if(rows == 0)
    {
        printf("Error: No Next point found\n");
        PQclear(res);
        return;
    }
    printf("%s: %d, %d, [%s] Downlink: %.6f MHz, Uplink: %.6f MHz\n"
        , PQgetvalue(res,0,0)
        , atoi(PQgetvalue(res,0,2))
        , atoi(PQgetvalue(res,0,3))
        , PQgetvalue(res,0,1)
        , atof(PQgetvalue(res,0,4))
        , atof(PQgetvalue(res,0,5))
    );
    strncpy(AzimuthData, PQgetvalue(res,0,2), 100);
    strncpy(ElevationData, PQgetvalue(res,0,3), 100);
    PQclear(res);
}

int main(int argc, char const *argv[]) {

  fprintf(stdout,
      "PSQL to Arduino test\n"
      "Hubert Khoo 2018\n"
  );

  char config_filename[100];
  strncpy(config_filename, "config.ini", 100);

  printf("Loading config from file: %s\n", config_filename);
  if(!config_load(&config, config_filename))
  {
      printf("Error loading config from file! Exiting..\n");
      return 1;
  }

  PGconn *conn = PQconnectdb(config.db_conn_string);

  if (PQstatus(conn) == CONNECTION_BAD) {

      fprintf(stderr, "Connection to database failed: %s\n",
          PQerrorMessage(conn));
      PQfinish(conn);
      return 1;
  }

  printf("Connected to Server (server: %d, client: %d)\n"
      , PQserverVersion(conn), PQlibVersion()
  );


  int fd = serialport_init("/dev/ttyACM0", 9600);

  while(1) {
    recieve_azi_el(conn);
    sleep(5);

    int bytesSent_1 = serialport_write(fd, AzimuthData);
     if(bytesSent_1 == -1) {
        printf("Error: Azimuth Data failed to send!\n" );
      } else {
        printf("Azimuth : %s sent\n", AzimuthData);
      }
    int bytesReceived_1 = serialport_read_until(fd, sensorData, ':', 2000, 200);
    if (bytesReceived_1 == -1){
        printf("Error: Serial Read function failed!\n");

      }

    printf("From Arduino Serial Debug : %s\n", sensorData);
    sleep(3);

    int bytesSent_2 = serialport_write(fd, ElevationData);
    if(bytesSent_2 == -1) {
         printf("Error: Elevation Data failed to send!\n" );
      }
    else {
      printf("Elevation : %s sent\n", ElevationData);
      }
    int bytesReceived_2 = serialport_read_until(fd, sensorData, ':', 2000, 200);
    if (bytesReceived_2 == -1){
      printf("Error: Serial Read function failed!\n");
    }

    printf("From Arduino Serial Debug : %s\n", sensorData);

    sleep(3);

    }



  return 0;
}
