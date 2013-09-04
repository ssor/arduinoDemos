/*
V1:
 * control on LED and Fan
 * check state of LED Fan and value of temperature sensor
 * when Led or Fan state changed, new state will be output
 * IR control on Led(BUTTON_2) and Fan(BUTTON_1)
 */



//#define DEBUG 


//IRemote
#include <IRremote.h>



//*******************************--*******
#define GUID                                   "1000000000000001"
#define HEAD                                   "["
#define TAIL                                   "]"

#define PRO_CMD_TYPE_CHECK_STATE               "01"
#define PRO_CMD_TYPE_STATE_RETURN              "02"
#define PRO_CMD_TYPE_ACTION_ON_DEVICE          "03"
#define PRO_CMD_TYPE_ACTION_ON_DEVICE_RETURN   "04"
#define PRO_CMD_TYPE_REQUEST_DEVICE_VALUE      "05"
#define PRO_CMD_TYPE_DEVICE_VALUE_RETURN       "06"

#define DEVICE_TYPE_LED                        "0001"
#define DEVICE_TYPE_FAN                        "0002"
#define DEVICE_TYPE_TEM_SENSOR                 "0003"

#define ACTION_OPEN                            "01"
#define ACTION_CLOSE                           "02"

#define FAN                        8
#define LED                        10
#define INDEX_ALL                  0
#define INDEX_FAN                  8
#define INDEX_LED                  10
#define INDEX_TEMPERATURE_SENSOR   14
#define BUTTON_1                   0xFF30CF
#define BUTTON_2                   0xFF18E7
#define BUTTON_3                   0xFF7A85
#define BUTTON_POWER               0xFFA25D

#define TEMP_INPUT                 A0


//****************************************
String msg =                       "";
char c;
int RECV_PIN =                     6;//IRemote Port
IRrecv irrecv(RECV_PIN);
decode_results results;
int incomingByte =                 -1;
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
#ifdef DEBUG
    Serial.println(results.value, HEX);
    Serial.println("");
#endif
    acceptIR(results.value);

    irrecv.resume(); // Receive the next value
  }

}
//*************************************************************
//  [10000000000000010300011001]   open LED
//  [10000000000000010200011002]
void parseProtocol(){
  int right = msg.indexOf(TAIL);

  if(right < 0) 
  {
    return;//not complete
  }
  int left = msg.indexOf( HEAD );
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
    if(info.startsWith(HEAD) && info.endsWith(TAIL)){
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
  int i = deviceIndex.toInt();
  String pro;
  String state;
  int statePin;
  if(protocolType == PRO_CMD_TYPE_CHECK_STATE){
#ifdef DEBUG
    Serial.print("check device state => ");
    Serial.println(i);
#endif
    switch(i){
    case INDEX_ALL:
      outputAllDeviceState();
      break;
    case INDEX_FAN:
      outputDeviceState(INDEX_FAN, DEVICE_TYPE_FAN); 
      break;
    case INDEX_LED:
      outputDeviceState(INDEX_LED, DEVICE_TYPE_LED);    
      break;

    }
    return;    
  }
  if(protocolType == PRO_CMD_TYPE_ACTION_ON_DEVICE){
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
  if(protocolType == PRO_CMD_TYPE_REQUEST_DEVICE_VALUE){
    switch(i){
    case INDEX_TEMPERATURE_SENSOR:
      int temp =getTempValue();
      String pro = constructProtocol(PRO_CMD_TYPE_DEVICE_VALUE_RETURN,DEVICE_TYPE_TEM_SENSOR,String(INDEX_TEMPERATURE_SENSOR),String(temp));
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
    if(HIGH == digitalRead(INDEX_FAN)){
      closeFan();
    }
    else{
      openFan();
    }
  }
  if(code ==BUTTON_2){
#ifdef DEBUG
    Serial.println("button => 2");
#endif
    if(HIGH == digitalRead(INDEX_LED)){
      closeLED();
    }
    else{
      openLED();
    }
  }
  if(code == BUTTON_POWER){
#ifdef DEBUG
    Serial.println("button => POWER");
#endif
    closeFan();
    closeLED();
  }
}
void outputAllDeviceState(){
  outputDeviceState(INDEX_FAN, DEVICE_TYPE_FAN); 
  outputDeviceState(INDEX_LED, DEVICE_TYPE_LED);
}
void outputDeviceState(int deviceIndex, String deviceType){
  int statePin = digitalRead(deviceIndex);
  String state = ACTION_CLOSE;
  if(statePin == HIGH) state = ACTION_OPEN;
  String strIndex = String(deviceIndex);
  if(deviceIndex < 10){
    strIndex = "0";
    strIndex = strIndex + String(deviceIndex);
  }
  String pro = constructProtocol(PRO_CMD_TYPE_STATE_RETURN,deviceType,strIndex,state);
  Serial.println(pro);  
}
void openLED(){
#ifdef DEBUG
  Serial.println("openLED =>");
#endif
  digitalWrite(LED,HIGH);
  outputDeviceState(INDEX_LED, DEVICE_TYPE_LED);
}
void closeLED(){
#ifdef DEBUG
  Serial.println("closeLED =>");
#endif
  digitalWrite(LED,LOW);
  outputDeviceState(INDEX_LED, DEVICE_TYPE_LED);
}
void openFan(){
#ifdef DEBUG
  Serial.println("openFan =>");
#endif
  digitalWrite(FAN,HIGH);
  outputDeviceState(INDEX_FAN, DEVICE_TYPE_FAN); 
}
void closeFan(){
#ifdef DEBUG
  Serial.println("closeFan =>");
#endif
  digitalWrite(FAN,LOW);
  outputDeviceState(INDEX_FAN, DEVICE_TYPE_FAN); 
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






































