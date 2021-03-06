

uint16_t addr = 0;


void init_Fram(){

  if (fram.begin()) {
    Serial.println("Found SPI FRAM");
  } else {
    Serial.println("No SPI FRAM found ... check your connections\r\n");
    return;
  }

}


int getFramSerialNumber(){
  uint8_t value;
  value = fram.read8(addr_serialNumber);
  //Serial.print("Serial number: ");
  //Serial.println(value);
  return value;
}

void setFramSerialNumber(uint8_t serialIntern){
  fram.writeEnable(true);
  fram.write8(addr_serialNumber, serialIntern);
  fram.writeEnable(false);
  //Serial.println("-> serial number saved to eeprom");
  //Serial.print  ("   station serial number: ");
  //Serial.println(serialIntern);
}


void framTest(){

  if (fram.begin()) {
    Serial.println("Found SPI FRAM");
  } else {
    Serial.println("No SPI FRAM found ... check your connections\r\n");
    return;
  }
  
  // Read the first byte
  uint8_t test = fram.read8(0x0);
  Serial.print("Restarted "); Serial.print(test); Serial.println(" times");

  // Test write ++
  fram.writeEnable(true);
  fram.write8(0x0, test+1);
  fram.writeEnable(false);

  // write a string
  fram.writeEnable(true);
  fram.write(0x1, (uint8_t *)"FTW!", 5);
  fram.writeEnable(false);

  // dump the entire 8K of memory!
  uint8_t value;
  for (uint16_t a = 0; a < 8192; a++) {
    value = fram.read8(a);
    if ((a % 32) == 0) {
      Serial.print("\n 0x"); Serial.print(a, HEX); Serial.print(": ");
    }
    Serial.print("0x"); 
    if (value < 0x1) 
      Serial.print('0');
    Serial.print(value, HEX); Serial.print(" ");
  } 
}
