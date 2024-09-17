#include "main_task/main_task.h"
#include "gps.h"

#define MINIMUM_SPEED 0.01

extern struct Position position;
extern double speedGPS;

double distanceGPS = 0.00f;

void main_task(__unused void *params)
{
    ble_init();
    gps_power(1);
    gps_configure();

    while (true)
    {

        if (speedGPS > MINIMUM_SPEED){
            distanceGPS += calc_distance(position.longtitude, position.latitude);
        //  dataDisp.distance+=0.1;
        }
        taskYIELD();
    }
}