#include <Servo.h>
#include "Motors.h"
#include "Sensors.h"
#include "NanoMouseMaze.h"

const byte ledpin = 13, buttonpin = 9;
byte event = 0;
byte targetf = 0;
boolean obt[3];
int targets;
int thers;
NanoMouseMaze<4, 6> maze;
Sensors<3, A6, 2, A5, 4, A7> sensors ;
Motors motors;
void setup() {
  pinMode(ledpin, OUTPUT);
  pinMode(buttonpin, INPUT_PULLUP);
  motors.attach(6, 5);
  Serial.begin(9600);
  sensors.configure();
  maze.mouseRow = 2;
  maze.mouseColumn = 1;
  maze.mouseHeading = EAST;
  maze.targetRow = 2;
  maze.targetColumn = 4;
  maze.addwalls();
  maze.solve();
 maze.print();
 
  while (digitalRead(buttonpin)) {

  }
  inita();
  delay(500);
  forwardfinsh();
  maze.print();
}

void loop() {
}
void forwardfinsh() {
  while (sensors.FF < targetf) {
    sensors.Sense();
    if (sensors.RF > thers && sensors.LF > thers) {
      motors.forwardprop(sensors.RF - sensors.LF);
    } else if ( sensors.RF > thers) {
      motors.forwardprop(sensors.RF - targets);
    } else if ( sensors.LF > thers) {
      motors.forwardprop(targets - sensors.LF);
    } else {
      motors.forward();
    }
  }
  motors.stop();
}
void inita() {
  sensors.init();
  targets = (sensors.RF + sensors.LF) / 2;

  motors.turn(RIGHT, 90);
  sensors.init();
  targetf = sensors.FF;
  thers = (targets + sensors.LF) / 2;
  motors.turn(LEFT, 90);
  sensors.init();
}
byte state() {
  event = 0;
  sensors.Sense();
  if (sensors.FF > 30) {
    obt[0] = true;
    event += 1;
  } else {
    obt[0] = false;
  }
  if (sensors.RF > 30) {
    obt[1] = true;
    event += 2;
  } else {
    obt[1] = false;
  }
  if (sensors.LF > 30) {
    obt[2] = true;
    event += 4;
  } else {
    obt[2] = false;
  }
}
void avoid() {
  switch (event)
  {
    case 1:
      if (random(2)) {
        motors.turn(LEFT, random(90, 181));
      } else {
        motors.turn(RIGHT, random(90, 181));
      }

      break;
    case 2:
      motors.turn(RIGHT,  random(30, 61));
      break;
    case 4:
      motors.turn(LEFT,  random(30, 61));
      break;
    default:
      motors.Runboxes(1);
      break;
  }
}

