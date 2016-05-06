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

LiquidCrystal_I2C	lcd(I2C_ADDR,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin);

void lcdSystemStart() {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("System Starting");
}

void lcdDisplay(){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Tesduino System");
    lcd.setCursor(0,1);
    lcd.print("Run Time = ");
    lcd.print(n);
    lcd.setCursor(0,3);
    lcd.print("IainStott Systems");
}

void normal_run(){
    currentTime = millis();
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
  lcd.begin (20,4);
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
