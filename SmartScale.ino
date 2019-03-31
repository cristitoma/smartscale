#include "GsmHandler.h"
#include "HX711.h"
#include "BMPProvider.h"
#include "Display.h"

//Display
// #define DEBUG
Display display;

//GSM
#define SIM_RX 10
#define SIM_TX 11
GsmHandler gsmHandler;

//Scale
#define calibration_factor 12140.0
#define DOUT  A1
#define CLK  A2
HX711 scale;

//BMP
#define ALTITUDE 91.0
BMPProvider bmpProvider;

void setup()
{
  display.init();
  gsmHandler.init();
  scale.begin(DOUT, CLK);
  scale.set_scale(calibration_factor);
  scale.tare();
}

void loop()
{
  gsmHandler.handle(onSmsReceived, onCallReceived);
}

void onCallReceived(String phone)
{
  gsmHandler.hangOffCall();
  notifyRecipient(phone);
}

void onSmsReceived(String phone, String message)
{
  notifyRecipient(phone);
}

void notifyRecipient(String phone)
{
  float weight = scale.get_units(100);
  if (weight < 0.00) {
    weight = 0.00;
  }

  bmpProvider.init();
  double temperature = bmpProvider.getTemperature(100);
  double pressure = bmpProvider.getPressure(temperature, 100);
  double seaLevelPressure = bmpProvider.getSeaLevelPressure(pressure);

  String message = "Weight: ";
  message.concat(weight);
  message.concat(" kg.\nTemperature: ");
  message.concat(temperature);
  message.concat(" C.\nPressure: ");
  message.concat(seaLevelPressure);

  display.println(message);
  gsmHandler.sendSms(phone, message);
}
