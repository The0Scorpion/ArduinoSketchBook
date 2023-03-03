#include <boarddefs.h>
#include <IRremote.h>
#include <IRremoteInt.h>
#include <ir_Lego_PF_BitStreamEncoder.h>
#define ON 0xFFFFFF
#define OFF 0x000000
#define MON 0
#define MOFF 1
IRrecv ir(11);

decode_results r;
bool killed=1; 
byte M[6] = {2, 3, 4, 5, 20, 21}; //lm11,lm12,rm21,rm22,pwm1,pmw2
byte LF1 = A0, LF2 = A0;
byte UST = 6, USE = 7;
int Speed = 200/*Motor Int Speed*/, T[] = {}/*distane to detrmine if opnennet is there*/, Thrs = 200/*IR Sensor*/;

void Forward() {
  digitalWrite(M[0], MON);
  digitalWrite(M[1], MOFF);
  digitalWrite(M[2], MON);
  digitalWrite(M[3],  MOFF);
  analogWrite(M[4], Speed);
  analogWrite(M[5], Speed);
}
void Backward() {
  digitalWrite(M[0], MOFF);
  digitalWrite(M[1], MON);
  digitalWrite(M[2],MOFF);
  digitalWrite(M[3], MON);
  analogWrite(M[4], Speed);
  analogWrite(M[5], Speed);
}
void Right() {
  digitalWrite(M[0], MON);
  digitalWrite(M[1], MOFF);
  digitalWrite(M[2], MOFF);
  digitalWrite(M[3], MON);
  analogWrite(M[4], Speed);
  analogWrite(M[5], Speed);
}
void Left() {
  digitalWrite(M[0], MOFF);
  digitalWrite(M[1], MON);
  digitalWrite(M[2], MON);
  digitalWrite(M[3], MOFF);
  analogWrite(M[4], Speed);
  analogWrite(M[5], Speed);
}
void Stop() {
  digitalWrite(M[0], MOFF);
  digitalWrite(M[1], MOFF);
  digitalWrite(M[2], MOFF);
  digitalWrite(M[3], MOFF);
  analogWrite(M[4], 0);
  analogWrite(M[5], 0);
}
byte CheckIn() {
  if (analogRead(LF1) < Thrs) {
    return 1;
  } else if (analogRead(LF2) > Thrs) {
    return 2;
  } else {
    return 0;
  }

}
void GetIn() {
  byte line = CheckIn();
  if (line == 1) {
    Backward();
  } else if (line == 2) {
    Forward();
  }
}
double GetDis() {
  digitalWrite(UST, 1);
  delay(40);
  digitalWrite(UST, 0);
  float dur = pulseIn(USE, 1);
  float dis = (dur / 2) *0.034;
  return dis;
  
}
void setup() {
    Serial.begin(9600);
  for (int i = 0; i < 6; i++) {
    pinMode(M[i], 1);
     digitalWrite(M[i],MOFF);
  }
  pinMode(UST, 1);
  ir.enableIRIn();
}

void loop() {
Serial.println(CheckIn());
  }
