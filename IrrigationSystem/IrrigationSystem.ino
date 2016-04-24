#include <DS3232RTC.h>
#include <Streaming.h>
#include <Wire.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <RTClib.h>
#include <Time.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>

#define I2C_ADDR    0x27 
#define BACKLIGHT_PIN     3
#define En_pin  2
#define Rw_pin  1
#define Rs_pin  0
#define D4_pin  4
#define D5_pin  5
#define D6_pin  6
#define D7_pin  7
#define DS3231_I2C_ADDRESS 0x68

const byte MoistureLevel1 = 15;
const byte MoistureLevel2 = 20;
const byte MoistureLevel3 = 25;
const byte MoistureLevel4 = 30;
const byte MoistureLevel5 = 35;
const byte MoistureLevel6 = 40;
const byte MoistureLevel7 = 45;
const byte MoistureLevel8 = 50;
const byte MoistureLevel9 = 55;
const byte MoistureLevel0 = 60;

const unsigned long Delay1 = 10000;
const unsigned long Delay2 = 20000;
const unsigned long Delay3 = 30000;
const unsigned long Delay4 = 40000;
const unsigned long Delay5 = 50000;
const unsigned long sysDelay = 1500;
const unsigned long LCDDelay = 6000;
unsigned long previousTime = 0;
unsigned long currentTime;
unsigned long interval = 3000;

const byte Time1 = 8;
const byte Time2 = 17;
const byte Time3 = 25;
const byte Time4 = 25;
const byte Time5 = 25;
const byte TimeMin = 1;

const unsigned long zone1RunTime = Delay1;
const unsigned long zone2RunTime = Delay1;
const unsigned long zone3RunTime = Delay1;
const unsigned long zone4RunTime = Delay1;

const byte zone1MoistureLevel = MoistureLevel8;
const byte zone2MoistureLevel = MoistureLevel8;
const byte zone3MoistureLevel = MoistureLevel8;
const byte zone4MoistureLevel = MoistureLevel8;

LiquidCrystal_I2C	lcd(I2C_ADDR,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin);
RTC_DS3231 rtc;

const char *string_table[] =
{   
  "   Sleeping",
  "   Watering",
  "    Testing",
  " Zone 1 For 30s ",
  " Zone 2 For 30s ",
  " Zone 3 For 30s ",
  " Zone 4 For 30s ",
  "  Priming pump  ",
  "   Test Mode    ",
  " Manual Run Mode",
  "System Starting",
  " Automatic Run  ",
  "Stopping Pump",
  "NO",
  "Interrupt",
  };


/*
-Read Sensors
-Control Resevoir Pump
-Control Circulation Pump
-Control Solenoid 1
-Control Button 2
* -Manual Water
* -Short Press Test and water
* -Long Press Manual Water for length of button press
-Set Date & Time
-Control LCD
* -Watering Zone 1
* -Watering Zone 2
* -Watering Zone 3
* -Watering Zone 4 
**
**
-RTC Alarm
-Sleep
*/

const byte testButton = 19;
const byte waterButton = 18;
const byte resevoirPump = 4;
const byte circulationPump = 5;
const byte solenoidZone1 = 50;
const byte solenoidZone2 = 48;
const byte solenoidZone3 = 46;
const byte solenoidZone4 = 44;
const byte zone1sensor1 = 12; 
const byte zone1sensor2 = 8; 
const byte zone2sensor1 = 13;
const byte zone2sensor2 = 9; 
const byte zone3sensor1 = 14;
const byte zone3sensor2 = 10; 
const byte zone4sensor1 = 15;
const byte zone4sensor2 = 11; 
int zone1Sensor1Value;
int zone1Sensor2Value;
int zone2Sensor1Value;
int zone2Sensor2Value;
int zone3Sensor1Value;
int zone3Sensor2Value;
int zone4Sensor1Value;
int zone4Sensor2Value;
int zone1Soil1;
int zone1Soil2;
int zone2Soil1;
int zone2Soil2;
int zone3Soil1;
int zone3Soil2;
int zone4Soil1;
int zone4Soil2;
int zone1Reading;
int zone2Reading;
int zone3Reading;
int zone4Reading;
volatile byte timeHour;
volatile byte timeMinute;
volatile bool zone1Run = false;
volatile bool zone2Run = false;
volatile bool zone3Run = false;
volatile bool zone4Run = false;
volatile bool testModeRun = false;
volatile bool waterModeRun = false;
const unsigned long systemStart = millis();
volatile bool startUp = true;
volatile bool systemRun = false;
volatile bool scheduleRun = false;
volatile bool pumpRun = false;
volatile bool wateringRoutine = false;
volatile bool waterRun = false;
volatile bool LCDRefresh = false;

byte decToBcd(byte val) {
  return( (val/10*16) + (val%10) );
}
byte bcdToDec(byte val) {
  return( (val/16*10) + (val%16) );
}


// System Functions ------
void readSensors() {
    zone1Sensor1Value = analogRead(zone1sensor1);
    zone1Sensor2Value = analogRead(zone1sensor2);
    zone2Sensor1Value = analogRead(zone2sensor1);
    zone2Sensor2Value = analogRead(zone2sensor2);
    zone3Sensor1Value = analogRead(zone3sensor1);
    zone3Sensor2Value = analogRead(zone3sensor2);
    zone4Sensor1Value = analogRead(zone3sensor1);
    zone4Sensor2Value = analogRead(zone3sensor2);
    zone1Sensor1Value = constrain(zone1Sensor1Value, 485, 1023);
    zone1Sensor2Value = constrain(zone1Sensor2Value, 485, 1023);
    zone2Sensor1Value = constrain(zone2Sensor1Value, 485, 1023);
    zone2Sensor2Value = constrain(zone2Sensor2Value, 485, 1023);
    zone3Sensor1Value = constrain(zone3Sensor1Value, 485, 1023);
    zone3Sensor2Value = constrain(zone3Sensor2Value, 485, 1023);
    zone4Sensor1Value = constrain(zone4Sensor1Value, 485, 1023);
    zone4Sensor2Value = constrain(zone4Sensor2Value, 485, 1023);
    zone1Soil1 = map(zone1Sensor1Value, 485, 1023, 100, 0);
    zone1Soil2 = map(zone1Sensor2Value, 485, 1023, 100, 0);
    zone2Soil1 = map(zone2Sensor1Value, 485, 1023, 100, 0);
    zone2Soil2 = map(zone2Sensor2Value, 485, 1023, 100, 0);
    zone3Soil1 = map(zone3Sensor1Value, 485, 1023, 100, 0);
    zone3Soil2 = map(zone3Sensor2Value, 485, 1023, 100, 0);
    zone4Soil1 = map(zone4Sensor1Value, 485, 1023, 100, 0);
    zone4Soil2 = map(zone4Sensor2Value, 485, 1023, 100, 0);
    zone1Reading = (zone1Soil1 + zone1Soil2)/2;
    zone2Reading = (zone2Soil1 + zone2Soil2)/2;
    zone3Reading = (zone3Soil1 + zone3Soil2)/2;
    zone4Reading = (zone4Soil1 + zone4Soil2)/2;
    }
void circulationPumpOn() {
  digitalWrite(circulationPump, HIGH);
  }
void circulationPumpOff() {
  digitalWrite(resevoirPump, LOW);
  }
void resevoirPumpOn() {
  digitalWrite(resevoirPump, HIGH);
  }
void resevoirPumpOff() {
  digitalWrite(resevoirPump, LOW);
  }
void solenoid1On() {
  digitalWrite(solenoidZone1, LOW);
  }
void solenoid1Off() {
  digitalWrite(solenoidZone1, HIGH);
  }
void solenoid2On() {
  digitalWrite(solenoidZone2, LOW);
  }
void solenoid2Off() {
  digitalWrite(solenoidZone2, HIGH);
  }
void solenoid3On() {
  digitalWrite(solenoidZone3, LOW);
  }
void solenoid3Off() {
  digitalWrite(solenoidZone3, HIGH);
  }
void solenoid4On() {
  digitalWrite(solenoidZone4, LOW);
  }
void solenoid4Off() {
  digitalWrite(solenoidZone4, HIGH);
  }

// ISR ----------
void testButton_ISR() {
    testModeRun = true;
    LCDRefresh = true;
}
void waterButton_ISR(){ 
    waterModeRun = true;
    LCDRefresh = true;
}

// Routines

// LCD Functions
void lcdInfoLine1(String info){
  lcd.setCursor(0,0);
  lcd.print(info);
}
void lcdInfoLine2(String info){
  lcd.setCursor(0,1);
  lcd.print(info);
}
void lcdSystemInfo(String info){
    lcd.print(info);
}
void lcdSystemStart() {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(string_table[10]);
}
void lcdTime() {
    DateTime tt = rtc.now();
    int tthour = tt.hour();
    int ttmin = tt.minute();
    lcd.clear();
    lcd.setCursor(0,0);
    if (tt.hour() < 10)
      lcd.print("0"),
      lcd.print(tt.hour(), DEC);
    else
      lcd.print(tt.hour(), DEC);
      lcd.print(':');
    if (tt.minute() < 10)
      lcd.print("0"),
      lcd.print(tt.minute(), DEC);
    else 
      lcd.print(tt.minute(), DEC);
    
};
void lcdDisplaySensors() {
    lcdTime();
    lcd.print(string_table[2]);
    lcd.setCursor(0,1);
    lcd.print(int (zone1Reading));
    lcd.print("%");
    lcd.setCursor(4,1);
    lcd.print(zone2Reading);
    lcd.print("%");
    lcd.setCursor(8,1);
    lcd.print(zone3Reading);
    lcd.print("%");
    lcd.setCursor(12,1);
    lcd.print(zone4Reading);
    lcd.print("%");
    }
void testModeScreen() {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(string_table[8]);
    LCDRefresh = false;
}
void waterModeScreen() {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(string_table[9]);
    LCDRefresh = false;
}
void automaticModeScreen() {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(string_table[11]);
    LCDRefresh = false;
}

// Date Functions
void setDS3231time(byte second, byte minute, byte hour, byte dayOfWeek, byte
dayOfMonth, byte month, byte year){
  // sets time and date data to DS3231
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set next input to start at the seconds register
  Wire.write(decToBcd(second)); // set seconds
  Wire.write(decToBcd(minute)); // set minutes
  Wire.write(decToBcd(hour)); // set hours
  Wire.write(decToBcd(dayOfWeek)); // set day of week (1=Sunday, 7=Saturday)
  Wire.write(decToBcd(dayOfMonth)); // set date (1 to 31)
  Wire.write(decToBcd(month)); // set month
  Wire.write(decToBcd(year)); // set year (0 to 99)
  Wire.endTransmission();
};

void setup() {
  pinMode(testButton, INPUT_PULLUP);
  pinMode(waterButton, INPUT_PULLUP);
  pinMode(solenoidZone1, OUTPUT);
  pinMode(solenoidZone2, OUTPUT);
  pinMode(solenoidZone3, OUTPUT);
  pinMode(solenoidZone4, OUTPUT);
  pinMode(circulationPump, OUTPUT);
  pinMode(resevoirPump, OUTPUT);
  digitalWrite(solenoidZone1, HIGH);
  digitalWrite(solenoidZone2, HIGH);
  digitalWrite(solenoidZone3, HIGH);
  digitalWrite(solenoidZone4, HIGH);
  Wire.begin();
  rtc.begin();
  lcd.begin (16, 2);
  lcd.setBacklightPin(BACKLIGHT_PIN, POSITIVE);
  lcd.setBacklight(HIGH);
  lcd.home ();
  attachInterrupt(digitalPinToInterrupt(testButton), testButton_ISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(waterButton), waterButton_ISR, FALLING);
  Serial.begin(57600);
  previousTime = millis();
  lcdSystemStart();
}

void loop() {
  currentTime = millis();
  DateTime tt = rtc.now();
  timeHour = tt.hour();
  timeMinute = tt.minute();
  if(((timeHour = Time1) || (timeHour = Time2) || (timeHour = Time3) || (timeHour = Time4) || (timeHour = Time5)) && (timeMinute == TimeMin)) {
    scheduleRun = true;
  }
  if (testModeRun) {
    if (LCDRefresh) {
      testModeScreen();
      previousTime = currentTime;
    }
    if (currentTime - previousTime >= 3000) {
      readSensors();
      lcdDisplaySensors();
      previousTime = currentTime;
      testModeRun = false;
    }
  }
  if (waterModeRun){
     if (LCDRefresh) {
      waterModeScreen();
      previousTime = currentTime;
    }
    if(currentTime - previousTime >= 3000) {
      if(zone1Reading <= zone1MoistureLevel){
        zone1Run = true;
      }
      if(zone2Reading <= zone2MoistureLevel){
        zone2Run = true;
      }
      if(zone3Reading <= zone3MoistureLevel){
        zone3Run = true;
      }
      if(zone4Reading <= zone4MoistureLevel){
        zone4Run = true;
      }
      if((zone1Run) || (zone2Run) || (zone3Run) || (zone4Run)) {
        waterRun = true;
      }
      waterModeRun = false;
      LCDRefresh = true;
      previousTime = currentTime;
      readSensors();
      lcdDisplaySensors();
    }
  }
  
  if(waterRun) {
    int circulationPumpPin = digitalRead(circulationPump);
    if(currentTime - previousTime >= 3000) {
      if(pumpRun == false)  {
        circulationPumpOn();
        if(LCDRefresh) {
          lcd.clear();
          lcdInfoLine1(string_table[7]);
          LCDRefresh = false; 
        }       
        if(currentTime - previousTime >= 4500) {
          pumpRun = true;
          waterRun = false;
          LCDRefresh = true;
        }
      }
    }
  }
  if(pumpRun) {
    int solenoidPin;
    if(zone1Run) {
      solenoidPin = digitalRead(solenoidZone1);
      if(LCDRefresh) {
        lcd.clear();
        lcdInfoLine1(string_table[1]);
        lcdInfoLine2(string_table[3]);
        LCDRefresh = false;
        previousTime = currentTime;
      }
      if(solenoidPin == HIGH) {
        solenoid1On();
      }
      if(currentTime - previousTime >= zone1RunTime) { 
        solenoid1Off();
        zone1Run = false;
        LCDRefresh = true;
      }
    }
    if((zone2Run) &~ (zone1Run)) {
      solenoidPin = digitalRead(solenoidZone2);
      if(LCDRefresh) {
        lcd.clear();
        lcdInfoLine1(string_table[1]);
        lcdInfoLine2(string_table[4]);
        LCDRefresh = false;
        previousTime = currentTime;
      }
      if(solenoidPin == HIGH) {
        solenoid2On();
      }
      if(currentTime - previousTime >= zone2RunTime) { 
        solenoid2Off();
        zone2Run = false;
        LCDRefresh = true;
      }
    }
    if((zone3Run) &~ (zone2Run)) {
      solenoidPin = digitalRead(solenoidZone3);
      if(LCDRefresh) {
        lcd.clear();
        lcdInfoLine1(string_table[1]);
        lcdInfoLine2(string_table[5]);
        LCDRefresh = false;
        previousTime = currentTime;
      }
      if(solenoidPin == HIGH) {
        solenoid3On();
      }
      if(currentTime - previousTime >= zone3RunTime) { 
        solenoid3Off();
        zone3Run = false;
        LCDRefresh = true;
      }
    }
    if((zone4Run) &~ (zone3Run)) {
      solenoidPin = digitalRead(solenoidZone3);
      if(LCDRefresh) {
        lcd.clear();
        lcdInfoLine1(string_table[1]);
        lcdInfoLine2(string_table[6]);
        LCDRefresh = false;
        previousTime = currentTime;
      }
      if(solenoidPin == HIGH) {
        solenoid4On();
      }
      if(currentTime - previousTime >= zone4RunTime) { 
        solenoid4Off();
        zone4Run = false;
        LCDRefresh = true;
      }
    }
    if((pumpRun) &~ (zone4Run)) {
      if(LCDRefresh) {
        lcd.clear();
        lcdInfoLine1(string_table[12]);
        previousTime = currentTime;
        circulationPumpOff();
        LCDRefresh = false;
      }
      if(currentTime - previousTime >= 2000) {
        pumpRun = false;
        zone1Run = false;
        zone2Run = false;
        zone3Run = false;
        zone4Run = false;
        previousTime = currentTime;
        lcdTime();
        lcdSystemInfo(string_table[0]);
      }
    }
  }
  if((currentTime - systemStart >= 1500) && (startUp)){
    lcd.clear();
    lcdTime();
    lcdSystemInfo(string_table[0]);
    startUp = false;
  }
  else if (currentTime - previousTime >= 60000) {
    previousTime = currentTime;
    lcdTime();
    lcdSystemInfo(string_table[0]);
  }
}

