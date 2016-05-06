int PWMpin = 3;

void setup()  /****** SETUP: RUNS ONCE ******/
{
  pinMode(PWMpin, OUTPUT);
  Serial.begin(9600);
}//--(end setup )---

void loop()   /****** LOOP: RUNS CONSTANTLY ******/
{
  digitalWrite(PWMpin, HIGH);
  delay(10000);
  digitalWrite(PWMpin, LOW);
  delay(10000);
}

