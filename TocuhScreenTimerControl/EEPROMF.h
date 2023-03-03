//void writeIntArrayIntoEEPROM(int address, unsigned int numbers[], int arraySize)
//{
//  int addressIndex = address;
//  for (int i = 0; i < arraySize; i++) 
//  {
//    EEPROM.write(addressIndex , numbers[i] >> 8);
//    EEPROM.write(addressIndex+1 , numbers[i] & 0xFF);
//    addressIndex += 2;
//  }
//}
//void readIntArrayFromEEPROM(int address, unsigned int numbers[], int arraySize)
//{
//  int addressIndex = address;
//  for (int i = 0; i < arraySize; i++)
//  {
//    numbers[i] =  (EEPROM.read(addressIndex)<<8)+EEPROM.read(addressIndex+1);
//    addressIndex += 2;
//  }
//  Serial.println();
//}
