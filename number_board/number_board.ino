//arduino驱动数码管

void setup() {
  // put your setup code here, to run once:
  pinMode(13,OUTPUT);     //激活13号引脚，我们需要用它供电             
  digitalWrite(13, HIGH);
  for(int n=2;n<=9;n++)
  {
    pinMode(n,OUTPUT);
    digitalWrite(n,HIGH);
  }
}

void loop() {
  // put your main code here, to run repeatedly: 
 //这个就是HIGH  和LOW 的另一种写法  0代表LOW   1代表HIGH

  /*==========显示0=============*/
  /*
    digitalWrite(5,LOW);
  delay(1000);
  digitalWrite(5,HIGH);
  delay(1000);
  
   int n0[8]={0,0,0,0,0,0,1,1};       //定义数组 通过0，1定义各数码管的明灭，
   int z=0;
   for(int x=2;x<=9;x++)          //采用循环方式依次点亮指定的数码管led（数组中0为点亮），单位时间只点亮1个led
   {
   digitalWrite(x,n0[z]);         //点亮led语句  X为引脚数  n0[z]为数组z为0-8的变量，来依次读取数组中的值
   z++;
   if (z>=9)                    //防止变量z累加超过了8
   z=0;
   }
   delay(1000);
   */
}


