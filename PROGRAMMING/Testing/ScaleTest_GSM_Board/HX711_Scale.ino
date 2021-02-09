
#define calibration_factor -24200 //Wert wird für jede Waage separat ermittelt!

HX711 scale;

void scale_initialize(){
  Serial.println("Starting scale");
  scale.begin(HX_DOUT, HX_CLK);
  scale.set_scale(calibration_factor); // see above
  scale.tare();

}

float scale_readings(){
  float weight_float = 0.0;

  weight_float = scale.get_units();
 
  return weight_float;
}
