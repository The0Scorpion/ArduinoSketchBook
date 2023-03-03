// ask_transmitter.pde
// -*- mode: C++ -*-
// RadioHead to transmit messages
// with a simple ASK transmitter in a very simple way.
// Implements a simplex (one-way) transmitter with an TX-C1 module
/*
  Connections:
  Buttons:3,4,5,6
  JoyStick:
  X:A0
  y:A1

  RF Tranimtter DATA: Pin 12

*/
#include <RH_ASK.h>//Library for RF Transmitter
#include <SPI.h> // Not actually used but needed to compile

RH_ASK driver; //defining the rf transmitter instance


#define joyX A0
#define joyY A1
#define Up 3
#define Down 4
#define Left 5
#define Right 6


void setup()
{
  Serial.begin(9600);	  // Debugging only

  if (!driver.init())
    Serial.println("init failed");
  //defining all switches to be pulled up internally (LOW=Pressed)
  pinMode(Up, INPUT_PULLUP);
  pinMode(Down, INPUT_PULLUP);
  pinMode(Left, INPUT_PULLUP);
  pinMode(Right, INPUT_PULLUP);
}
/*
 * we will have a data frame of 2 bytes:
 * byte 1: using bits: 0,0,0,0,up,down,left,right
 * byte 2: X value
 * byte 3: Y value
 */
void loop()
{
  byte byte1 = (digitalRead(Up) << 0) + (digitalRead(Down) << 1) + (digitalRead(Left) << 2) + (digitalRead(Right) << 3); // the symbol << means left shift to make which is a binary opretain to fit the data
  byte byte2 = map(analogRead(joyX),0,1023,0,180); //x analog value will be 8 bit = 0-180 range
  byte byte3 = map(analogRead(joyY),0,1023,0,180); //y analog value will be 8 bit = 0-180 range
  
  char msg[]={byte1,byte2,byte3};
  driver.send((uint8_t *)msg, 3);
  driver.waitPacketSent();
}
