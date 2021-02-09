// + DISPLAY STUFF ---------------------------------------------------------------------


void writestarting(void) {
  display.clearDisplay();

  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(WHITE);        // Draw white text
  display.setCursor(30,30);             // Start at top-left corner
  display.println(F("starting..."));
  display.display();
  delay(2000);
}


void writeconnectingGSM(void) {
  display.clearDisplay();

  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(WHITE);        // Draw white text
  display.setCursor(0,30);             // Start at top-left corner
  display.println(F("connecting GSM..."));
  display.display();
  delay(2000);
}

void writeGSMconnected(void) {
  display.clearDisplay();

  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(WHITE);        // Draw white text
  display.setCursor(0,30);             // Start at top-left corner
  display.println(F("GSM connected!"));
  display.display();
  delay(2000);
}

void writeconnectingAPN(void) {
  display.clearDisplay();

  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(WHITE);        // Draw white text
  display.setCursor(0,30);             // Start at top-left corner
  display.println(F("connecting APN..."));
  display.display();
  delay(2000);
}
