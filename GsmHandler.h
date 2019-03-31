#include <SoftwareSerial.h>
#ifndef GsmHandler_h
#define GsmHandler_h

class GsmHandler
{
  typedef void (*smsReceivedCallback)(String phone, String message);
  typedef void (*callReceivedCallback)(String phone);

public:
  void handle(smsReceivedCallback onSmsReceivedCallback, callReceivedCallback onCallReceivedCallback);
  void onSmsReceived(String message, smsReceivedCallback onSmsReceivedCallback);
  void onCallReceived(String message, callReceivedCallback onCallReceivedCallback);
  void sendSms(String number, String message);
  void setupReceivingSms();
  void setupReceivingCall();
  void hangOffCall();
  void checkingForPIN();
  void checkingForCarrier();
  void restartSIM();
  void init();
  void checkForNetwork();
};
#endif
