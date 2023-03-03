// Include RadioHead Amplitude Shift Keying Library
#include <RH_ASK.h>
// Include dependant SPI Library
#include <SPI.h>
#include <Servo.h>
#define Speed
byte M[6] = {2, 4, 7, 8, 3, 5}; //Motor Driver Pins: left m1 pin 1,left m1 pin 2,right m1 pin 1,right m1 pin 2,pwm1,pmw2
#define Vser 9 //pin for Vertical Movement
#define Hser 10 //pin for horizontal Movement 
#define Speed 200
Servo VServo, HServo; //defining the servos used for arm
RH_ASK rf_driver; //defining the rf transmitter instance

void setup() {
  for (int i = 0; i < 6; i++) {
    pinMode(M[i], 1);
    digitalWrite(M[i], 0);
  }
  // Initialize ASK Object
  rf_driver.init();
  VServo.attach(Vser); //hooking up the servo objects to the pins
  HServo.attach(Hser);
  VServo.write(90); //home the value to 90 degree
  HServo.write(90); //home the value to 90 degree
  // Setup Serial Monitor
  Serial.begin(9600);
}
uint8_t buf[3]; //buffer to hold our message
void loop() {
  uint8_t buflen = sizeof(buf);
  if (rf_driver.recv(buf, &buflen)) { //if the message is avaliable
    //time to decode our message
    /*
      we will have a data frame of 2 bytes:
      byte 1: first 6 bits are X value then 2 bits for Up and Down
      byte 2: first 6 bits are Y value then 2 bits for Left and Right
    */
    //first we use >> to right shift our value then we appy a filter to extract eithr a binary value or 6 bit analog values
    byte up =    (buf[0] >> 0) % 2;
    byte down =  (buf[0] >> 1) % 2;
    byte left =  (buf[0] >> 2) % 2;
    byte right = (buf[0] >> 3) % 2;
    byte X = buf[1];
    byte Y = buf[2];
    //now we have all our inputs we can handle them:
    //arm:
    VServo.write(Y); //move the vertical to value
    HServo.write(X); //move the Horizontal to value
    if (!up) { //if up is pressed
      Forward();
    } else if (!down) {
      Backward();
    } else if (!left) {
      Left();
    } else if (!right) {
      Right();
    }else{
      Stop();
    }
  }

}
void Forward() {
  digitalWrite(M[0], 1);
  digitalWrite(M[1], 0);
  digitalWrite(M[2], 1);
  digitalWrite(M[3],  0);
  analogWrite(M[4], Speed);
  analogWrite(M[5], Speed);
}
void Backward() {

  digitalWrite(M[0], 0);
  digitalWrite(M[1], 1);
  digitalWrite(M[2], 0);
  digitalWrite(M[3], 1);
  analogWrite(M[4], Speed);
  analogWrite(M[5], Speed);
}
void Right() {

  digitalWrite(M[0], 1);
  digitalWrite(M[1], 0);
  digitalWrite(M[2], 0);
  digitalWrite(M[3], 1);
  analogWrite(M[4], Speed);
  analogWrite(M[5], Speed);
}
void Left() {

  digitalWrite(M[0], 0);
  digitalWrite(M[1], 1);
  digitalWrite(M[2], 1);
  digitalWrite(M[3], 0);
  analogWrite(M[4], Speed);
  analogWrite(M[5], Speed);
}
void Stop() {

  digitalWrite(M[0], 0);
  digitalWrite(M[1], 0);
  digitalWrite(M[2], 0);
  digitalWrite(M[3], 0);
  analogWrite(M[4], 0);
  analogWrite(M[5], 0);
}
