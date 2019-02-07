
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>
#include <stdlib.h>
#include <libpq-fe.h>

#include "arduino-serial/arduino-serial-lib.h"
#include "config.h"


char AzimuthData[30];
char ElevationData[30];
char sensorData[2000];
char YaesuBuffer [100];
char CommandString [100];
char input_ID[20];

void display_spacecraft(PGconn *conn)
{
  PGresult *res = PQexec(conn,"SELECT id, name FROM spacecraft");
  if(PQresultStatus(res) != PGRES_TUPLES_OK)
  {
      printf("Error: Display Spacecraft query failed!\n");
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
  PQprintOpt	options = {0};

  options.header = 1;
  options.align = 1;
  options.fieldSep = "|";

  PQprint(stdout, res, &options);
  PQclear(res);

}

void recieve_azi_el(PGconn *conn, const char *input_ID)
{
    snprintf(CommandString, 100, "SELECT schedule.time, spacecraft.name, round(degrees(observations.azimuth)), round(degrees(observations.elevation)), spacecraft.frequency_downlink-(spacecraft.frequency_downlink*(observations.relative_velocity/3e8)) AS downlink, spacecraft.frequency_uplink-(spacecraft.frequency_uplink*(observations.relative_velocity/3e8)) AS uplink FROM (SELECT * FROM schedule WHERE time > NOW() AND spacecraft = %s ORDER BY time LIMIT 1) AS schedule INNER JOIN observations ON schedule.spacecraft=observations.spacecraft AND schedule.time=observations.time INNER JOIN spacecraft ON schedule.spacecraft=spacecraft.id;", input_ID);
    printf("%s\n", CommandString );
    PGresult *res = PQexec(conn, CommandString);

    PQprintOpt	options = {0};


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
    options.header = 1;
    options.align = 1;
    options.fieldSep = "|";

    printf("%s: %d, %d, [%s] Downlink: %.6f MHz, Uplink: %.6f MHz\n"
        , PQgetvalue(res,0,0)
        , atoi(PQgetvalue(res,0,2))
        , atoi(PQgetvalue(res,0,3))
        , PQgetvalue(res,0,1)
        , atof(PQgetvalue(res,0,4))
        , atof(PQgetvalue(res,0,5))
    );
    PQprint(stdout, res, &options);
    strncpy(AzimuthData, PQgetvalue(res,0,2), 3);
    strncpy(ElevationData, PQgetvalue(res,0,3), 3);
    PQclear(res);
}


void yaesustringformat(const char *str , const char *str2){
  char return_string_1[30];
  char return_string_2[30];
  int len = strlen(str);
  int len2 = strlen(str2);
  if (!(len == 3)){
    if (len == 2){
      strcpy(return_string_1, "0");
      strcat(return_string_1, AzimuthData);
    } if (len == 1){
      strcpy(return_string_1, "00");
      strcat(return_string_1, AzimuthData);
    }
  } else {
      strcpy(return_string_1, AzimuthData);
      printf("AzimuthData has %d digits\n", len);
    }
  if (!(len2 == 3)){
    if (len2 == 2){
      strcpy(return_string_2, "0");
      strcat(return_string_2, ElevationData);
    }
    if (len2 == 1){
      strcpy(return_string_2, "00");
      strcat(return_string_2, ElevationData);
    }else {
      strcpy(return_string_2, ElevationData);
      printf("ElevationData has %d digits\n", len2);
    }

  }
  printf("%s\n", return_string_1 );
  printf("%s\n", return_string_2 );
  snprintf(YaesuBuffer, 100, "W%s %s\r\n", return_string_1, return_string_2);
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
  display_spacecraft(conn);

  printf("Enter ID to track: ");
  scanf("%[^\n]", &input_ID);



  int fd = serialport_init("/dev/ttyUSB0", 9600);

  while(1) {
    recieve_azi_el(conn, input_ID);
    yaesustringformat(AzimuthData, ElevationData);
    sleep(5);

    int bytesSent_1 = serialport_write(fd, YaesuBuffer);
     if(bytesSent_1 == -1) {
        printf("Error: YaesuBuffer failed to send!\n" );
      } else {
        printf("YaesuBuffer : %s sent\n", YaesuBuffer);
      }

    sleep(5);
    int bytesReceived_1 = serialport_read_until(fd, sensorData, ':', 2000, 200);
    if (bytesReceived_1 == -1){
        printf("Error: Serial Read function failed!\n");

      }

    printf("From Arduino Serial Debug : %s\n", sensorData);
    sleep(3);
    memset(sensorData, 0, sizeof(sensorData));
    }



  return 0;
}
