#include <Arduino.h>

/********************************************************************/
// First we include the libraries
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
/********************************************************************/
// Data wire is plugged into pin 2 on the Arduino
#define ONE_WIRE_BUS 2
/********************************************************************/
// Setup a oneWire instance to communicate with any OneWire devices
// (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
/********************************************************************/
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);
/********************************************************************/
// Setup LCD Display
LiquidCrystal_I2C lcd(0x27,16,2); // set the LCD address to 0x27 for a 16 chars and 2 line display
/********************************************************************/
#define ventilatorPIN 10
#define fantoglePIN 3

bool fanon = false;

void setup(void)
{
  // start serial port
  Serial.begin(9600);
  Serial.println("3D Printing enclosure");
  // Start up the library
  sensors.begin();
  lcd.init(); //initialize the lcd
  //lcd.begin (16,2);
  lcd.backlight(); //open the backlight

   //define outputs
   pinMode(fantoglePIN,INPUT);
  pinMode(ventilatorPIN,OUTPUT);
  digitalWrite (ventilatorPIN, LOW);

  printLCD("3D Printing",0);
  printLCD("3D Printing",1);
  
  //digitalWrite (fantoglePIN, HIGH);
  //attachInterrupt(digitalPinToInterrupt(fantoglePIN),fantogle,HIGH);
}

void fantogle()
{
  Serial.println("3fantogle");
  if(fanon)
  {
    fanon=false;
  }
  else
  {
    fanon = true;
  }
}

void loop(void)
{

  float temp = 0;
   // call sensors.requestTemperatures() to issue a global temperature
   // request to all devices on the bus
  /********************************************************************/
   Serial.print(" Requesting temperatures...");
   sensors.requestTemperatures(); // Send the command to get temperature readings
   Serial.println("DONE");
  /********************************************************************/
   temp = sensors.getTempCByIndex(0);
   Serial.print("Temp: " + String(temp) );
   //Serial.print(sensors.getTempCByIndex(0));
   // Why "byIndex"?
    // You can have more than one DS18B20 on the same bus.
   // 0 refers to the first IC on the wire
   printLCD("Temp: " + String(temp),0);
   //lcd.print("Temp: " + String(temp) );
   
   if(digitalRead(fantoglePIN) == HIGH)
   {
    fantogle();
    }
   // If temo gets to high we need the Fan
   if(temp>27 || fanon)
   {
     digitalWrite (ventilatorPIN, HIGH);
     printLCD("Fan: ON  ",1);
   }
   else
   {
      digitalWrite (ventilatorPIN, LOW);
      printLCD("Fan: OFF",1);
   }


   delay(1000);
}


void printLCD(string message, int line)
{
    lcd.setCursor(0,line);
    lcd.print(message);
}