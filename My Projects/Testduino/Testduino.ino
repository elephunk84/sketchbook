/*Tesduino Script*/
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>

#define I2C_ADDR    0x27 // <<----- Add your address here.  Find it from I2C Scanner
#define BACKLIGHT_PIN     3
#define En_pin  2
#define Rw_pin  1
#define Rs_pin  0
#define D4_pin  4
#define D5_pin  5
#define D6_pin  6
#define D7_pin  7

volatile bool systemStart = true;
unsigned long previousTime = 0;
unsigned long currentTime;
int n = 1;
int pwmPin = 9;
float input_voltage = 0.0;
float temp=0.0;
float r1=100000.0;
float r2=10000.0;
const int analogIn = A1;
int mVperAmp = 185; // use 100 for 20A Module and 66 for 30A Module
int RawValue= 0;
int ACSoffset = 2500; 
double Voltage = 0;
double Amps = 0;


LiquidCrystal_I2C	lcd(I2C_ADDR,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin);

void lcdSystemStart() {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("System Starting");
}

void lcdDisplay(){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Power System");
    lcd.setCursor(0,1);
    lcd.print("Voltage= ");
    lcd.print(input_voltage);
    }

void normal_run(){
    currentTime = millis();
    int analog_value = analogRead(A0);
    temp = (analog_value * 5.0) / 1024.0; 
    input_voltage = temp / (r2/(r1+r2));
    RawValue = analogRead(analogIn);
    Voltage = (RawValue / 1024.0) * 5000; // Gets you mV
    Amps = ((Voltage - ACSoffset) / mVperAmp);
    if (input_voltage < 0.1) 
    {
     input_voltage=0.0;
    } 
    if((currentTime - previousTime >= 1000) &! (systemStart)){
        lcdDisplay();
        n++;
        previousTime = currentTime;
    }
    if((systemStart) && (currentTime - previousTime >= 3000)){
        systemStart = false;
        previousTime = currentTime;
    }
}

void pwmLED() {
    currentTime = millis();
    digitalWrite(pwmPin, HIGH);
    if((currentTime - previousTime >= 100) &! (systemStart)){
        lcdDisplay();
        digitalWrite(pwmPin, LOW);
        previousTime = currentTime;
    }
    if((currentTime - previousTime >= 1000) &! (systemStart)){
        n++;
    }
    if((systemStart) && (currentTime - previousTime >= 3000)){
        systemStart = false;
        previousTime = currentTime;
    }
}
void setup()
{
  lcd.begin (20, 4);
  lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
  lcd.setBacklight(HIGH);
  lcd.home ();
  previousTime = millis(); 
  Serial.begin(9600);
  lcdSystemStart();
}

void loop()
{
  normal_run();  
}
