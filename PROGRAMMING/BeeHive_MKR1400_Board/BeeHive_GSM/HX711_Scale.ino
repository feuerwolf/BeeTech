

//#define calibration_factor -24200 //Wert wird für jede Waage separat ermittelt!

HX711 scale;


int scale_initialize(){
  Serial.println("Starting scale");
  scale.begin(HX_DOUT, HX_CLK);
  scale.set_scale(calibration_factor_int); // see above
  
  // Acquire reading without blocking and check if scale is connected
  if (scale.wait_ready_timeout(3000)) {
      long reading = scale.get_units(10);
      Serial.print("Weight: ");
      Serial.println(reading, 2);
  } else {
      Serial.println("Scale not found.");
      return 0;
  }
  
  return 1;
}



void scale_realAll(){
 //float value_float = 0.0;

  Serial.println("------------------------------------");

  Serial.print("read raw ADC: ");
  Serial.println(scale.read());                 // print a raw reading from the ADC

  Serial.print("read average: ");
  Serial.println(scale.read_average(20));       // print the average of 20 readings from the ADC

  Serial.print("get value   : ");
  Serial.println(scale.get_value(5));    // print the average of 5 readings from the ADC minus the tare weight, set with tare()

  Serial.print("get units   : ");
  Serial.println(scale.get_units(5), 1);        // print the average of 5 readings from the ADC minus tare weight, divided
            // by the SCALE parameter set with set_scale
/*
  Serial.print("one reading:\t");
  Serial.print(scale.get_units(), 1);
  Serial.print("\t| average:\t");
  Serial.println(scale.get_units(10), 1);
*/
}



float scale_readings(){
  float weight_float = 0.0;

  weight_float = scale.get_units(5);

  return weight_float;
}
