/******************************************************************************
   INCLUDES
 ******************************************************************************/

#include <MKRNB.h>
#include <ArduinoMqttClient.h>
#include <ArduinoJson.h>
#include "arduino_secrets.h"
#include "LiveObjects.h"

/******************************************************************************
   DEFAULT VALUES
 ******************************************************************************/

// Set MQTT security:
// comment the line to disable security (MQTT),
// uncomment to activate TLS security (MQTTS).
#define MQTT_TLS

/******************************************************************************
   USER VARIABLES
 ******************************************************************************/

uint32_t messageRate = 60;   // stores the current data message rate in Seconds
uint32_t messageRateInMs = messageRate * 1000;   // stores the current data message rate in Milliseconds
unsigned long uptime;   // stores the device uptime (sent as fake sensor data)
unsigned long lastMessageTime = 0;   // stores the time when last data message was sent


/******************************************************************************
   USER PROGRAM
 ******************************************************************************/

  // an example of simple parameter callback function:
  // this function is called after the messageRate parameter has been updated over the air
  // and transforms the message rate from Seconds to Milliseconds, which is the native time measurement on Arduino
void processMsgRate() {
  messageRateInMs = messageRate * 1000;
}


void setup() {
  Serial.begin(115200);
  Serial.print("\n*** Live Objects on Arduino MKR NB 1500, revision ");
  Serial.print(SW_REVISION);
  Serial.println("***");

  // Declaring a parameter with a callback function 'processMsgRate'.
  // This function will be called after the update of the variable 'messageRate'.
  // This parameter will become available for modification over the air from Live Objects
  // upon the first connection: go to Devices > your device > Parameters
  // Note that parameters are reset upon restart.
  addParameter("message rate (seconds)", messageRate, processMsgRate);
  
  LiveObjects_connect();                          // connects to the network + Live Objects
}

void loop() {
  if (millis() - lastMessageTime > messageRateInMs) {
    // collect data periodically
    Serial.println("Sampling data");
    uptime = millis();
    addToPayload("uptime", uptime);               // adding 'uptime' value to the current payload

    Serial.println("Sending data to Live Objects");
    sendData();                                   // send the data to Live Objects
    lastMessageTime = millis();
  }

  LiveObjects_loop();                             // don't forget to keep this in your main loop
}