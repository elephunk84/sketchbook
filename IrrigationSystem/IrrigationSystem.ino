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

int n = 1;
int solenoid;
String water_info;
int delay_time;
int soil1;
int soil2; 
int SensorCount = 2;
int CirculationPump = 5;
int ResevoirPump = 4;
/*Start of User Defined Settings
*/
// Time1-5 are the hours for program to run (Sensors and watering)
int Time1 = 8;
int Time2 = 23;
int Time3;
int Time4;
int Time5;
// TimeMin is the minute on the hour for program to run.
int TimeMin = 33;
// Water levels are soil moisture settings in percent. So if the moisture in the soil reads less than WaterLevel
// then the program wil run.
int WaterLevel_1 = 60;
int WaterLevel_2 = 60;
int WaterLevel_3 = 60;
int WaterLevel_4 = 60;
int WaterLevel_5 = 60;
// ZoneSettings------
/* Only user serviceable settings are
   -Zone*Delay = Watering time in milliseconds so 1000 is 1sec and 30000 is 30sec
   -Zone*WaterLevel = Moisture Level you want the zone set to, link to WaterLevels above
*/
int Zone1Solenoid = 50;
int Zone1Sensors[] = {A12, A8};
int Zone1Delay = 30000;
int Zone1SensorValue;
int Zone1Water = 'NO';
int Zone1WaterLevel = WaterLevel_1;
int Zone2Solenoid = 48;
int Zone2Sensors[] = {A13, A9};
int Zone2Delay = 30000;
int Zone2SensorValue;
int Zone2Water = 'NO';
int Zone2WaterLevel = WaterLevel_1;
int Zone3Solenoid = 44;
int Zone3Sensors[] = {A14, A10};
int Zone3Delay = 30000;
int Zone3SensorValue;
int Zone3Water = 'NO';
int Zone3WaterLevel = WaterLevel_1;
int Zone4Solenoid = 46;
int Zone4Sensors[] = {A15, A11};
int Zone4Delay = 30000;
int Zone4SensorValue;
int Zone4Water = 'NO';
int Zone4WaterLevel = WaterLevel_1;

LiquidCrystal_I2C	lcd(I2C_ADDR,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin);

RTC_DS3231 rtc;
int INTERRUPT_PIN = 22;
volatile int state = LOW;

const char *string_table[] =
{   
  "   Sleeping",
  "   Watering",
  "    Testing",
  "  Zone 1 For 30s",
  "  Zone 2 For 30s",
  "  Zone 3 For 30s",
  "  Zone 4 For 30s",
  "",
  "",
  };

byte decToBcd(byte val)
{
  return( (val/10*16) + (val%10) );
}
// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val)
{
  return( (val/16*10) + (val%16) );
}

void setup () {
  // Set Time (sec,min,hour,day,date,mon,yr)
  //setDS3231time(00,22,18,5,21,04,16); // Set time
  pinMode(INTERRUPT_PIN, INPUT_PULLUP); 
  pinMode(CirculationPump, OUTPUT);
  pinMode(ResevoirPump, OUTPUT);
  pinMode(Zone1Solenoid, OUTPUT);
  pinMode(Zone2Solenoid, OUTPUT);
  pinMode(Zone3Solenoid, OUTPUT);
  pinMode(Zone4Solenoid, OUTPUT); 
  digitalWrite(Zone1Solenoid, HIGH);
  digitalWrite(Zone2Solenoid, HIGH);
  digitalWrite(Zone3Solenoid, HIGH);
  digitalWrite(Zone4Solenoid, HIGH); 
  Wire.begin();
  rtc.begin();
  lcd.begin (16,2); 
  lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
  lcd.setBacklight(HIGH);
  lcd.home ();
  Serial.begin(57600);
}

void program() {
  DateTime tt = rtc.now();
  int tthour = tt.hour();
  int ttmin = tt.minute();
  Serial.print(tt.hour(), DEC);
  Serial.print(':');
  Serial.print(tt.minute(), DEC);
  Serial.print(':');
  Serial.println(tt.second(), DEC);
  if ((tthour == Time1 || Time2 || Time3 || Time4 || Time5 ) && ttmin == TimeMin ){
    water_run();
  }
  else {
  lcd_time(),
  lcd.print(string_table[0]),
  delay(1000),
  Serial.flush();
  } 
  }

void water_run(){
    lcd_time(),
    lcd.print(string_table[2]),
    read_sensors();
    lcd.setCursor(0,1);
    lcd.print(int (Zone1SensorValue));
    lcd.print("%"),
    lcd.setCursor(4,1),
    lcd.print(Zone2SensorValue),
    lcd.print("%"),
    lcd.setCursor(8,1),
    lcd.print(Zone3SensorValue),
    lcd.print("%"),
    lcd.setCursor(12,1),
    lcd.print(Zone4SensorValue),
    lcd.print("%"),
    delay(5000);
    lcd_time();
    lcd.print(string_table[8]);
    if (Zone1SensorValue <= Zone1WaterLevel ) {
      Zone1Water = 'YES';
    };
    if (Zone2SensorValue <= Zone2WaterLevel ) {
      Zone2Water = 'YES';
    };
    if (Zone3SensorValue <= Zone3WaterLevel ) {
      Zone3Water = 'YES';
    };
    if (Zone4SensorValue <= Zone4WaterLevel ) {
      Zone4Water = 'YES';
    };
    if (Zone1Water || Zone2Water || Zone3Water || Zone4Water == 'YES'){
      digitalWrite(CirculationPump, HIGH);
      delay(1500);
    };
    if (Zone1Water == 'YES'){
      delay_time = Zone1Delay,
      solenoid = Zone1Solenoid,
      water_info = string_table[3], 
      water_routine(); 
    };
    if (Zone2Water == 'YES'){
      delay_time = Zone2Delay,
      solenoid = Zone2Solenoid,
      water_info = string_table[4],
      water_routine();  
    };
    if (Zone3Water == 'YES'){
      delay_time = Zone3Delay,
      solenoid = Zone3Solenoid,
      water_info = string_table[5],
      water_routine();  
    };
    if (Zone4Water == 'YES'){
      delay_time = Zone4Delay,
      solenoid = Zone4Solenoid,
      water_info = string_table[6],
      water_routine();  
    }; 
    digitalWrite(CirculationPump, LOW);
    Zone1Water = 'NO';
    Zone2Water = 'NO';
    Zone3Water = 'NO';
    Zone4Water = 'NO';
}

void water_routine() {
  lcd_time(),
  lcd.print(string_table[1]),
  digitalWrite(solenoid, LOW),
  lcd.setCursor(0,1),
  lcd.print(water_info);
  delay(delay_time),
  digitalWrite(solenoid, HIGH);
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
    
}

void read_sensors()  {
    int Zone1Sensor1Value = analogRead(Zone1Sensors[0]);
    Zone1Sensor1Value = constrain(Zone1Sensor1Value, 485, 1023);
    int Zone1Sensor2Value = analogRead(Zone1Sensors[1]);
    Zone1Sensor2Value = constrain(Zone1Sensor2Value, 485, 1023);
    soil1 = map(Zone1Sensor1Value, 485, 1023, 100, 0);
    Serial.println("///---------------------Zone 1");
    Serial.println("Sensor 1 Value ");
    Serial.print(soil1);
    Serial.println("%");
    soil2 = map(Zone1Sensor2Value, 485, 1023, 100, 0);
    Serial.println("Sensor 2 Value ");
    Serial.print(soil2);
    Serial.println("%");
    Zone1Sensor1Value = soil1;
    Zone1Sensor2Value = soil2;
    Zone1SensorValue = (soil1 + soil2) / 2;
    int Zone2Sensor1Value = analogRead(Zone2Sensors[0]);
    Zone2Sensor1Value = constrain(Zone2Sensor1Value, 485, 1023);
    int Zone2Sensor2Value = analogRead(Zone2Sensors[1]);
    Zone2Sensor2Value = constrain(Zone2Sensor2Value, 485, 1023);
    soil1 = map(Zone2Sensor1Value, 485, 1023, 100, 0);
    Serial.println("///---------------------Zone 2");
    Serial.println("Sensor 1 Value ");
    Serial.print(soil1);
    Serial.println("%");
    soil2 = map(Zone2Sensor2Value, 485, 1023, 100, 0);
    Serial.println("Sensor 2 Value ");
    Serial.print(soil2);
    Serial.println("%");
    Zone2Sensor1Value = soil1;
    Zone2Sensor2Value = soil2;
    Zone2SensorValue = (soil1 + soil2) / 2;
    int Zone3Sensor1Value = analogRead(Zone3Sensors[0]);
    Zone3Sensor1Value = constrain(Zone3Sensor1Value, 485, 1023);
    int Zone3Sensor2Value = analogRead(Zone3Sensors[1]);
    Zone3Sensor2Value = constrain(Zone3Sensor2Value, 485, 1023);
    soil1 = map(Zone3Sensor1Value, 485, 1023, 100, 0);
    Serial.println("///---------------------Zone 3");
    Serial.println("Sensor 1 Value ");
    Serial.print(soil1);
    Serial.println("%");
    soil2 = map(Zone3Sensor2Value, 485, 1023, 100, 0);
    Serial.println("Sensor 2 Value ");
    Serial.print(soil2);
    Serial.println("%");
    Zone3Sensor1Value = soil1;
    Zone3Sensor2Value = soil2;
    Zone3SensorValue = (soil1 + soil2) / 2;
    int Zone4Sensor1Value = analogRead(Zone4Sensors[0]);
    Zone4Sensor1Value = constrain(Zone4Sensor1Value, 485, 1023);
    int Zone4Sensor2Value = analogRead(Zone4Sensors[1]);
    Zone4Sensor2Value = constrain(Zone4Sensor2Value, 485, 1023);
    soil1 = map(Zone4Sensor1Value, 485, 1023, 100, 0);
    Serial.println("///---------------------Zone 4");
    Serial.println("Sensor 1 Value ");
    Serial.print(soil1);
    Serial.println("%");
    soil2 = map(Zone4Sensor2Value, 485, 1023, 100, 0);
    Serial.println("Sensor 2 Value ");
    Serial.print(soil2);
    Serial.println("%");
    Zone4Sensor1Value = soil1;
    Zone4Sensor2Value = soil2;
    Zone4SensorValue = (soil1 + soil2) / 2;
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
}
void readDS3231time(byte *second,
byte *minute,
byte *hour,
byte *dayOfWeek,
byte *dayOfMonth,
byte *month,
byte *year)
{
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set DS3231 register pointer to 00h
  Wire.endTransmission();
  Wire.requestFrom(DS3231_I2C_ADDRESS, 7);
  // request seven bytes of data from DS3231 starting from register 00h
  *second = bcdToDec(Wire.read() & 0x7f);
  *minute = bcdToDec(Wire.read());
  *hour = bcdToDec(Wire.read() & 0x3f);
  *dayOfWeek = bcdToDec(Wire.read());
  *dayOfMonth = bcdToDec(Wire.read());
  *month = bcdToDec(Wire.read());
  *year = bcdToDec(Wire.read());
}

void displayTime()
{
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  // retrieve data from DS3231
  readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month,
  &year);
  // send it to the serial monitor
  Serial.print(hour, DEC);
  // convert the byte variable to a decimal number when displayed
  Serial.print(":");
  if (minute<10)
  {
    Serial.print("0");
  }
  Serial.print(minute, DEC);
  Serial.print(":");
  if (second<10)
  {
    Serial.print("0");
  }
  Serial.print(second, DEC);
  Serial.print(" ");
  Serial.print(dayOfMonth, DEC);
  Serial.print("/");
  Serial.print(month, DEC);
  Serial.print("/");
  Serial.print(year, DEC);
  Serial.print(" Day of week: ");
  switch(dayOfWeek){
  case 1:
    Serial.println("Sunday");
    break;
  case 2:
    Serial.println("Monday");
    break;
  case 3:
    Serial.println("Tuesday");
    break;
  case 4:
    Serial.println("Wednesday");
    break;
  case 5:
    Serial.println("Thursday");
    break;
  case 6:
    Serial.println("Friday");
    break;
  case 7:
    Serial.println("Saturday");
    break;
  }
}
    
void testSolenoids() {
    digitalWrite(Zone1Solenoid, LOW);
    Serial.println("Zone 1 On");
    delay(1000);
    digitalWrite(Zone1Solenoid, HIGH);
    Serial.println("Zone 1 Off");
    delay(1000);
    digitalWrite(Zone2Solenoid, LOW);
    Serial.println("Zone 2 On");
    delay(1000);
    digitalWrite(Zone2Solenoid, HIGH);
    Serial.println("Zone 2 Off");    
    delay(1000);
    digitalWrite(Zone3Solenoid, LOW);
    Serial.println("Zone 3 On");    
    delay(1000);
    digitalWrite(Zone3Solenoid, HIGH);
    Serial.println("Zone 3 Off");
    delay(1000);
    digitalWrite(Zone4Solenoid, LOW);
    Serial.println("Zone 4 On");
    delay(1000);
    digitalWrite(Zone4Solenoid, HIGH);
    Serial.println("Zone 4 Off");
    delay(1000);
}

void loop(){
  program();
}
  
