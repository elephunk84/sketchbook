/*Tesduino Script*/
#include <Wire.h>/tmp/buildf0f5612a78456c3e260f7c6ba47d8bf9.tmp/Bedroom_Control.ino.hex
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include <DS3231.h>
#include <MenuSystem.h>
#include <Time.h>

#define NANO_1_ADDRESS 0x04
#define NANO_2_ADDRESS 0x05

#define I2C_ADDR    0x27 // <<----- Add your address here.  Find it from I2C Scanner
#define BACKLIGHT_PIN     3
#define En_pin  2
#define Rw_pin  1
#define Rs_pin  0
#define D4_pin  4
#define D5_pin  5
#define D6_pin  6
#define D7_pin  7

const byte bedLightsButton = 40;
const byte iainLightsButton = 42;
const byte eloraLightsButton = 44;
const byte bedLightsPWM = 8;
const byte iainLightsPWM = 9;
const byte eloraLightsPWM = 10;
int bedLightsVal = 0;
int iainLightsVal = 0;
int eloraLightsVal = 0;
int bedLightsState = LOW;
int iainLightsState = LOW;
int eloraLightsState = LOW;
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

int number = 0;
int state = 0;

LiquidCrystal_I2C lcd(I2C_ADDR,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin);
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
    lcd.setCursor(0,2);
    //lcd.print("Runtime = ");
    //lcd.setCursor(13,2);
    //lcd.print(number);
    }

void normal_run(){
    currentTime = millis();
    int analog_value = analogRead(A0);
    if((currentTime - previousTime >= 1000) &! (systemStart)){
        lcdDisplay();
        number++;
        previousTime = currentTime;
    }
    if((systemStart) && (currentTime - previousTime >= 3000)){
        systemStart = false;
        previousTime = currentTime;
    }
}

void display_time(){
  long time_now;
  dow = rtc.getDOWStr();
  date = rtc.getDateStr();
  time = rtc.getTimeStr();
  Serial.println(time);
}
void run_time(long val){  
  int days = elapsedDays(val);
  int hours = numberOfHours(val);
  int minutes = numberOfMinutes(val);
  int seconds = numberOfSeconds(val);
  Serial.print(days,DEC);  
  printDigits(hours);  
  printDigits(minutes);
  printDigits(seconds);
  Serial.println();  
}

void printDigits(byte digits){
 // utility function for digital clock display: prints colon and leading 0
 Serial.print(":");
 if(digits < 10)
   Serial.print('0');
 Serial.print(digits,DEC);  
}

void button_check(){
  bedLightsReading = digitalRead(bedLightsButton);
  if (bedLightsReading == HIGH && bedLightsPrevious == LOW && millis() - debounce_time > debounce) {
    if (bedLightsState == HIGH)
      bedLightsState = LOW;
    else
      bedLightsState = HIGH;
    debounce_time = millis();    
  }
  digitalWrite(bedLightsPWM, bedLightsState);
  bedLightsPrevious = bedLightsReading;  
  iainLightsReading = digitalRead(iainLightsButton);
  if (iainLightsReading == HIGH && iainLightsPrevious == LOW && millis() - debounce_time > debounce) {
    if (iainLightsState == HIGH)
      iainLightsState = LOW;
    else
      iainLightsState = HIGH;
    debounce_time = millis();    
  }
  digitalWrite(iainLightsPWM, iainLightsState);
  iainLightsPrevious = iainLightsReading;
  eloraLightsReading = digitalRead(eloraLightsButton);
  if (eloraLightsReading == HIGH && eloraLightsPrevious == LOW && millis() - debounce_time > debounce) {
    if (eloraLightsState == HIGH)
      eloraLightsState = LOW;
    else
      eloraLightsState = HIGH;
    debounce_time = millis();    
  }
  digitalWrite(eloraLightsPWM, eloraLightsState);
  eloraLightsPrevious = eloraLightsReading;
}

void light(int n){
  switch ( n ) {
    case 1:
    {
      if (bedLightsState == HIGH)
        bedLightsState = LOW;
      else
        bedLightsState = HIGH;
      }
      break;
    case  2:
    {
     if (iainLightsState == HIGH)
        iainLightsState = LOW;
    else
        iainLightsState = HIGH;
      }
      break;
    case 3:
    {
     if (eloraLightsState == HIGH)
      eloraLightsState = LOW;
    else
      eloraLightsState = HIGH;
      }
      break;
    }
  }
  
void lightcheck(){
  if (iainLightsState == HIGH){
    Serial.println("Iains Lights On");
  }
  else if (iainLightsState == LOW){
    Serial.println("Iains Lights Off");
  }
  if (eloraLightsState == HIGH){
    Serial.println("Eloras Lights On");
  }
  else if (eloraLightsState == LOW){
    Serial.println("Eloras Lights Off");
  }
  if (bedLightsState == HIGH){
    Serial.println("Bed Lights On");
  }
  else if (bedLightsState == LOW){
    Serial.println("Bed Lights Off");
  }
}
    
void setup(){
  pinMode(bedLightsButton, INPUT_PULLUP);
  pinMode(iainLightsButton, INPUT_PULLUP);
  pinMode(eloraLightsButton, INPUT_PULLUP);
  pinMode(bedLightsPWM, OUTPUT);
  pinMode(iainLightsPWM, OUTPUT);
  pinMode(eloraLightsPWM, OUTPUT);
  lcd.begin (20, 4);
  lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
  lcd.setBacklight(160);
  lcd.home ();
  previousTime = millis(); 
  Serial.begin(9600);
  rtc.begin();
  //rtc.setDOW(WEDNESDAY);     // Set Day-of-Week to SUNDAY
  //rtc.setTime(19, 17, 00);     // Set the time to 12:00:00 (24hr format)
  //rtc.setDate(19, 7, 2016);   // Set the date to January 1st, 2014
  lcdSystemStart();
}

void loop(){
//  lightcheck();
  if (Serial.available()) {
    light(Serial.read() - '0' );
  }
  button_check();
}
