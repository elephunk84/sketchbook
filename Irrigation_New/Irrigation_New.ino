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
  "YES",
  "NO",
  "Interrupt",
  "Test mode active",
  "Manual Watering",
  };


/*-Read Sensors
-Control Resevoir Pump
-Control Circulation Pump
-Control Solenoid 1
-Control Button 1
*-Test Mode
-Control Button 2
* -Manual Water
* -Short Press Test and water
* -Long Press Manual Water for length of button press
-Read Date & Time
-Set Date & Time
-Control LCD
* -System start
* -Sleep
* -Testing
* -Watering Zone 1
* -Watering Zone 2
* -Watering Zone 3
* -Watering Zone 4 
**
**
-RTC Alarm
-Sleep
*/


const unsigned long Delay1 = 1000;
const unsigned long Delay2 = 2000;
const unsigned long Delay3 = 3000;
const unsigned long Delay4 = 4000;
const unsigned long Delay5 = 5000;
const unsigned long sysDelay = 1500;
const unsigned long LCDDelay = 6000;
unsigned long previousTime = 0;
unsigned long interval = 3000;
unsigned long currentMillis;

const byte testButton = 2;
volatile byte testButtonFlag = LOW;
const byte waterButton = 3;
volatile byte waterButtonFlag = LOW;
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

void testButton_ISR() {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(string_table[16]);
    myDelay(3000);
}

void waterButton_ISR() {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(string_table[17]);
    myDelay(3000);
}
    
void lcd_time() {
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

void displaySensors() {
    lcd_time();
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
    delay(5000);
}

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

byte decToBcd(byte val)
{
  return( (val/10*16) + (val%10) );
}
// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val)
{
  return( (val/16*10) + (val%16) );
}

void setDS3231time(byte second, byte minute, byte hour, byte dayOfWeek, byte
dayOfMonth, byte month, byte year)
{
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

void myDelay(int x)   {
  for(int i=0; i<=x; i++)   
  {
    delayMicroseconds(1000);
  }
}

void setup() {
    pinMode(testButton, INPUT_PULLUP);
    pinMode(waterButton, INPUT_PULLUP);
    Wire.begin();
    rtc.begin();
    lcd.begin (16,2); 
    lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
    lcd.setBacklight(HIGH);
    lcd.home ();
    attachInterrupt(0, testButton_ISR, FALLING);
    attachInterrupt(1, waterButton_ISR, FALLING);
    Serial.begin(57600);
}

void loop() {
    unsigned long currentTime = millis();
    if (currentTime - previousTime > interval) {
        previousTime = currentTime;
        readSensors();
        displaySensors();
    } 
    } 
