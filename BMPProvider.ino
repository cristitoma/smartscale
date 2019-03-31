#include "BMPProvider.h"

SFE_BMP180 bmp180;

void BMPProvider::init()
{
    bmp180.begin();
}

/**
 * Get temperature measure in C
 */
double BMPProvider::getTemperature(int precision)
{
    char status;
    double temperature = 0;
    long double readings = 0;

    for (int i = 0; i < precision; i++) {
        status = bmp180.startTemperature();
        if (status != 0)
        {
            delay(status);
            status = bmp180.getTemperature(temperature);
        }

        readings += temperature;
    }

    return readings / precision;
}

/**
 * Get pressure measure in hPa
 */
double BMPProvider::getPressure(double temperature, int precision)
{
    char status;
    double pressure = 0;
    long double readings = 0;

    for (int i = 0; i < precision; i++) {
        status = bmp180.startPressure(3);
        if (status != 0)
        {
            delay(status);
            status = bmp180.getPressure(pressure, temperature);
        }

        readings += pressure;
    }

    return readings / precision;
}

/**
 * Get sealevel pressure mesure in hPa
 */
double BMPProvider::getSeaLevelPressure(double pressure)
{
    return bmp180.sealevel(pressure, ALTITUDE);
}
