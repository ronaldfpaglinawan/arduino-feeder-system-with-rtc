/*
  Created by Ronald Fornis Paglinawan on 27/07/19.
  Copyright © 2019 Paglinawan Technologies. All rights reserved.
*/

#include <DS3231_Simple.h>
#include <Servo.h>


// create a clock object
DS3231_Simple Clock;

// create servo object to control a servo
Servo feederServo;  

// variable to store the servo position
int pos = 90;    

// set pin 13 to a variable
const int motorPin = 2;

void setup() {
  
  // initiate Serial communication
  Serial.begin(9600);

  // start the clock
  Clock.begin();
  
  // set the output pins
  pinMode(motorPin, OUTPUT);

  // attach the servo to pin 9
  feederServo.attach(9);

  // set the servo to initial position (90 degree)
  feederServo.write(pos);

  // set the date and time
  setDateTime();
  
  // set the alarms
  setAlarms();

}

void setDateTime() {
  
  // Create a variable to hold the data 
  DateTime MyTimestamp;
  
  // Set the date and time (24 hour time)
  // 8th of August 2019 at 12:15pm and 0 Seconds...
  MyTimestamp.Day    = 8;  
  MyTimestamp.Month  = 8;  
  MyTimestamp.Year   = 19; 
  MyTimestamp.Hour   = 12;
  MyTimestamp.Minute = 15;
  MyTimestamp.Second = 0;

  // Then write it to the clock
  Clock.write(MyTimestamp);
  
  // Read the the date and time the clock was set
  Serial.print("The time has been set to: ");
  Clock.printTo(Serial);
  Serial.println();
  
}

void setAlarms() {
  
  // First we will disable any existing alarms
  Clock.disableAlarms();
  
  // Get an initialized timestamp
  DateTime MyTimestamp = Clock.read();

  // Set the morning alarm (6:00am)
  MyTimestamp.Hour   = 6;
  MyTimestamp.Minute = 0;     
  MyTimestamp.Second = 0;     
  Clock.setAlarm(MyTimestamp, DS3231_Simple::ALARM_MATCH_SECOND_MINUTE_HOUR); 


  // Set the evening alarm (5:00pm)
  MyTimestamp.Hour   = 17;     
  MyTimestamp.Minute = 0;
  Clock.setAlarm(MyTimestamp, DS3231_Simple::ALARM_MATCH_MINUTE_HOUR); 
  
}

void  loop() {
  
  // To check the alarms we just ask the clock
  uint8_t AlarmsFired = Clock.checkAlarms();
  
  // Then can check if either alarm is fired (there are 2 alarms possible in the chip)
  // by using a "bitwise and"
  if(AlarmsFired & 1)
  {
    Clock.printTo(Serial); 
    Serial.println("Morning Alarm: - release the feeds");
    runFeederServo();
    //runFeederMotor();
  }
  
  if(AlarmsFired & 2)
  {
    Clock.printTo(Serial); 
    Serial.println("Evening Alarm: - release the feeds");
    runFeederServo();
    //runFeederMotor();
  }

  // read the current time
  Serial.print("Current time: ");
  Clock.printTo(Serial);
  Serial.println();
}

void runFeederMotor() {
  
  Serial.println("===Running the Solenoid===");
  
  // turn the motorPin on (HIGH is the voltage level)
  digitalWrite(motorPin, HIGH);
  delay(10000); // wait for 10s

  // turn the motorPin off by making the voltage LOW
  digitalWrite(motorPin, LOW);
  
}

void runFeederServo() {
  
  Serial.println("===Running the Servo Motor===");
  
  pos = 180;
  feederServo.write(pos);   // set the servo position to 180 degrees
  delay(10000);             // wait for 10s

  pos = 90;
  feederServo.write(pos);   // return the servo position to 90 degrees
  
}
