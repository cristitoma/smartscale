#include "GsmHandler.h"

SoftwareSerial sim(SIM_RX, SIM_TX);

void GsmHandler::handle(smsReceivedCallback onSmsReceivedCallback, callReceivedCallback onCallReceivedCallback)
{
  if (sim.available() > 0)
  {
    String message = sim.readString();
    if (message != "\n")
    {
      message.trim();

      //on Sms received
      if (message.substring(1, 4) == "CMT")
      {
        onSmsReceived(message, onSmsReceivedCallback);
      }

      //on call received
      else if (message.substring(0, 4) == "RING")
      {
        onCallReceived(message, onCallReceivedCallback);
      }

      // unknown command
      else
      {
        //Debug AT output
        #ifdef DEBUG
          display.println(message);
        #endif
      }
    }
  }
}

void GsmHandler::onSmsReceived(String message, smsReceivedCallback onSmsReceivedCallback)
{
  String phone = message.substring(7, 19);
  display.println("SMS received from: " + phone);
  onSmsReceivedCallback(phone, message);
}

void GsmHandler::onCallReceived(String message, callReceivedCallback onCallReceivedCallback)
{
  String phone = message.substring(16, 26);
  display.println("Phone number: " + phone + " is calling...");
  onCallReceivedCallback(phone);
}

void GsmHandler::sendSms(String number, String message)
{
  display.println("Sending SMS...");

  //Mobile phone number to send message
  sim.println("AT+CMGS=\"" + number + "\"");
  delay(1000);

  sim.println(message);
  delay(100);

  // ASCII code of CTRL+Z
  sim.println(char(26));
  delay(1000);

  String output = sim.readString();
  output.trim();

  #ifdef DEBUG
    display.println(output);
  #endif

  if (output.indexOf("OK") != -1)
  {
    display.println("SMS sent.");
  }
  else
  {
    display.println("Error sending SMS.");
  }
}

void GsmHandler::setupReceivingSms()
{
  display.println("Setup SIM to receive SMS...");

  //Sets the GSM Module in Text Mode
  sim.println("AT+CMGF=1");
  delay(1000);

  // AT Command to receive a live SMS
  sim.println("AT+CNMI=2,2,0,0,0");
  delay(1000);

  display.println("Waiting for SMS...");
}

void GsmHandler::setupReceivingCall()
{
  display.println("Setup SIM to receive calls...");

  sim.println("AT+CLIP=1");
  delay(1000);

  display.println("Waiting for calls...");
}

void GsmHandler::hangOffCall()
{
  display.println("Hanging off call...");
  sim.println("ATH");
  delay(1000);
}

void GsmHandler::checkingForPIN()
{
  bool isLocked = true;

  do
  {
    display.println("Waiting for PIN validation...");
    sim.println("AT+CPIN?");
    delay(1000);

    String output = sim.readString();
    output.trim();

    #ifdef DEBUG
      display.println(output);
    #endif

    if (output.indexOf("NOT READY") != -1)
    {
      isLocked = true;
    }
    else if (output.indexOf("READY") != -1)
    {
      isLocked = false;
    }
    else if (output.indexOf("ERROR") != -1)
    {
      display.println("PIN validation faild.");
      restartSIM();
    }
    else
    {
      delay(3000);
    }
  } while (isLocked);
  display.println("PIN is ok.");
}

void GsmHandler::checkingForCarrier()
{
  bool hasCarrier = false;

  do
  {
    display.println("Waiting for CARRIER...");
    sim.println("AT+CSPN?");
    delay(1000);

    String output = sim.readString();
    output.trim();

    #ifdef DEBUG
      display.println(output);
    #endif

    if (output.indexOf("ERROR") != -1)
    {
      display.println("CARRIER validation faild.");
      restartSIM();
    }
    else
    {
      hasCarrier = true;
    }
  } while (!hasCarrier);

  display.println("CARRIER is ok.");
}

void GsmHandler::restartSIM()
{
  display.println("Waiting for SIM to restart...");
  //restart sim if error
  sim.println("AT+CFUN=1,1");
  delay(15000);
}

void GsmHandler::init()
{
  display.println("Initializing SIM...");
  sim.begin(9600);
  sim.println("AT+CMGF=1");
  delay(1000);

  checkingForPIN();
  checkingForCarrier();
  checkForNetwork();
  display.println("SIM is initialized...");

  setupReceivingSms();
  setupReceivingCall();
}

void GsmHandler::checkForNetwork()
{
  bool isConnected = false;

  do
  {
    display.println("Waiting for network...");
    sim.println("AT+CGATT?");
    delay(1000);

    String output = sim.readString();
    output.trim();

    #ifdef DEBUG
      display.println(output);
    #endif

    if (output.indexOf("CGATT: 1") != -1)
    {
      isConnected = true;
    }
    else if (output.indexOf("ERROR") != -1)
    {
      display.println("Network validation faild.");
      restartSIM();
    }
    else
    {
      isConnected = false;
      delay(5000);
    }
  } while (!isConnected);

  display.println("Network is ok.");
}
