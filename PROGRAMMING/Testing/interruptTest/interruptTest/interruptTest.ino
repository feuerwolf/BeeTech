const byte LED = 15;
const byte BUTTON = 18;  // Button 2
const byte BUTTON2 = 19;  // Button 2

// Interrupt Service Routine (ISR)
void switchPressed ()
{
  if (digitalRead (BUTTON) == HIGH)
    digitalWrite (LED, HIGH);
  else
    digitalWrite (LED, LOW);
}  // end of switchPressed


// Interrupt Service Routine (ISR)
void switchPressed2 ()
{
  if (digitalRead (BUTTON2) == HIGH)
    digitalWrite (LED, HIGH);
  else
    digitalWrite (LED, LOW);
}  // end of switchPressed


void setup ()
{
  pinMode (LED, OUTPUT);  // so we can update the LED
  digitalWrite (BUTTON, HIGH);  // internal pull-up resistor
  attachInterrupt (digitalPinToInterrupt (BUTTON), switchPressed, CHANGE);  // attach interrupt handler

  digitalWrite (BUTTON2, HIGH);  // internal pull-up resistor
  attachInterrupt (digitalPinToInterrupt (BUTTON2), switchPressed2, CHANGE);  // attach interrupt handler

  
}  // end of setup

void loop ()
{
  // loop doing nothing 
} 
