#include <SoftwareSerial.h>
#include "UCN5804.h"
#include <Servo.h>
#define motorSteps 200     // change this depending on the number of steps
                           // per revolution of your motor
#define RxD 2              // receiver pin for SPI
#define TxD 3              // transmitter pin for SPI

#define RxDLCD 5              // receiver pin for SPI
#define TxDLCD 10             // transmitter pin for SPI


#define ledpin   A5
#define dirPin   13      // define the pins that the motor is attached to. You can use
#define stepPin  12      // any digital I/O pins.
#define halfPin  7
#define phasePin 6

#define servoPin 4

#define echoPin 8 // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin 9 //attach pin D3 Arduino to pin Trig of HC-SR04

Servo myServo;
UCN5804 myStepper(motorSteps, dirPin, stepPin, halfPin, phasePin);  // initialize object from the Stepper library:
char recvChar;                                        //variable to store character received
SoftwareSerial blueToothSerial(RxD,TxD);              //bluetooth device acts as a serial communication device                                                      //receive and transmit with respect to the Arduino board
SoftwareSerial master(RxDLCD,TxDLCD);

int val;                                              // variable for servo position        
int door;
int inputlevel;
int currentlevel;
long duration;                                        // variable for the duration of sound wave travel
int distance;                                         // variable for the distance measurement
int wait;
int sound;
bool detect;
bool doorstatus; 
void setup()
{
    Serial.begin(9600);                               //start serial monitor, for communicating with the PC
    myStepper.setSpeed(150);                           // set the motor speed at 60 RPMS:
    myServo.attach(servoPin);                          // attaches the servo on pin 4 to the servo object
    pinMode(RxD, INPUT);                              //set mode of receive pin (from bluetooth)
    pinMode(TxD, OUTPUT);                             //set mode of transmit pin (to bluetooth)
    pinMode(trigPin, OUTPUT);                         // Sets the trigPin as an OUTPUT
    pinMode(echoPin, INPUT);                          // Sets the echoPin as an INPUT
    pinMode(ledpin, OUTPUT);
    pinMode(RxDLCD, INPUT);                              
    pinMode(TxDLCD, OUTPUT);
    master.begin(9600);
    blueToothSerial.begin(9600);                      //start the bluetooth serial "port"
    currentlevel = 1;                                 //when the program starts, the lift will automatically go to 1
    doorstatus = true;
    master.write(49);                                 //level 1
    
}
void loop()
{  
   digitalWrite(ledpin, HIGH);                          
  
  if(blueToothSerial.available())                     //if data is available from the bluetooth device
  {

      if (doorstatus == false)
      { delay (1500) ; closedoor(); doorstatus = true; }
      recvChar = blueToothSerial.read();              //store character received
      Serial.print(recvChar);                         //display character received on the serial monitor
      inputlevel = (recvChar -48);
      if ( currentlevel != inputlevel ) {door =0 ; digitalWrite(ledpin, LOW);} 
      myStepper.step((inputlevel - currentlevel)*980);
   
      currentlevel = inputlevel;
      master.write(currentlevel+48);
    if (door ==0 && doorstatus == true){
        
        opendoor();
        analogWrite(ledpin, 1023);
        wait = 0;
    
        doorstatus = false;
    } 
                        
  }

  delay (100);
  wait ++; 
   // For ultrasound sensors to simulate call button
  
if( detect == false )
 { digitalWrite(trigPin, LOW);                         // Clears the trigPin condition
  delayMicroseconds(2);                               
  digitalWrite(trigPin, HIGH);                        // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);  
  duration = pulseIn(echoPin, HIGH);                  // Reads the echoPin, returns the sound wave travel time in microseconds 
  distance = duration * 0.034 / 2;                    // Calculating the distance, Speed of sound wave divided by 2 (go and back)  
  if (distance <30)
  {
     detect = true;
     analogWrite(ledpin, 0);
     delay (100);
     
  }
  
 }
  
  if (wait == 30 && doorstatus == false)
  { closedoor();
  doorstatus = true;
  }
    
  
  if (detect == true && doorstatus == true)
  {
    analogWrite(ledpin, 0);
    myStepper.step((3 - currentlevel)*980);  
    currentlevel = 3;
     master.write(currentlevel + 48); 
     opendoor();
     detect = false;
     analogWrite(ledpin, 1023);
     wait=0;
     doorstatus = false;
     
  }
}


void opendoor ()
{
      delay(1000);
      sound = 0;
                 // For lift door
      for (val = 45; val <= 135; val++)               // loop to open door
     {
         sound ++;
         myServo.write(val);                          // sets the servo position
         delay(15);                                   // waits for the servo to get there
         if ((sound%8) == 0){
          tone(11, 500, 125);
         }
         else (noTone(11));
     }
      delay(30);  // wait a few seconds while the door is opened
     door ++;

}

void closedoor()
{
  sound = 0;
  for (val = 135; val >= 45; val--)                // loop to close door
     {
         sound ++;
         myServo.write(val);                          // sets the servo position
         if ((sound%8) == 0){
          tone(11, 1000, 125);
         }
         else (noTone(11));
         
         delay(15);       // waits for the servo to get there 
     }
     delay(30);  
}
