#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
int IRSensor = 2;

LiquidCrystal_I2C lcd(0x27, 16, 2);
/*
void setup() 
{
    Serial.begin(9600);          //  setup serial

   
  pinMode (IRSensor, INPUT); 
}


void loop()
{
  int statusSensor = digitalRead (IRSensor);

  if (statusSensor == 1) {
    Serial.println("Not detected"); 
  }
  else {
    Serial.println("Detected"); 
  }
} */
byte sensor = 2; //hall effect sensor
int sendRPM = 9;
int RPM = 0;
int count = 0;
int totRPM = 0;
int avgRPM = 0;
byte lastReading;
byte reading;
unsigned long stateChangeTime;
unsigned long previousStateChangeTime;
unsigned long duration;
unsigned long lastDisplay;
boolean newDurationValue = false;

void setup() {
  pinMode (sensor, INPUT_PULLUP);
  Serial.begin(9600);

    lcd.init();
    lcd.backlight();

    pinMode(sendRPM, OUTPUT);
    
}
void loop() {
  lastReading = reading;
  reading = digitalRead(sensor);
  if (reading == LOW && lastReading == HIGH)
  {
    stateChangeTime = micros();
    duration = stateChangeTime - previousStateChangeTime;
    previousStateChangeTime = stateChangeTime;
    count++;
  }

  if (micros() - stateChangeTime >= 5000000) //5 seconds without a transition
  {
    RPM = 0;//unclear what you want to do with tot and avg at this point
    avgRPM = 0;
  }
  else if(count)
  {
    count = 0;
    RPM = 60000000 / duration;
   //create moving average of 6 values
    avgRPM = (avgRPM * 5 + RPM)/6;
  } 

  if (millis() - lastDisplay >= 1000)
  {
    // Serial.print ("avgRPM ");
    // Serial.println (RPM);
     digitalWrite(sendRPM, RPM);
    Serial.println(RPM);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("RPM: ");
    lcd.print(RPM);
    lastDisplay += 1000;
  }
}

