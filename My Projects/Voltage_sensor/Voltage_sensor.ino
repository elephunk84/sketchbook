#include <Wire.h>
#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
int analogInput = A5; // I used A1
 
float vout = 0.0;
float vin = 0.0;
float R1 = 30000.0; //30k
float R2 = 7500.0; //7500 ohm resistor, I tweaked this
int value = 0;
 
void setup()
{
   pinMode(analogInput, INPUT);
   Serial.begin(9600);
   Serial.println("BASIC DC VOLTMETER");
}
 
void loop()
{
   // read the value at analog input
   value = analogRead(analogInput);
   vout = (value * 5.0) / 1023.0;
   vin = vout / (R2/(R1+R2)); 
 
  Serial.print("Voltage = ");
  Serial.println(vin,2);
      lcd.setCursor(0,0);
      lcd.print("V: ");
      lcd.print(vin);
//      lcd.print(current);
      lcd.print(" :A ");
  
      //lcd.setCursor(0,1);
      //lcd.print(watts);
      //lcd.print(" W ");
      //lcd.print(time/3600);
      //lcd.print(" H ");
     
   delay(1000);
}
