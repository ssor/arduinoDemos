//IRemote
#include <IRremote.h>

//****************************************
//
#define GUID "1000000000000001"
#define HEAD "["
#define TAIL "]"
#define FAN 8
#define LED 10
#define INDEX_FAN 8
#define INDEX_LED 10
#define INDEX_TEMPERATURE_SENSOR 14
#define BUTTON_1 0xFF30CF
#define BUTTON_2 0xFF18E7
#define BUTTON_3 0xFF7A85
#define ACTION_OPEN "01"
#define ACTION_CLOSE "02"
#define TEMP_INPUT A0

#define DEBUG 
//****************************************
String msg="";
char c;
int RECV_PIN = 6;//IRemote Port
IRrecv irrecv(RECV_PIN);
decode_results results;
int incomingByte = -1;
//*************************************************
void setup() {
  Serial.begin(9600);
  pinMode(LED,OUTPUT);
  pinMode(FAN,OUTPUT);
  irrecv.enableIRIn(); // Start the receiver
}

void loop() {
  if(Serial.available()){
    while(Serial.available() > 0){
      c = Serial.read();
      msg.concat(c);
    }
    if(msg != ""){
      parseProtocol();
    }
  }
  if (irrecv.decode(&results)) {
    Serial.println(results.value, HEX);
    acceptIR(results.value);
    Serial.println("");
    irrecv.resume(); // Receive the next value
  }

}
//*************************************************************
void parseProtocol(){
  int right = msg.indexOf(']');

  if(right < 0) 
  {
    return;//not complete
  }
  int left = msg.indexOf('[');
  if(left < 0) 
  {
    return;//not complete
  }
  if(left > right){
    msg = msg.substring(left, msg.length());//not a pair
  }
  else{
    String info = msg.substring(left,right+1);
    msg = msg.substring(right+1, msg.length());
    if(info.startsWith("[") && info.endsWith("]")){
      String guid = info.substring(1,17);
      if(guid == GUID){
      }
      else{
        return;
      }
      String protocolType = info.substring(17,19);

      String deviceType = info.substring(19,23);

      String deviceIndex = info.substring(23,25);

      int length = info.length();
      String para = info.substring(25,length-1);

      doActionOnDevice(protocolType, deviceType, deviceIndex, para) ;
    }
  }
  parseProtocol();
}
void doActionOnDevice(String protocolType ,String deviceType, String deviceIndex,String para) {
  //  int i = atoi(deviceIndex);
  int statePin = LOW;
  String pro = "";
  String state = ACTION_CLOSE;
  int i = deviceIndex.toInt();
  if(protocolType == "01"){
    switch(i){
    case INDEX_FAN:
      //
      statePin = digitalRead(INDEX_FAN);
      
      if(statePin == HIGH) state = ACTION_OPEN;
       pro = constructProtocol("02","0001",String(INDEX_FAN),state);
      Serial.println(pro);  
      break;
    case INDEX_LED:
      //
      statePin = digitalRead(INDEX_LED);
      if(statePin == HIGH) state = ACTION_OPEN;
       pro = constructProtocol("02","0001",String(INDEX_LED),state);
      Serial.println(pro);      
      break;
    }
    return;    
  }
  if(protocolType == "03"){
    switch(i){
    case INDEX_FAN:
      if(para == ACTION_OPEN){
        openFan();
      }
      if(para == ACTION_CLOSE){
        closeFan();
      }
      break;
    case INDEX_LED:
      if(para == ACTION_OPEN){
        openLED();
      }
      if(para == ACTION_CLOSE){
        closeLED();
      }
      break;
    }
    return;
  }
  if(protocolType == "05"){
    switch(i){
    case INDEX_TEMPERATURE_SENSOR:
      int temp =getTempValue();
      String pro = constructProtocol("06","0003",String(INDEX_TEMPERATURE_SENSOR),String(temp));
      Serial.println(pro);
      break;
    }
    return;
  }
}
void acceptIR(unsigned long code){
  if(code ==BUTTON_1){
#ifdef DEBUG
    Serial.println("button => 1");
#endif
    openFan();
  }
  if(code ==BUTTON_2){
#ifdef DEBUG
    Serial.println("button => 2");
#endif
    openLED();
  }
  if(code ==BUTTON_3){
#ifdef DEBUG
    Serial.println("button => 3");
#endif
    closeFan();
    closeLED();
  }
}

void openLED(){
#ifdef DEBUG
  Serial.println("openLED =>");
#endif
  digitalWrite(LED,HIGH);
}
void closeLED(){
#ifdef DEBUG
  Serial.println("closeLED =>");
#endif
  digitalWrite(LED,LOW);
}
void openFan(){
#ifdef DEBUG
  Serial.println("openFan =>");
#endif
  digitalWrite(FAN,HIGH);
}
void closeFan(){
#ifdef DEBUG
  Serial.println("closeFan =>");
#endif
  digitalWrite(FAN,LOW);
}

int getTempValue(){
  int valueTemp=0;
  valueTemp = analogRead(TEMP_INPUT);
  valueTemp = valueTemp * 125 / 256;
  return valueTemp;
}
String constructProtocol(String protocolType,String deviceType, String deviceIndex,  String para){
  String info = "";
  info = String(info + HEAD);
  info = String(info + GUID);
  info = String(info + protocolType);
  info = String(info + deviceType);
  info = String(info + deviceIndex);
  info = String(info + para);
  info = String(info + TAIL);
#ifdef DEBUG

#endif
  return info;
}























