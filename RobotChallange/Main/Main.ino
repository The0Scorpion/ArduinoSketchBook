 #include <IRremote.h>
#include <IRremoteInt.h>
#define ON 0xFFFFFF
#define OFF 0x000000
#define MON 0
#define MOFF MON?0:1
bool spin = 0, dir;
IRrecv ir(11);
decode_results r;
bool killed = 0;
byte line;
byte M[6] = { 4, 5, 3, 2, 20, 21}; //lm11,lm12,rm21,rm22,pwm1,pmw2
byte LF[] = {A0, A1, A2, A3};
byte UST = 6, USE = 7;
bool deb = 0; //Debugging State
int ls = 5;
int Speed = 200/*Motor Int Speed*/, T[] = {5, 50, 150}/*distane to detrmine if opnennet is there*/, Thrs = 200/*IR Sensor*/;
void Forward() {
  if (deb) {
    if (ls != 0) {
      ls = 0;
      Serial.println("Forward");
    }
    return;
  }
  digitalWrite(M[0], MON);
  digitalWrite(M[1], MOFF);
  digitalWrite(M[2], MON);
  digitalWrite(M[3],  MOFF);
  analogWrite(M[4], Speed);
  analogWrite(M[5], Speed);
}
void Backward() {
  if (deb) {
    if (ls != 1) {
      ls = 1;
      Serial.println("Backward");
    }
    return;
  }
  digitalWrite(M[0], MOFF);
  digitalWrite(M[1], MON);
  digitalWrite(M[2], MOFF);
  digitalWrite(M[3], MON);
  analogWrite(M[4], Speed);
  analogWrite(M[5], Speed);
}
void Right() {
  if (deb) {
    if (ls != 2) {
      ls = 2;
      Serial.println("Right");
    }
    return;
  }
  digitalWrite(M[0], MON);
  digitalWrite(M[1], MOFF);
  digitalWrite(M[2], MOFF);
  digitalWrite(M[3], MON);
  analogWrite(M[4], Speed);
  analogWrite(M[5], Speed);
}
void Left() {
  if (deb) {
    if (ls != 3) {
      ls = 3;
      Serial.println("Left");

    }
    return;
  }
  digitalWrite(M[0], MOFF);
  digitalWrite(M[1], MON);
  digitalWrite(M[2], MON);
  digitalWrite(M[3], MOFF);
  analogWrite(M[4], Speed);
  analogWrite(M[5], Speed);
}
void Stop() {
  if (deb) {
    if (ls != 4) {
      ls = 4;
      Serial.println("Stop");
    }
    return;
  }
  digitalWrite(M[0], MOFF);
  digitalWrite(M[1], MOFF);
  digitalWrite(M[2], MOFF);
  digitalWrite(M[3], MOFF);
  analogWrite(M[4], 0);
  analogWrite(M[5], 0);
}
byte CheckIn() {
  if ( digitalRead(LF[0]) && !digitalRead(LF[1]) && !digitalRead(LF[2]) && !digitalRead(LF[3])) {
    return 5;
  }
 /* if(digitalRead(LF[0])==1&&(!digitalRead(LF[2])||!digitalRead(LF[3]))){
    Backward();
    
  }*/
  return digitalRead(LF[0]) ? 1 : !digitalRead(LF[1]) ? 2 :/* !digitalRead(LF[2]) ? 3 : !digitalRead(LF[3]) ? 4 :*/ 0;
}
void GetIn() {
  byte Dir = CheckIn();
  if(Dir!=line){
    line=Dir;
  }
  if (line == 1) {
    Backward();
    delay(400);
   Turn();
  } else if (line == 2) {
    Forward();
  } else if (line == 3) {
    Left();
  } else if (line == 4) {
    Right();
  } else if (line == 5) {
    Stop();
  }
}
double GetDis() {
  float dis;
  float dur;
  for (int i = 0; i < 1; i++) {
    digitalWrite(UST, 1);
    if (CheckIn()) {
      GetIn();
    }
    delayMicroseconds(100);
    digitalWrite(UST, 0);
    dur = pulseIn(USE, 1);
    dis += (dur / 2) * 0.034;
  }
   
    if (CheckIn()) {
      GetIn();
    }
  return dis;

}
void Turn() {
  if (spin) {
    if (dir) {
      Right();
    } else {
      Left();
    }
  } else {
    dir = random(0, 2);
    spin = 1;
    if (dir) {
      Right();
    } else {
      Left();
    }
  }
}
void setup() {
  for (int i = 0; i < 6; i++) {
    pinMode(M[i], 1);
    digitalWrite(M[i], MOFF);
  }
  pinMode(UST, 1);
  ir.enableIRIn();
  Serial.begin(9600);
  if (deb) {
    Serial.begin(9600);
  }
  delay(5000);
}
void loop() {
  /*  if (Serial.available()) {
    int c = Serial.read() - 48;
    if (c == 1) {
      deb = !deb;
      Stop();
    }
    if (c == 0) {
      killed = !killed;
      Stop();
    }
    if (c == 2) {
     Serial.print("Dis : ");
    Serial.println(GetDis());
    }
    if(c==3){
      Serial.print("   &  L1,L2,L3,L4 : ");
    Serial.println(analogRead(A0));
    }
  }
  if (deb) {
     Serial.print("Dis : ");
     Serial.println(GetDis());
     Serial.print("   &  L1,L2,L3,L4 : ");
     Serial.println((String)!digitalRead(A0) + " , " + (String)digitalRead(A1) + " , " + (String)digitalRead(A2) + " , " + (String)digitalRead(A3));
  }*/
  if (!killed) {
    if (CheckIn()) {
      GetIn();
      return ;
    }

    double distance = GetDis();
    if (distance < T[0]) {
       spin = 0;
      Forward();
      return ;
    } else if (distance < T[1]) {
       spin = 0;
      Forward();
      return ;
    } else if (distance < T[2]) {
      Turn();
      return;
    } else {
      Turn();
      return;
    }
  }
  if (ir.decode(&r)) {
    if (r.value == OFF) {
      Stop();
      killed = 1;
    }
    ir.resume();
  } else {
    if (ir.decode(&r)) {
      if (r.value == ON) {
        killed = 0;
      }
      ir.resume(); // Receive the next value
    }
  }
}
