#define LED 13

int val = 0;
void setup() {
  // put your setup code here, to run once:
  pinMode(LED,OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly: 
  val = analogRead(0);
  Serial.println(val);
  analogWrite(LED,val/4);
  //  digitalWrite(LED, HIGH);
  //  delay(val);
  //  digitalWrite(13,LOW);
  delay(10);
}


