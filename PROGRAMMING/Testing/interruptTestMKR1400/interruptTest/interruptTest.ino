const byte LED = 21;
/*
const byte BUTTON1 = 18;  // Button 2
const byte BUTTON2 = 19;  // Button 2
*/

const int SWITCH  =  0;  // interrupt ?  
const int BUTTON1 =  16;  // 20
const int BUTTON2 =  17;  // 21


// Interrupt Service Routine (ISR)
void switchPressed ()
{
  if (digitalRead (BUTTON1) == HIGH)
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


// Interrupt Service Routine (ISR)
void switchPressed3 ()
{
  if (digitalRead (SWITCH) == HIGH)
    digitalWrite (LED, HIGH);
  else
    digitalWrite (LED, LOW);
}  // end of switchPressed

void setup ()
{
  pinMode (LED, OUTPUT);  // so we can update the LED
  /*
  pinMode (BUTTON1, INPUT_PULLUP); 
  pinMode (BUTTON2, INPUT_PULLUP); 
  pinMode (SWITCH , INPUT_PULLUP); 
  */
 
  /*
   * usable interrupts on MKR Boards https://www.arduino.cc/reference/de/language/functions/external-interrupts/attachinterrupt/
   * PINs 0, 1, 4, 5, 6, 7, 8, 9, A1, A2
   */
  
  attachInterrupt (digitalPinToInterrupt (BUTTON1),  switchPressed, CHANGE);  // attach interrupt handler
  attachInterrupt (digitalPinToInterrupt (BUTTON2), switchPressed2, CHANGE);  // attach interrupt handler
  attachInterrupt (digitalPinToInterrupt (SWITCH),  switchPressed3, CHANGE);  // attach interrupt handler


 
}  // end of setup

void loop ()
{
  // loop doing nothing 
} 
