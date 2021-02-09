

/*
 * not used so far
void sendTime()
{
  // sending time to slaves
  // Time format is 6 fixed bytes:
  //    byte 1: day in HEX
  //    byte 2: month in HEX
  //    byte 3: year  in HEX; not 2020 but 20
  //    byte 4: Hours in HEX
  //    byte 5: minutes in HEX
  //    byte 6: sec in HEX
  //    byte 7: cr as End of Line
  
  // Print date...
  Serial1.write(rtc.getDay());
  Serial1.write(rtc.getMonth());
  Serial1.write(rtc.getYear());
  Serial1.write(rtc.getHours());
  Serial1.write(rtc.getMinutes());
  Serial1.write(rtc.getSeconds());
  Serial1.write(0x0D);
 
}
*/
void printTime()
{
  //Serial.print("Unix time = ");
  //Serial.println(rtc.getEpoch());
 
  // Print date...
  print2digits(rtc.getDay());
  Serial.print("/");
  print2digits(rtc.getMonth());
  Serial.print("/");
  print2digits(rtc.getYear());
  Serial.print(",");

  // ...and time
  print2digits(rtc.getHours());
  Serial.print(":");
  print2digits(rtc.getMinutes());
  Serial.print(":");
  print2digits(rtc.getSeconds());
  Serial.print(",");
 
}

void print2digits(int number) {
  if (number < 10) {
    Serial.print("0"); // print a 0 before if the number is < than 10
  }
  Serial.print(number);
}
