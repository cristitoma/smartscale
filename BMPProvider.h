#include <Wire.h>
#include <SFE_BMP180.h>

#ifndef BMPProvider_h
#define BMPProvider_h

class BMPProvider
{
  public:
    void init();
    double getTemperature(int precision);
    double getPressure(double temperature, int precision);
    double getSeaLevelPressure(double pressure);
};
#endif
