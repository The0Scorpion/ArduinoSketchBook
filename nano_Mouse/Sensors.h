#include <Arduino.h>
template<byte FE, byte FD, byte RE, byte RD, byte LE, byte LD>
class Sensors {
  private:
    int FA, FC, FR, RA, RC,
        RR, LA, LC, LR, FT, RT,
        LT, FRs[20], LRs[20],
        RRs[20], FS, RS, LS;
    byte index = 0 ;

  public:
    int FF, LF, RF;
    void configure () {
      pinMode(FE, OUTPUT);
      pinMode(RE, OUTPUT);
      pinMode(LE, OUTPUT);

    }
    void view() {

      Serial.println(sense());
    }
   void Sense() {
      digitalWrite(LE, HIGH);
      digitalWrite(RE, HIGH);
      digitalWrite(FE, HIGH);
      delay(4);
      FC = analogRead(FD);
      RC = analogRead(RD);
      FC = analogRead(RD);
      digitalWrite(LE, LOW);
      digitalWrite(RE, LOW);
      digitalWrite(FE, LOW);
      FA = analogRead(FD);
      RA = analogRead(RD);
      FA = analogRead(RD);
      FR = FC - FA;
      RR = RC - RA;
      LR = LC - LA;
      FT -= FRs[index];
      RT -= RRs[index];
      LT -= LRs[index];
      FRs[index] = FR;
      RRs[index] = RR;
      LRs[index] = LR;
      FT += FR;
      RT += RR;
      LT += LR;
      index += 1;
      if (index >= 20) {
        index = 0;
      }
      FS = FT / 20;
      RS = RT / 20;
      LS = LT / 20;
      FF = FS;
      RF = RS;
      LF = LS;
     
    }
    void init() {
      for(byte i ; i <20 ; i++){
        Sense();
      }
      
    }
};

