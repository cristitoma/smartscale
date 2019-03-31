#include <SoftwareSerial.h>
#ifndef Display_h
#define Display_h

class Display
{
  public:
    void init();
    void print(String message);
    void println(String message);
};
#endif
