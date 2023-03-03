#include <SoftwareSerial.h>
#include <Keypad.h>
int latchPin = 8;
//Pin connected to SH_CP of 74HC595
int clockPin = 12;
////Pin connected to DS of 74HC595
int dataPin = 11;
const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {13, A1, A2, A3}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {A4, A5, A6, A7}; //connect to the column pinouts of the keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );


SoftwareSerial  Serial9(9, 10);
byte state = 0;
byte numofSR = 1;
uint8_t Values[] = {0x00};
uint8_t off[] = {0x00};
uint8_t on[] = {0xff};
void initsr() {
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  setAll(Values);
}
void set(int pin, uint8_t value) {
  if (value == 1)
    Values[pin / 8] |= 1 << (pin % 8);
  else
    Values[pin / 8] &= ~(1 << (pin % 8));

  setAll(Values);
}
void setAll(uint8_t  digitalValues[]) {
  int i;
  digitalWrite(latchPin, 0);
  for (i =  numofSR - 1 ; i >= 0; i--) {
    shiftOut(dataPin, clockPin, MSBFIRST, digitalValues[i]);
    Values[i] = digitalValues[i];
  }

  digitalWrite(latchPin, 1);
  Serial.println(digitalValues[0], BIN);
}
void show(int x) {
  // if (state == 0) {
  set(x , 1);

  //} else if (state == 1) {

  // } else {

  //}
  Serial.println((String)x + ';');
}
void hide(int x) {
  set(x, 0);
}
void hideall() {
  setAll(off);
}
void setup()
{
  Serial.begin(9600);
  Serial9.begin(9600);            // set the baud rate to 9600, same should be of your Serial9 Monitor
  pinMode(13, OUTPUT);
  Serial.println("Ready");
  initsr();
}
int c;
String pass;
bool f = true;
int xs ;
void Dance(int d1 , int d2 , int d3) {
  setAll(on);
  delay(d1);
  setAll(off);
  delay(d1);
  setAll(on);
  delay(d2);
  setAll(off);
  delay(d2);
  setAll(on);
  delay(d3);
  setAll(off);
  delay(d3);
}
void loop()
{
  int i = 0;

  /*  c = Serial9.read()-48 ;
    c *= 10;
    c += Serial9.read()-48;
    Serial.println(c);*/
  char key = keypad.getKey();
  if (key != NO_KEY) {
    Serial.println(key);
  }
  if (Serial9.available() && f) {
    Serial.println(c = (Serial9.read() - 48) * 10);
    Serial.println(c += Serial9.read() - 48);
    f = !f;
  } else {

  }
  switch ((int) c) {
    case 10:
      //    Dance(100,100,200);
      //   Dance(100,100,200);
      //  Dance(100,100,200);
      Serial.println("Connected");
      f = !f;
      break;
    case 11:
      xs = Serial9.readStringUntil('"').toInt();
      Values[0] = {0x00};
      show(xs);

      break ;
    case 12 :
      Dance(100, 100, 2000);
      Serial.println("Disconnected");
      f = !f;
      break ;
    case 13:
      f = !f;
      Dance(100, 100, 500);
      Dance(500, 1000, 2000);
      digitalWrite(19, 1);
      digitalWrite(20, 0);
      digitalWrite(21, 0);
      delay(1000);
      digitalWrite(19, 0);
      digitalWrite(20, 1);
      digitalWrite(21, 0);
      delay(1000);
      digitalWrite(19, 0);
      digitalWrite(20, 0);
      digitalWrite(21, 1);
      delay(1000);
      digitalWrite(19, 1);
      digitalWrite(20, 1);
      digitalWrite(21, 1);
      delay(1000);
      digitalWrite(19, 0);
      digitalWrite(20, 0);
      digitalWrite(21, 0);
      Serial.println("Reseted");
      break ;
    case 15:
      f = !f;
      Serial.print("Recived : ");
      Serial.println(Serial9.readStringUntil('"'));
      break ;
    case 50:
      f = !f;
      digitalWrite(19, 1);
      digitalWrite(20, 0);
      digitalWrite(21, 0);
      break ;
    case 51:
      f = !f;
      digitalWrite(19, 0);
      digitalWrite(20, 1);
      digitalWrite(21, 0);
      break ;
    case 52:
      f = !f;
      digitalWrite(19, 0);
      digitalWrite(20, 0);
      digitalWrite(21, 1);
      break ;
    case 44:
      f = !f;
      Serial9.print(404);
      Serial.println("Command Not Found");
      while (Serial9.available()) {
        Serial.write(Serial9.read());
      }
      for (int i = 1; i <= 5; i++) {
        digitalWrite(53, 1);
        delay(150 * i);
        digitalWrite(53, 0);
        delay(150 * i);
      }
      break;

    case 99:
      Serial.println("Enter Password");
      for (int i = 0; i < 4; i++) {
        for (int i = 0 ; i < 4; i++) {
          char key = keypad.getKey();
          while (key == NO_KEY) {
            char key = keypad.getKey();
          }
          if (key != NO_KEY) {
            pass += key;
            Serial.println(pass);
          }
        }

      }
      if (pass = "1111") {
        Serial9.print("Right");
      }
      pass = "";
      break;

    default:
      f = !f;
      break;
  }
  bool srq = analogRead(A0) < 500;
  for (int i = 0; i < 3; i++)
    digitalWrite(19+i, srq);


  c = 0;
}


