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

const byte upButtongh = 6;
const byte enterButton = 7;
const byte leftButton = 5;
const byte rightButton = 4;
const byte downButton = 3;
const byte exitButton = 2;

volatile bool systemStart = true;
unsigned long previousTime = 0;
unsigned long currentTime;
String dow;
String time;
String date;

int number = 0;
int state = 0;

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
  //rtc.setDOW(THURSDAY);     // Set Day-of-Week to SUNDAY
  //rtc.setTime(21, 13, 0);     // Set the time to 12:00:00 (24hr format)
  //rtc.setDate(19, 5, 2016);   // Set the date to January 1st, 2014
  lcdSystemStart();
}

void loop()
{
  normal_run(); 
}

void receiveData(int byteCount){
  while(Wire.available()) {
    number = Wire.read();
    Serial.print("data received: ");
    Serial.println(number);
    if (number == 1){
      if (state == 0){
      digitalWrite(13, HIGH); // set the LED on
      state = 1;
      }
    else{
      digitalWrite(13, LOW); // set the LED off
      state = 0;
      }
    }
  }
}

void sendData(){
  Wire.write(number);
}
