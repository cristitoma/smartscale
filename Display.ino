#include "Display.h"

void Display::init()
{
    #ifdef DEBUG
        Serial.begin(115200);
    #endif
}

void Display::print(String message)
{
    #ifdef DEBUG
        Serial.print(message);
    #endif
}

void Display::println(String message)
{
    #ifdef DEBUG
        Serial.println(message);
    #endif
}
