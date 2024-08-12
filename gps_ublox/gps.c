#include "gps.h"

static const uint8_t set5hz[] = {0xB5, 0x62, 0x06, 0x08, 0x06, 0x00, 0xC8, 0x00, 0x01, 0x00, 0x01, 0x00, 0xDE, 0x6A}; //(5Hz)

struct Position pos;
struct Time timeGPS;

void gps_init()
{
    uart_configure();
    gpio_deinit(5);
    gpio_init(5);
    gpio_set_dir(5, GPIO_OUT);
    // sleep_ms(1000);
    // uart_write_blocking(UART_ID, set5hz, sizeof set5hz);
}

void gps_on(bool state)
{
    if (state)
        gpio_put(5, 1);
    else
        gpio_put(5, 0);
}
bool checkCRC(const uint8_t *str)
{
    uint8_t lenght = strlen((const char *)str);
    if (lenght < 8)
        return 0;
    if (str[0] != '$' || str[lenght - 5] != '*')
        return 0;

    uint8_t checksum = 0;
    for (uint8_t i = 1; i < (lenght - 5); i++)
    {
        checksum ^= str[i];
    }
    char crc[] = {str[lenght - 4], str[lenght - 3]};
    int crcMSG = (int)strtol(crc, NULL, 16);
    if (crcMSG == checksum)
        return 1;
    return 0;
}

void nmea_parcer(uint8_t *str)
{
    if (!checkCRC(str))
        return;
    if (strstr((char *)str, "RMC") != NULL)
    {
        parse_RMC(str);
    }
    if (strstr((char *)str, "VTG") != NULL)
    {
        parse_VTG(str);
    }
}

void parse_RMC(uint8_t *data)
{
    uint8_t count_ch = 0;
    uint8_t num = 0;
    uint8_t i = 0;
    char buff[14];
    memset(buff, '\0', 14);

    while (data[count_ch - 1] != '\n')
    {
        if (data[count_ch] == ',' || data[count_ch] == '*')
        {
            if (buff[0] != '\0')
            {
                switch (num)
                {
                case TIME_RMC:
                {
                    char ch[] = {buff[0], buff[1]};
                    timeGPS.hours = atoi(ch);
                    timeGPS.hours = timeGPS.hours > 20 ? timeGPS.hours - 21 : timeGPS.hours + 3;
                    ch[0] = buff[2];
                    ch[1] = buff[3];
                    timeGPS.minutes = atoi(ch);
                    ch[0] = buff[4];
                    ch[1] = buff[5];
                    timeGPS.seconds = atoi(ch);
                    break;
                }
                case STATUS_RMC:
                {
                    if (buff[0] == 'A')
                    {
                        pos.status = 1;
                    }
                    else
                    {
                        pos.speed = 0;
                        pos.status = 0;
                    }
                    break;
                }
                case LATITUDE_RMC:
                {
                    if (pos.status)
                    {
                        char *p2 = strchr(buff, '.');
                        double latitude = atof(p2 - 2) / 60.0;
                        buff[p2 - 2 - buff] = '\0';
                        latitude += atof(buff);
                        pos.latitude = latitude;
                    }
                    break;
                }
                case 4:
                    if (buff[0] == 'S')
                    {
                        pos.longtitude = -pos.longtitude;
                    }
                    break;
                case LONGTITUDE_RMC:
                    if (pos.status)
                    {
                        char *p2 = strchr(buff, '.');
                        double longtitude = atof(p2 - 2) / 60.0;
                        buff[p2 - 2 - buff] = '\0';
                        longtitude += atof(buff);
                        pos.longtitude = longtitude;
                    }

                    break;

                case 6:
                    if (buff[0] == 'W')
                    {
                        pos.longtitude = -pos.longtitude;
                    }
                    break;
                }
            }
            i = 0;
            num++;
            memset(buff, '\0', 14);
        }
        if (data[count_ch] != ',')
        {
            buff[i] = data[count_ch];
            i++;
        }
        count_ch++;
    }
}

void parse_VTG(uint8_t *data)
{
    uint8_t count_ch = 0;
    uint8_t num = 0;
    uint8_t i = 0;
    char buff[10];
    memset(buff, '\0', 10);

    while (data[count_ch - 1] != '\n')
    {

        if (data[count_ch] == ',' || data[count_ch] == '*')
        {
            if (buff[0] != '\0')
            {
                switch (num)
                {
                case 7:
                {
                    if (buff[0] != '\0')
                        pos.speed = atof(buff);
                    else
                        pos.speed = 0;
                    break;
                }
                }
            }
            i = 0;
            num++;
            memset(buff, '\0', 10);
        }
        if (data[count_ch] != ',')
        {
            buff[i] = data[count_ch];
            i++;
        }
        count_ch++;
    }
}

double calc_distance()
{
    static double longtitude = 0;
    static double latitude = 0;

    if (latitude == 0 && latitude == 0)
    {
        longtitude = pos.longtitude;
        latitude = pos.latitude;
        return 0;
    }

    if ((pos.latitude == 0) || (pos.longtitude == 0))
        return 0;

    if ((latitude == pos.latitude) && (longtitude == pos.longtitude))
    {
        return 0;
    }
    else
    {
        double theta;
        double dist = 0.01f;
        theta = longtitude - pos.longtitude;
        dist = sin(deg2rad(latitude)) * sin(deg2rad(pos.latitude)) + cos(deg2rad(latitude)) * cos(deg2rad(pos.latitude)) * cos(deg2rad(theta));
        dist = acos(dist);
        dist = rad2deg(dist);
        dist = dist * 60 * 1.1515 * 1.609344;

        if (isinf(dist) == 0)
        {
            longtitude = pos.longtitude;
            latitude = pos.latitude;
            return dist;
        }

        else
            return 0;
    }
}

double deg2rad(double deg)
{
    return (deg * pi / 180);
}

double rad2deg(double rad)
{
    return (rad * 180 / pi);
}
