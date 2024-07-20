#ifndef GPS_UBLOX_H
#define GPS_UBLOX_H

#include "uart_config.h"
#include <math.h>
#include <stdlib.h>


#define pi 3.14159265358979323846

#define TIME_RMC 1
#define STATUS_RMC 2 // A - sucsess V - bad
#define LATITUDE_RMC 3
#define LONGTITUDE_RMC 5

struct Time
{
    uint8_t hours;
    uint8_t minutes;
    uint8_t seconds;
};

struct Position
{
    uint8_t status;
    double latitude;
    double longtitude;
    double speed;
};

void gps_init();
bool checkCRC(const uint8_t *str);
void nmea_parcer(uint8_t *str);
void parse_RMC(uint8_t *data);
void parse_VTG(uint8_t *data);

double calc_distance(double lat1, double lon1, double lat2, double lon2);
static double deg2rad(double);
static double rad2deg(double);

#endif