#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
#define RxD 10
#define TxD 11

#define forcepin A1

#define ledpin2 12

char recvChar;                                        //variable to store character received
String recvStr;                                       //variable to store the series of received character

boolean blink;
const int rs = 6, en = 7, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
SoftwareSerial slave(RxD, TxD);                       //Slave arduino acts as a serial communication device

void setup()
{
  Serial.begin(9600);                                //start serial monitor, for communicating with the PC

  pinMode(RxD, INPUT);                               //set mode of receive pin (from bluetooth)
  pinMode(TxD, OUTPUT);                              //set mode of transmit pin (to bluetooth)
  slave.begin(9600);                                 //start the bluetooth serial "port"
  pinMode (forcepin, INPUT);                         // set force pin to input
  pinMode(ledpin2, OUTPUT);                           //set led pin to output

  
   // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  // Print a message to the LCD.
  lcd.print("Floor Level");
  lcd.setCursor(0, 1);

}
void loop()
{
  if (slave.available()>0)                            //if data is available from the master arduino
  {
    recvChar = slave.read();                        //store character received
    Serial.print(recvChar);                         //display character received on the serial monitor
    lcd.setCursor(0, 1);
    lcd.print(recvChar);
  
  }
   int analogReading = analogRead(forcepin);
 
    if (analogReading <10)      // sanitize; led will be blinking
   {
    digitalWrite(ledpin2, LOW);
    delay(1000);
    digitalWrite(ledpin2, HIGH);
    delay(1000);
   }
   else        ///dont sanitize
   {
   digitalWrite(ledpin2, HIGH);       // do not light up
   }
 
}
