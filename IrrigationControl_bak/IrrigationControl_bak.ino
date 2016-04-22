#include <DS3231.h>
#include "Wire.h"
#define DS3231_I2C_ADDRESS 0x68
// Convert normal decimal numbers to binary coded decimal
byte decToBcd(byte val)
{
  return( (val/10*16) + (val%10) );
}
// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val)
{
  return( (val/16*10) + (val%16) );
}

int CirculationPump = 5;
int ResevoirPump = 4;
int Zone1Solenoid = 50;
int Zone1Sensors[] = {A12, A8};
int Zone2Solenoid = 48;
int Zone2Sensors[] = {A13, A9};
int Zone3Solenoid = 44;
int Zone3Sensors[] = {A14, A10};
int Zone4Solenoid = 46;
int Zone4Sensors[] = {A15, A11};
int soil1 = 0;
int soil2 = 0; 
int SensorCount = 2;

void setup() {
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
  Serial.begin(9600);
  // setDS3231time(30,42,21,4,26,11,14);
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

void read_sensors()  {
    displayTime();
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
    delay(1000);        // delay in between reads for stability
    displayTime();
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
    delay(1000);        // delay in between reads for stability
    displayTime();
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
    delay(1000);
    displayTime();
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
    delay(1000);        // delay in between reads for stability
    }
    
void test_solenoids() {
  displayTime();
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

void loop() {
  test_solenoids();
}
