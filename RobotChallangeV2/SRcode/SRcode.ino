int latchPin =9;
//Pin connected to SH_CP of 74HC595
int clockPin = 10;
////Pin connected to DS of 74HC595
int dataPin = 8;



void setup() {
  //set pins to output so you can control the shift register
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // count from 0 to 255 and display the number 
  // on the LEDs
  for (int numberToDisplay = 0; numberToDisplay < 256; numberToDisplay++) {
    // take the latchPin low so 
    // the LEDs don't change while you're sending in bits:
    digitalWrite(latchPin, LOW);
    // shift out the bits:
  // numberToDisplay=analogRead(A8)/4;
    shiftOut(dataPin, clockPin, MSBFIRST, numberToDisplay);  
Serial.println(numberToDisplay);
    //take the latch pin high so the LEDs will light up:
    digitalWrite(latchPin, HIGH);
    // pause before next value:
    delay(500);
  }
}


