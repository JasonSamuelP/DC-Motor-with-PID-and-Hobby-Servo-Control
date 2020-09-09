#include <Servo.h>
int pos = 0;
Servo servoku;  initialize servoku as servo object

// Initial Set up for push button mechanism
bool pressedCW = false;    // pressedCW is set "false" initially 
bool pressedCCW = false;   // pressedCCW is set "false" initially 
#define PushButtonCCW 10   // Pin 10 is defined as push button pin to increment 20 degree in CCW direction
#define PushButtonCW 11    // Pin 11 is defined as push button pin to increment 20 degree in CW direction

void setup() {
    Serial.begin(9600);// setup Serial Monitor to display information
    servoku.attach(9);  // attaches the servo on pin 9 to the servo object
    pinMode(PushButtonCW, INPUT); // PushButtonCW is defined as INPUT
    pinMode(PushButtonCCW, INPUT); // PushButtonCCW is defined as INPUT
}
void loop()  
{ 
  servoku.write(pos); // Write the servo position in degree (1-180)

  // Left Button Mechanism
  if (digitalRead(PushButtonCCW)==HIGH)  // if we press the push button, pressed will have "true" value since we declared it as "false"
  {
   pressedCCW = !pressedCCW; 
  }
  while(digitalRead(PushButtonCCW)==HIGH) // This function enable us to push the button at period of time
                                          // while the push button is true (being pressed), it will do nothing
  // Once we release the pushbutton . . .     
  if (pressedCCW == true && pos<180)      // If these conditions are true, the servo will rotate 20 degree in CCW direction
  {
      pos = pos + 20;                     // Incrementing the pos value
      delay(30);                          // Delay for 30ms
      pressedCCW = !pressedCCW;           // Change the condition for pressedCCW
      Serial.println("CCW rotation for 20 degree"); // Print on the serial monitor
      Serial.println(pos); // Print on the serial monitor
  }

  // Right Button Mechanism
  if (digitalRead(PushButtonCW)==HIGH)    // if we press the push button, pressed will have "true" value since we declared it as "false"
  {
   pressedCW = !pressedCW; 
  }
  while(digitalRead(PushButtonCW)==HIGH)  // This function enable us to push the button at period of time
                                          // while the push button is true (being pressed), it will do nothing
  // Once we release the pushbutton . . .
    if (pressedCW == true && pos>0)       // If these conditions are true, the servo will rotate 20 degree in CW direction
  {
      pos = pos - 20;                     // Decrementing the pos value
      delay(30);                          // Delay for 30ms
      pressedCW = !pressedCW;             // Change the condition for pressedCW
      Serial.println("CW rotation for 20 degree"); // Print on the serial monitor
      Serial.println(pos);                          // Print on the serial monitor
  }
