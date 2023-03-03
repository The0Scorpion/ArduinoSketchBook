#include <Servo.h>
#include <LiquidCrystal_I2C.h>

#define HoldingAngle 90
#define ReleaseAngle 180
#define GateClose 0
#define GateOpen 90
#define METAL  0
#define PG  90
#define Other  180
#define NoiseLevel 800
#define IND 500

LiquidCrystal_I2C lcd(0x27, 20, 4);
Servo Holder;
Servo Basket;
Servo Gate;


//Pins
#define trigPin  7
#define echoPin  8
#define Induction  A0
#define MIC    A1
#define capa  2

//variables:
int indValue;
int i = 0;
int Pipe_Pos = 0;
int Gate_Pos = 0;
int n = 0;

long duration;
int distance;

float metalDetected;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Starting");
  Serial.begin(9600);
  Basket.attach(11);
  Gate.attach(10);
  Holder.attach(9);
  lcd.setCursor(0, 1);
  lcd.print("Moving To Orgin");
  Holder.write(HoldingAngle);
  Gate.write(GateClose);
  Basket.write(METAL);
  delay(2000);
  lcd.clear();
}

void loop() {

  delay(500);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(10);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(30);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
  Serial.print("Distance: ");
  Serial.println(distance);
  indValue = analogRead(Induction); //Save value that is read from the analog pin A0 to the variable indValue
  delay(10);
  byte CAP = digitalRead(capa); // Digital value for Capacitive S.
  delay(500);
  if (indValue >= IND) { //metal
      lcd.clear();
     lcd.setCursor(0, 0);
      lcd.print("Object Detected");
      lcd.setCursor(0, 1);
      lcd.print("Metal");
      Serial.println("Metal Detected");
      Basket.write(METAL);
      delay(2500);
      Holder.write(ReleaseAngle);
      delay(1500);
      Gate.write(GateOpen);
      delay(1500);
      Holder.write(HoldingAngle);
      delay(1500);
      Gate.write(GateClose);
      delay(1500);

    }
    else if (indValue <= IND && CAP == 1) {//Plastic
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Object Detected");
      delay(100);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Checking Sound");
      unsigned long t = millis();
      Holder.write(ReleaseAngle);
      int maxsound = 0;
      while (millis() - t < 2000) {
        maxsound = max(maxsound, analogRead(MIC));
      }
      if (maxsound > NoiseLevel) { //glass/plastic
        lcd.setCursor(0, 1);
        lcd.print("Plastic/Glass");
        Basket.write(PG);
        delay(2000);
        Gate.write(GateOpen);
        delay(1500);
        Gate.write(GateClose);
        delay(1500);
      } else {
        lcd.setCursor(0, 1);
        lcd.print("Other");
        Basket.write(Other);
        delay(2000);
        Gate.write(GateOpen);
        delay(1500);
        Gate.write(GateClose);
        delay(1500);
      }
      Holder.write(HoldingAngle);
      delay(1500);
    }
    else if (distance <= 15) {
    Serial.println("Object Detected");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Object Detected");
    if (indValue <= IND && CAP != 1) {//Glass And Other
      delay(100);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Checking Sound");
      unsigned long t = millis();
      Holder.write(ReleaseAngle);
      int maxsound = 0;
      while (millis() - t < 2000) {
        maxsound = max(maxsound, analogRead(MIC));
      }
      if (maxsound > NoiseLevel) { //glass/plastic
        lcd.setCursor(0, 1);
        lcd.print("Plastic/Glass");
        Basket.write(PG);
        delay(2000);
        Gate.write(GateOpen);
        delay(1500);
        Gate.write(GateClose);
        delay(1500);
      } else {
        lcd.setCursor(0, 1);
        lcd.print("Other");
        Basket.write(Other);
        delay(2000);
        Gate.write(GateOpen);
        delay(1500);
        Gate.write(GateClose);
        delay(1500);
      }
      Holder.write(HoldingAngle);
      delay(1500);
    }
  } else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("No Object Detected");
  }

}
