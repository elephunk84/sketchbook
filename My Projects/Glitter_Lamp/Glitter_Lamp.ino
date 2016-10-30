int LEDpotPin = A0;   
int LEDpotValue = 0;  
int ledPIN = 9;     
int LEDoutputValue = 0;
int MOTORpotPin = A1;   
int MOTORpotValue = 0;  
int motorPIN = 10;     
int MOTORoutputValue = 0;
 
void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
  // declare the led pin as an output:
  pinMode(ledPIN, OUTPUT);
  pinMode(motorPIN, OUTPUT);
}
 
void loop() {
  LEDpotValue = analogRead(LEDpotPin); // read the pot value
  MOTORpotValue = analogRead(MOTORpotPin); // read the pot value
  LEDoutputValue = map(LEDpotValue, 0, 1023, 0, 255);
  MOTORoutputValue = map(MOTORpotValue, 0, 1023, 0, 255);
  analogWrite(ledPIN, LEDoutputValue);  // PWM the LED with the pot value (divided by 4 to fit in a byte)
  analogWrite(motorPIN, MOTORoutputValue);  // PWM the LED with the pot value (divided by 4 to fit in a byte)
  }
 
