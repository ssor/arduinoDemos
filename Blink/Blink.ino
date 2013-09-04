/*
  Blink
 Turns on an LED on for one second, then off for one second, repeatedly.
 
 This example code is in the public domain.
 */
#include "test.h"
// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
#define TEMP_INPUT                 A0
int led = 13;
int incomingByte = -1;
int inputCount = 0;
char msg[128] = {
  0};
char guid[] =  {
  '0','0','0','0','0','0','0','1'};
int var = 0;
// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  Serial.begin(9600);
  pinMode(led, OUTPUT);   
  var = test_var();
}

// the loop routine runs over and over again forever:
void loop() {
  //  Serial.println("*** open led ...");
  //  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  //  delay(1000);               // wait for a second
  //  Serial.println("*** close led ...");
  //  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  //  delay(1000);               // wait for a second
  //  
  if(Serial){
    incomingByte = Serial.read();

    if(incomingByte >= 0){
      //      long deviceType = 0L;
      //      deviceType += (long)(1000 + 100 + 10) * 0x30 + 0x31;
      //      Serial.print("deviceType->");
      //      Serial.println(String(deviceType)); 
      //      int test = 52800;
      //      Serial.print("int 52800->");
      //      Serial.println(String(test)); 
      //      long test1 = 52800;
      //      Serial.print("long 52800->");
      //      Serial.println(String(test1)); 
      Serial.print("var->");
      Serial.println(String(g_var));
      /*   
       int temp = getTempValue();
       Serial.print("temp->");
       Serial.println(String(temp)); 
       
       inputCount++;
       msg[inputCount-1] = incomingByte;
       //msg.concat(incomingByte);
       if(inputCount>=8)
       {
       int re = strcmp(guid,msg);
       if(re == 0)
       {
       Serial.println("ok");
       }
       else{
       Serial.println("no");
       }
       }
       */

      //      Serial.print(incomingByte);
      //      Serial.print(incomingByte,DEC);
      //      Serial.println("]");
    }
  }
}
int getTempValue(){
  int valueTemp=0;
  valueTemp = analogRead(TEMP_INPUT);
  valueTemp = valueTemp * 125 / 256;
  return valueTemp + 64;
}

















