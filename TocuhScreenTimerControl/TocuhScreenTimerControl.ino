/*
   Coonections:
   Temp Sensor >> 2
   R1          >> 3
   R2          >> 4
   R3          >> 5
   rtc         >>(6, 7, 8)(RST,CLK,SDA)
   LCD         >>(9,10)
*/
/*
   dataStructure
   10:
      01:Temp Limit
      02:Time
      03:H remain //send
      04:M remain //send
      05:H set    //send
      06:M set    //send
      07:H1 Passed//send
      08:M1 Passed//send
      09:H2 Passed//send
      10:M2 Passed//send
      0a:RelayState//send
      0b:TON      //keep
      0c:SetT     //Keep
      11:Cur Temp //send
      15:Button pressed
    11:
      00->04:Password

*/

#include <SoftwareSerial.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Ds1302.h>
#include <EEPROM.h>
#include "EEPROMF.h"
#define R1 3
#define R2 5
#define R3 4
#define ONE_WIRE_BUS 2
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature TSense(&oneWire);
SoftwareSerial myserial(10, 9); // RX, TX
//SoftwareSerial NoSerial(50, 51);
Ds1302 rtc(6, 7, 8);
Ds1302::DateTime now;

int iTmp;
unsigned int variables[0x15];

int password[] = {0, 0, 0, 0, 0, 0x18, 0x5B, 0xA1}; //1,596,321

unsigned long T_OLD = 0;
unsigned int cur_min = 0;
void setup() {
  // initialize serial:
  myserial.begin(115200);
  Serial.begin(9600);
  rtc.init();
  TSense.begin();
  pinMode(R1, OUTPUT);
  pinMode(R2, OUTPUT);
  pinMode(R3, OUTPUT);
  //retain past values
  EEPROM.get(10, variables);
  myserial.write(0x5A);
  myserial.write(0xA5);
  myserial.write(0x07);
  myserial.write(0x82);
  myserial.write((byte)0);
  myserial.write(0x80);
  myserial.write(0x5a);
  myserial.write(0x14);
  myserial.write(0x03);
  myserial.write(0x30);
  rtc.getDateTime(&now);
  cur_min = (((now.year * 12 + now.month) * 30 + now.day) * 24 + now.hour) * 60 + now.minute;
  updateUI();
  
}

void loop()
{
  if (millis() - T_OLD > 60000) {
    rtc.getDateTime(&now);
    cur_min = (((now.year * 12 + now.month) * 30 + now.day) * 24 + now.hour) * 60 + now.minute;
    Serial.println(cur_min);

    if (variables[0x0b]) {
      if (cur_min > (unsigned int)variables[0x0c]) {
        digitalWrite(R1, 0);
        digitalWrite(R2, 0);
        variables[0x0b] = 0;
      } else {
        if ((variables[0x0a] >> 0) & 1) {
          digitalWrite(R1, (variables[0x0a] >> 0) & 1);
          variables[8] = variables[8] == 59 ? 0 : variables[8] + 1;
          variables[7] += variables[8] ? 0 : 1;
        }
        if ((variables[0x0a] >> 1) & 1) {
          digitalWrite(R2, (variables[0x0a] >> 1) & 1);
          variables[0x10] = variables[0x10] == 59 ? 0 : variables[0x10] + 1;
          variables[9] += variables[0x10] ? 0 : 1;
        }
      }
    }
    T_OLD = millis();
    //store values
    EEPROM.put(10, variables);
    updateUI();
  }
  if (Serial.available()) {
    for (int i = 0; i < 0x15; i++) {
      Serial.print(variables[i], HEX);
      Serial.print(' ');
      Serial.read();
    }
    Serial.println();
  }
  myserialEvent();

}


void myserialEvent()
{
  int iLen;
  byte iData[50];
  int iTmp;
  int iAdr, iVal;
  if (myserial.available())
  {
    if (myserial.read() == 0x5A)
      if (myserial.read() == 0xA5)
      {
        //Serial.print("Recived: ");
        iLen = myserial.read();
        //Serial.print(iLen, HEX);
        //Serial.print(' ');

        iTmp = 0;
        if ((iLen != 6) && (iLen != 0xc)) {
          //Serial.println(" not useful");
          return;
        }
        while (myserial.available() < iLen) ; //Wait for all frame !!!! Blocking procedure
        //Data Structure {0x83,VPH,VPL,VN,VH,VL}
        while (iTmp < iLen) //Compile all frame
        {
          iData[iTmp] = myserial.read();
          if (iData[0] != 0x83)return;
          Serial.print(iData[iTmp], HEX);
          Serial.print(' ');
          iTmp++;
        }
        Serial.println();

        if (iLen == 6) {
          if (iData[2] == 0x15) {

            switch (iData[5]) {
              case 1:
                variables[0x0d] = variables[0];
                EEPROM.put(10, variables);
                beeb(15);
                break;
              case 2:
                variables[0x0c] = cur_min + 30 * variables[2];
                variables[0x0b] = 1;
                variables[2] = 0;
                EEPROM.put(10, variables);
                beeb(15);
                break;
              case 3:
                variables[0x0a] ^= (1 << 0);
                beeb(15);
                break;
              case 4:
                variables[0x0a] ^= (1 << 1);
                beeb(15);
                break;
              case 5:
                beeb(15);
                variables[1]--;
                if (variables[1] < 20)variables[1] = 20;
                break;
              case 6:
                beeb(15);
                variables[1]++;
                if (variables[1] > 40)variables[1] = 40;
              case 7:
                beeb(15);
                variables[2]--;
                if ((int)variables[2] < 0)variables[2] = 0;
                break;
              case 8:
                beeb(15);
                variables[2]++;
                if (variables[2] > 48)variables[2] = 48;
                break;
              default:
                break;
            }
          } else {
            if (iData[5] < 48)
              variables[iData[2]] = iData[5];
          }
          updateUI();
        } else {
          byte correct = 1;
          for (byte i = 0; i < 8; i++) {
            Serial.print(iData[4 + i], HEX);
            Serial.print(' ');
            Serial.print(password[i], HEX);
            Serial.println(' ');
            if ((byte)iData[4 + i] != password[i]) {
              correct = 0;
              return;
            }
          }
          if (correct) {
            beeb(90);
            for (int i = 0; i < 0x16; i++) {
              variables[i] = 0;

              delay(1);
            }
            EEPROM.put(10, variables);
            updateUI();
          }
        }
        //Serial.println();
      }
  }
}

void DGUS_SendVal(int iAdr, int iVal)//Send iVal for VP= iAdr to DGUS
{
  byte bAdrL, bAdrH, bValL, bValH;
  bAdrL = iAdr & 0xFF;
  bAdrH = (iAdr >> 8) & 0xFF;
  bValL = iVal & 0xFF;
  bValH = (iVal >> 8) & 0xFF;
  myserial.write(0x5A);
  myserial.write(0xA5);
  myserial.write(0x05);
  myserial.write(0x82);
  myserial.write(bAdrH);
  myserial.write(bAdrL);
  myserial.write(bValH);
  myserial.write(bValL);

}
void DGUS_Sendarr(int iAdr, int iVal[],int len)//Send iVal for VP= iAdr to DGUS
{
  byte bAdrL, bAdrH, bValL, bValH;
  bAdrL = iAdr & 0xFF;
  bAdrH = (iAdr >> 8) & 0xFF;
  myserial.write(0x5A);
  myserial.write(0xA5);
  myserial.write((byte)3+2*len);
  myserial.write(0x82);
  myserial.write(bAdrH);
  myserial.write(bAdrL);
  for(int i=0;i<len;i++){
  myserial.write((byte)(iVal[i] >> 8) & 0xFF);
  myserial.write((byte)iVal[i] & 0xFF);
  
  }

}
void updateUI() {
  if (variables[1] < 20)variables[1] = 20;
  if (variables[1] > 40)variables[1] = 40;
  if ((int)variables[2] < 0)variables[2] = 0;
  if ((int)variables[2] > 48)variables[2] = 48;
  variables[3] = variables[0x0b] ? (variables[0x0c] - cur_min) / 60 : 0;
  variables[4] = variables[0x0b] ? (variables[0x0c] - cur_min) % 60 : 0;

  variables[5] = (variables[2]) / 2;
  variables[6] = (variables[2] % 2) * 30;
  TSense.requestTemperatures();
  variables[0x11] = TSense.getTempCByIndex(0);
  if (variables[0x0d] <= TSense.getTempCByIndex(0)) {
    variables[0x0a] |= (1 << 2);
  }
  if (variables[0x0d] > TSense.getTempCByIndex(0) - 3) {
    variables[0x0a] &= ~(1 << 2);
  }
  digitalWrite(R3, (variables[0x0a] >> 2) & 1);
  if (variables[0x0b]) {
    if (cur_min > variables[0x0c]) {
      digitalWrite(R1, 0);
      digitalWrite(R2, 0);
      variables[0x0a] &= ~(1 << 0);
      variables[0x0a] &= ~(1 << 1);
      variables[0x0b] = 0;
    } else {
      digitalWrite(R1, (variables[0x0a] >> 0) & 1);
      digitalWrite(R2, (variables[0x0a] >> 1) & 1);
    }
  }
  DGUS_Sendarr(0x1000,variables,0x16);
  /*for (int i = 1; i < 0x16; i++) {
    DGUS_SendVal(0x1000 + i, variables[i]);
    delay(1);
  }*/

}
void beeb(byte len) {
  myserial.write(0x5A);
  myserial.write(0xA5);
  myserial.write(0x06);
  myserial.write(0x82);
  myserial.write((byte)0);
  myserial.write(0xa0);
  myserial.write(0x10);
  myserial.write(len);
  myserial.write(0x40);
}
