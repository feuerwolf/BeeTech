const int numReadings = 40;


int getSound(int inputPin){
  int total = 0;                  // the running total
  int average = 0;                // the average
  
  total = 0;
  // collect values of X readings
  for(int i = 0; i < numReadings; i++) {
    total = total + analogRead(inputPin);
    delay(1);  // for stability
  }

  average = total / numReadings;
  
  //Serial.println(average);
  return average;

}
