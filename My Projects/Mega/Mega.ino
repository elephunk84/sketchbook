/*Tesduino Script*/
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include <DS3231.h>
#include <MenuSystem.h>

#define SLAVE_ADDRESS 0x04
#define I2C_ADDR    0x27 // <<----- Add your address here.  Find it from I2C Scanner
#define BACKLIGHT_PIN     3
#define En_pin  2
#define Rw_pin  1
#define Rs_pin  0
#define D4_pin  4
#define D5_pin  5
#define D6_pin  6
#define D7_pin  7

int bedLightsButton = 7;
int iainLightsButton = 6;
int eloraLightsButton = 5;
const byte bedLightsPWM = 13;
const byte iainLightsPWM = 12;
const byte eloraLightsPWM = 11;
int bedLightsVal = 0;
int iainLightsVal = 0;
int eloraLightsVal = 0;
int bedLightsState = HIGH;
int iainLightsState = HIGH;
int eloraLightsState = HIGH;
int bedLightsReading;
int iainLightsReading;
int eloraLightsReading;
int bedLightsPrevious = LOW;
int iainLightsPrevious = LOW;
int eloraLightsPrevious = LOW;

volatile bool systemStart = true;
unsigned long previousTime = 0;
unsigned long currentTime;
String dow;
String time;
String date;
long debounce_time = 0;
long debounce = 200;

LiquidCrystal_I2C	lcd(I2C_ADDR,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin);
DS3231  rtc(SDA, SCL);

void lcdSystemStart() {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("System Starting");
}

void lcdDisplay(){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Bedroom Iain's Side");
    display_time();
    }

void normal_run(){
    currentTime = millis();
    int analog_value = analogRead(A0);
    if((currentTime - previousTime >= 1000) &! (systemStart)){
        lcdDisplay();
        previousTime = currentTime;
    }
    if((systemStart) && (currentTime - previousTime >= 3000)){
        systemStart = false;
        previousTime = currentTime;
    }
}

void display_time(){
  dow = rtc.getDOWStr();
  date = rtc.getDateStr();
  time = rtc.getTimeStr();
  lcd.setCursor(0,1);
  lcd.print(date + "  ");
  lcd.print(time);
}
void setup(){
  pinMode(bedLightsButton, INPUT);
  pinMode(iainLightsButton, INPUT);
  pinMode(eloraLightsButton, INPUT);
  pinMode(bedLightsPWM, OUTPUT);
  pinMode(iainLightsPWM, OUTPUT);
  pinMode(eloraLightsPWM, OUTPUT);
  lcd.begin (20, 4);
  lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
  lcd.setBacklight(HIGH);
  lcd.home ();
  previousTime = millis(); 
  Serial.begin(9600);
  rtc.begin();
  Wire.begin(SLAVE_ADDRESS);
  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);
  rtc.setDOW(FRIDAY);     // Set Day-of-Week to SUNDAY
  rtc.setTime(18, 00, 00);     // Set the time to 12:00:00 (24hr format)
  rtc.setDate(27, 5, 2016);   // Set the date to January 1st, 2014
  lcdSystemStart();
}

void loop()
{
  
}
