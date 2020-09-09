#define P1A 5    // (L293D input pin, pin number 2) , control direction of rotation  
#define P2A 6   // (L293D input pin, pin number 7) , To control the PWM 
#define EN1 9   // Pin 9 is defined as EN1 (L293D enable pin, pin number 1)
// encoder pin
const int encoderChannelA = 2;//Input 1 of encoder
const int encoderChannelB = 3;//Input 2 of encoder
// encoder value 
const float ratio = 360./188.611/48.; // These values are obtained from the data sheet
// 360 is 1 turn, 188.611 is the gear ratio, and 48 is the motor shaft Countable Events per revolution
// Parameters for PID Control
float DesiredSpeed=22;          // 22 is the value for Desired Speed in RPM 
float Kp = 1.29;                // proportional constant is 1.29
float Ki = 0.031;               // integral constant is 0.031
float kd = 0.01;                // derivative constant is 0.01
float EncoderSpeed=0;           // the speed of the encoder is declared and set as 0    
float PreviousEncoderPos=0;     // PreviousEncoderPos is declared and set as 0
float ActualSpeed =0;           // ActualSpeed is declared as 0
float currentTime=0;            // currentTime variable is declared and set as 0
unsigned long PreviousTime   = 0; // PreviousTime variable is declared and set as 0
float Duration  = 15;           // Duration variable is declared and set as 15
int encoderPos = 0;             // Encoder pos is declared and set as 0
float error=1;                  // Error is declared and set as 1 initially
float previouserror = 0;        // Previouserror is declared and set as 0 initially
float SumError = 0;             // SumError is declared and set as 0 initially
// Initiate Variable for the Motor Motion
bool a; // a is the variable for setting the motor direction, in this case CCW
int b;  // b is the variable for controlling the PWM signal

void doEncoderA() //Fuction to Counting rotation on encoder in pin A
{  
  encoderPos += (digitalRead(encoderChannelA)==digitalRead(encoderChannelB))?1:-1; // using ternary operator
  // encoderPos = 1 + encoderPos if true // encoderPos = -1 + encoderPos If false
}
void doEncoderB()//Fuction to Counting rotation on encoder in pin B
{  
  encoderPos += (digitalRead(encoderChannelA)==digitalRead(encoderChannelB))?-1:1;// using ternary operator
  // encoderPos = -1 + encoderPos if true // encoderPos = 1 + encoderPos If false
}
void setup()
{
  Serial.begin(9600);           
  // setup Serial Monitor to display information
  pinMode(encoderChannelA, INPUT_PULLUP); 
  // EncoderchannelA was set as input_pullup, which the pin is effectively connected through an internal resistor to the VCC+ power line
  attachInterrupt(0, doEncoderA, CHANGE); 
  // Interrupt number 0 means for Arduino pin number 2, it will cal the doencoderA function to calculate the encoder value
  // CHANGED is used to trigger the interrupt whenever the pin change the value
  pinMode(encoderChannelB, INPUT_PULLUP); 
  // EncoderchannelB was set as input_pullup, which the pin is effectively connected through an internal resistor to the VCC+ power line
  attachInterrupt(1, doEncoderB, CHANGE);
  // Interrupt number 1 means for Arduino pin number 3,
  // CHANGED is used to trigger the interrupt whenever the pin change the value
  pinMode(P1A, OUTPUT);   // P1A is defined as output
  pinMode(EN1, OUTPUT);   // EN1 is defined as output
}
void loop()
{
    for (int i = 0; i <= 100; i++)  {  // Iteration for 100 times 
  currentTime = millis();       // Use Millis function to obtain the current time
  if(currentTime ==0)         // the first count is set to 1
  {currentTime =1;}
    Duration = (double)(currentTime - PreviousTime); // Calculate the duration time
  PreviousTime=currentTime;              // CurrentTime is set as previous time
  SumError += error * Duration;            // Total error is obtained from error times duration
   previouserror=error;                  // current error is set as the previous error
  EncoderSpeed=encoderPos-PreviousEncoderPos;    // The encoder speed is obtained from encoderPos - PreviousencoderPos
  PreviousEncoderPos=encoderPos;           // the current encoder pos is set as the previous encoderpos
  // Actual Speed Calculation
  ActualSpeed = float(EncoderSpeed)*ratio;       
  ActualSpeed = ActualSpeed*1000*60/(360*Duration);//RPM
  error = DesiredSpeed - ActualSpeed;        // Error is obtained from the difference between Desired and actual speed
  float control = Kp*error+Ki*SumError+kd*(error-previouserror)*Duration;// Formula for PID
  digitalWrite(EN1, 255);          // send 255 PWM signal or 5volt to enable pin of L293D
  a = (control>=0)?HIGH:LOW;       // If control is greater than 0, a is set HIGH
  b =  min(abs(control), 255);     //  The control value is taken as absolute and limit to 255 value, cannot exceed this value
  analogWrite(P2A, a?(255 - b):b); // if a is HIGH the 255-b value will be sent to control the desired PWM value
  digitalWrite(P1A,a);             // send High or low value to Arduino pin a,in this case, HIGH is CCW  
   Serial.print(ActualSpeed); // Display the actual speed in the graph_  
  Serial.print("| Encoder : "); // Display the Encoder Value
  Serial.print(encoderPos);
  Serial.print("| Speed : "); // Display the Speed Value
  Serial.print(ActualSpeed);
  Serial.print("| Error : "); // Display the Error Value
  Serial.print(error);
  Serial.print("| Control : "); // Display the Control Value
  Serial.println(control);
    }
  Serial.print("Process Finished"); 
  while(1); // Function to stop the iteration from looping
    }
