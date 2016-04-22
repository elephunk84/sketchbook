
#include <DS3232RTC.h>    //http://github.com/JChristensen/DS3232RTC
#include <Time.h>         //http://www.arduino.cc/playground/Code/Time  
#include <Wire.h>         //http://arduino.cc/en/Reference/Wire (included with Arduino IDE)

tmElements_t tm;
const byte rtcAlarmPin =2;

void setup(void)
{
  pinMode(rtcAlarmPin,INPUT_PULLUP);
  Serial.begin(9600);
  Serial.println("DS3231RTC Alarm Test");
  setSyncProvider(RTC.get);   // the function to get the unix time from the RTC
  if(timeStatus() != timeSet) 
    Serial.println("Unable to sync with the RTC");
  else
    Serial.println("RTC has set the system time");
  Serial.println(); 
  
  //enable alarm interrupts on match; also sets status flags A1F,A2F

  RTC.alarmInterrupt(1,1);//enable alarm 1 interrupt A1IE
  RTC.alarmInterrupt(2,1);//enable alarm 2 interrupt A2IE

  //enable or disable the square wave output,
  // no square wave (NONE) sets ITCN bit=1  
  //enables alarm interrupt on square wave pin set from A1F, A2F
  //digitalRead with INPUT_PULLUP; LOW is alarm interrupt

  RTC.squareWave(SQWAVE_NONE);

  
  /*Alarm_Types defined in RTC3232.h.............................................
  
    ALM1_EVERY_SECOND 
    ALM1_MATCH_SECONDS
    ALM1_MATCH_MINUTES     //match minutes *and* seconds
    ALM1_MATCH_HOURS       //match hours *and* minutes, seconds
    ALM1_MATCH_DATE        //match date *and* hours, minutes, seconds
    ALM1_MATCH_DAY         //match day *and* hours, minutes, seconds
    ALM2_EVERY_MINUTE 
    ALM2_MATCH_MINUTES     //match minutes
    ALM2_MATCH_HOURS       //match hours *and* minutes
    ALM2_MATCH_DATE       //match date *and* hours, minutes
    ALM2_MATCH_DAY        //match day *and* hours, minutes
  */
   //setAlarm Format (Alarm_Type, seconds, minutes, hours, date(or day))
  RTC.setAlarm(ALM2_MATCH_HOURS,0,23,20,0);
  RTC.setAlarm(ALM1_MATCH_HOURS,0,24,20,0);

}
void loop()
{
  TimeDateDisplay();
  Serial.print("rtcAlarmPin   ");
  Serial.println(digitalRead(rtcAlarmPin));//resets with status reset
  Serial.print("Alarm1 status  ");
  Serial.println(RTC.alarm(1));//reads and resets status
  Serial.print("Alarm2 status  ");
  Serial.println(RTC.alarm(2));//reads and resets status
  Serial.println();
  delay(5000);
}

void TimeDateDisplay(void)
{
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(' ');
  Serial.print(month());
  Serial.print("/");
  Serial.print(day());
  Serial.print("/");
  Serial.print(year()); 
  Serial.println(); 
}

void printDigits(int digits)
{
  // utility function for TimeDateDisplay prints preceding colon and leading 0
  Serial.print(':');
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}

