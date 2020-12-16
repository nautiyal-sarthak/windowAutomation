// Remote Control with the Huzzah + Adafruit IO
// 
// LED Board
// 
// Adafruit invests time and resources providing this open source code.
// Please support Adafruit and open source hardware by purchasing
// products from Adafruit!
//
// Written by Richard Albritton, based on original code by Tony DiCola for Adafruit Industries
// Licensed under the MIT license.
//
// All text above must be included in any redistribution.

/************************** Configuration ***********************************/

// edit the config.h tab and enter your Adafruit IO credentials
// and any additional configuration needed for WiFi, cellular,
// or ethernet clients.
#include "config.h"

/************************ Example Starts Here *******************************/

#include <ESP8266WiFi.h>
#include <Servo.h>

// RGB LED Pins
#define servoPin 5

Servo Servo1;

// set up the 'digital' feed
AdafruitIO_Feed *AssistiveCallButtons = io.feed("switch");

void setup() {

  // set power switch tail pin as an output
  Servo1.attach(servoPin);

  // set up serial monitor
  Serial.begin(115200);

  // wait for serial monitor to open
  while(! Serial);

  // connect to io.adafruit.com
  Serial.print("Connecting to Adafruit IO");
  io.connect();

  Servo1.detach();
  

  // set up a message handler for the 'digital' feed.
  // the handleMessage function (defined below)
  // will be called whenever a message is
  // received from adafruit io.
  AssistiveCallButtons -> onMessage(handleMessage);

  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());
  // recv. the assistive-call-buttons feed
  //AssistiveCallButtons->get();

}

void loop() {

  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io.run();

}

// this function is called whenever an 'digital' feed message
// is received from Adafruit IO. it was attached to
// the 'digital' feed in the setup() function above.
void handleMessage(AdafruitIO_Data *datain) {

  Serial.print("received <- ");
  Serial.println(datain->toInt());
  Servo1.attach(servoPin);
  switch(datain->toInt()) {
    case 0: // your hand is on the sensor
      Serial.println("Anti-clock");
      Servo1.write(0);
      delay(20000);
      Servo1.detach();
      break;
    case 1: // your hand is close to the sensor
      Serial.println("clock");
      Servo1.write(180);
      delay(20000);
      Servo1.detach();
      break;
  }
  // delay in-between reads for stability
  delay(1);
}
