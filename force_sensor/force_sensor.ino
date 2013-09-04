#define ANA_COUNT 20

int realvs[ANA_COUNT];
int index = 0;
float temp;   //创建一个浮点型变量temp作为存储空间准备存放数据
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly: 
  int V1 = analogRead(A0);                    
  //从A0口读取电压数据存入刚刚创建整数型变量V1，模拟口的电压测量范围为0-5V 返回的值为0-1024
  float vol = V1*(5.0 / 1023.0); 
  if(index < ANA_COUNT){
    realvs[index] = (5-vol) * 100;
    index++;
    Serial.print(index);
  }
  else{
    index = 0;
    int sum = 0;
    for(int i=0;i<ANA_COUNT;i++){
      sum += realvs[i];
      realvs[i] = 0;
    }
    temp = sum/ANA_COUNT;
    Serial.println("=> "); 
    Serial.print(temp);                       //串口输出电压值，并且不换行  
    Serial.println(" V");                    //串口输出字符V，并且换行
  }
  delay(500);
  /*
  if (vol == temp)                             
    //这部分的判断是用来过滤重复的数据，只有本次的电压值和上次不一时才进行输出
  {
    temp = vol;                               //比较完成后，将这次的值存入比对比用的变量temp
  }
  else
  {
    Serial.print(vol);                       //串口输出电压值，并且不换行  
    Serial.println(" V");                    //串口输出字符V，并且换行
    temp = vol;
    delay(1000);                           //输出完成后等待1秒钟，用于控制数据的刷新速度。
  }
  */
}


