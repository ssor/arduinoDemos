// button to control led

#define LED_13 11
#define LED_12 11
#define BUTTON 7
int val = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_13, OUTPUT);
  //  pinMode(LED_12, OUTPUT);
  pinMode(BUTTON, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly: 
  val = digitalRead(BUTTON);

  if(val == HIGH){
    digitalWrite(LED_13,LOW);
    for (int a = 0; a <= 255;a ++)                //循环语句，控制PWM亮度的增加
    {
      analogWrite(LED_13,a);
      delay(20);                             //当前亮度级别维持的时间,单位毫秒            
    }
    for (int a = 255; a >= 0;a --)                //循环语句，控制PWM亮度的增加
    {
      analogWrite(LED_13,a);
      delay(20);                             //当前亮度级别维持的时间,单位毫秒            
    }
    //delay(1000);
    //digitalWrite(LED_13,LOW);
  }
}









