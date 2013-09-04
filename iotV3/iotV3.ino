
/*
V3:
 * device type and device index can both be recognized
 V2:
 * HEX Protocol
 V1:
 * control on LED and Fan
 * check state of LED Fan and value of temperature sensor
 * when Led or Fan state changed, new state will be output
 * IR control on Led(BUTTON_2) and Fan(BUTTON_1)
 */

//IRemote
#include <IRremote.h>
#include "iotlib.h"


#define DEBUG

char c;
int RECV_PIN =                     6;//IRemote Port
IRrecv irrecv(RECV_PIN);
decode_results results;

//*************************************************
void setup() {
  Serial.begin(9600);
  pinMode(LED_1,OUTPUT);
  pinMode(FAN_1,OUTPUT);
  irrecv.enableIRIn(); // Start the receiver
}

void loop() {

  if(Serial.available()){
    while(Serial.available() > 0){
      c = Serial.read();
      inputCount ++;
      total_msg[inputCount -1] = c;
    }
    if(inputCount >= PROTOCOL_COUNT){
      inputCount = 0;
      parseProtocol();
      zero_msg();
    }
  }
  ///*
  if (irrecv.decode(&results)) {
#ifdef DEBUG
    Serial.println(results.value, HEX);
    Serial.println("");
#endif
    acceptIR(results.value);

    irrecv.resume(); // Receive the next value
    //*/
  }

}
//*************************************************************

//   0    1    2    3    4     5    6    7   8     9   10   11   12   13   14   15   16   17
//  0x5b 0x30 0x30 0x30 0x30 0x30 0x30 0x30 0x31 0x33 0x30 0x30 ox30 ox31 0x30 ox31 0x31 0x5d
//  5b 30 30 30 30 30 30 30 31 33 30 30 30 31 00 32 31 5d
//  [0000000130001021]   open LED
//  [0000000130001021]   close LED
//  [0000000120001022]   return LED state

//  [0000000150003030]   request temperature
void parseProtocol(){
#ifdef DEBUG
  Serial.println("parseProtocol->");
#endif
  if(total_msg[0] != HEAD || total_msg[PROTOCOL_COUNT-1] != TAIL){
    Serial.println("parseProtocol-> error 1");
    zero_msg();
    return;
  }
  bool bSame = true;
  for(int i = 0; i< 8; i++){
    if(total_msg[i+1] != guid[i]){
      bSame = false;
      break;
    }
  }
  if(bSame == false) return;
  long protocolType = total_msg[9];
  long deviceType = (long)total_msg[13];
  long deviceIndex = (long)total_msg[15];
  long para = total_msg[16];

  doActionOnDevice(protocolType, deviceType, deviceIndex, para) ;
}

void doActionOnDevice(long protocolType ,long deviceType, long deviceIndex,long para) {
#ifdef DEBUG
  Serial.println("doActionOnDevice->");
  Serial.print("protocolType->");
  Serial.println(String(protocolType));
  Serial.print("deviceType->");
  Serial.println(String(deviceType)); 
  Serial.print("deviceIndex->");
  Serial.println(String(deviceIndex)); 
  Serial.print("para->");
  Serial.println(String(para)); 
#endif
  //return;
  switch(protocolType)
  {
  case PRO_CMD_TYPE_CHECK_STATE:
    DoCheckDeviceState(deviceType, deviceIndex, para);
    break;
  case PRO_CMD_TYPE_ACTION_ON_DEVICE:
    DoActionOnDevice(deviceType, deviceIndex, para);
    break;
  case PRO_CMD_TYPE_REQUEST_DEVICE_VALUE:
    DoRequestDeviceValue(deviceType, deviceIndex, para);
    break;
  }
}
void DoActionOnDevice(long deviceType, long deviceIndex,long para)
{
  switch(deviceType){
  case DEVICE_TYPE_FAN:
    if(para == ACTION_OPEN){
      openFan(deviceIndex);
    }
    if(para == ACTION_CLOSE){
      closeFan(deviceIndex);
    }
    break;
  case DEVICE_TYPE_LED:
    if(para == ACTION_OPEN){
      openLED(deviceIndex);
    }
    if(para == ACTION_CLOSE){
      closeLED(deviceIndex);
    }
    break;
  }
}
void DoRequestDeviceValue(long deviceType, long deviceIndex,long para)
{
  switch(deviceType){
  case DEVICE_TYPE_TEM_SENSOR:
    int temp =getTempValue(deviceIndex);
    char pro_temp[PROTOCOL_COUNT+1];
    pro_temp[0] = '[';

    for(int i = 0; i < 8; i++){
      pro_temp[i+1] = guid[i];
    }// 1->8
    pro_temp[9] = PRO_CMD_TYPE_STATE_RETURN;
    pro_temp[10] = '0'; 
    pro_temp[11] = '0';
    pro_temp[12] = '0';
    pro_temp[13] = deviceType;
    pro_temp[14] = '0';
    pro_temp[15] = deviceIndex;
    pro_temp[16] = temp;
    pro_temp[17] = ']';
    pro_temp[PROTOCOL_COUNT] = 0;
    Serial.println(pro_temp);  
    break;
  }
}
void DoCheckDeviceState(long deviceType, long deviceIndex,long para)
{
#ifdef DEBUG
  Serial.print("check device state => ");
  Serial.println(deviceType);
#endif
  switch(deviceType){
  case DEVICE_TYPE_ALL:
    outputAllDeviceState();
    break;
  case DEVICE_TYPE_FAN:
    outputDeviceState(INDEX_FAN_1, DEVICE_TYPE_FAN); 
    break;
  case DEVICE_TYPE_LED:
    outputDeviceState(INDEX_LED_1, DEVICE_TYPE_LED);    
    break;
  case DEVICE_TYPE_TEM_SENSOR:

    break;
  }
}
void outputAllDeviceState(){
  outputDeviceState(INDEX_FAN_1, DEVICE_TYPE_FAN); 
  outputDeviceState(INDEX_LED_1, DEVICE_TYPE_LED);
}

void outputDeviceState(int deviceIndex, int deviceType){

  int statePin = digitalRead(GetPinMatchDeviceIndex(deviceType,deviceIndex));
  int state = ACTION_CLOSE;
  if(statePin == HIGH) state = ACTION_OPEN;
  char pro_temp[PROTOCOL_COUNT+1];
  pro_temp[0] = '[';
  pro_temp[17] = ']';
  //  strcpy(pro_temp,guid);
  for(int i = 0; i < 8; i++){
    pro_temp[i+1] = guid[i];
  }// 1->8
  pro_temp[9] = PRO_CMD_TYPE_STATE_RETURN;
  pro_temp[10] = '0'; 
  pro_temp[11] = '0';
  pro_temp[12] = '0';
  pro_temp[13] = deviceType;
  pro_temp[14] = '0';
  pro_temp[15] = deviceIndex;
  pro_temp[16] = state;
  pro_temp[PROTOCOL_COUNT] = 0;
  Serial.println(pro_temp);  
}

void openLED(int led_index){
#ifdef DEBUG
  Serial.println("openLED =>");
#endif
  //  digitalWrite(LED_1,HIGH);
  digitalWrite(GetPinMatchDeviceIndex(DEVICE_TYPE_LED,led_index), HIGH);
  outputDeviceState(led_index, DEVICE_TYPE_LED);
}
void closeLED(int led_index){
#ifdef DEBUG
  Serial.println("closeLED =>");
#endif
  //  digitalWrite(LED_1,LOW);
  digitalWrite(GetPinMatchDeviceIndex(DEVICE_TYPE_LED,led_index), LOW);
  outputDeviceState(led_index, DEVICE_TYPE_LED);
}
void openFan(int fan_index){
#ifdef DEBUG
  Serial.println("openFan =>");
#endif
  //  digitalWrite(FAN_1,HIGH);
  digitalWrite(GetPinMatchDeviceIndex(DEVICE_TYPE_FAN,fan_index), HIGH);
  outputDeviceState(fan_index, DEVICE_TYPE_FAN); 
}
void closeFan(int fan_index){
#ifdef DEBUG
  Serial.println("closeFan =>");
#endif
  //  digitalWrite(FAN_1,LOW);
  digitalWrite(GetPinMatchDeviceIndex(DEVICE_TYPE_FAN,fan_index), LOW);
  outputDeviceState(fan_index, DEVICE_TYPE_FAN); 
}
int getTempValue(int index){
  int valueTemp=0;
  valueTemp = analogRead(GetPinMatchDeviceIndex(DEVICE_TYPE_TEM_SENSOR, index));
  valueTemp = valueTemp * 125 / 256;
  return valueTemp + 32;//adjust to could-be-printed ascii
}
void acceptIR(unsigned long code){
  if(code ==BUTTON_1){
#ifdef DEBUG
    Serial.println("button => 1");
#endif
    int fan_pin = GetPinMatchDeviceIndex(DEVICE_TYPE_FAN, INDEX_FAN_1);
    if(HIGH == digitalRead(fan_pin)){
      closeFan(INDEX_FAN_1);
    }
    else{
      openFan(INDEX_FAN_1);
    }
  }
  if(code ==BUTTON_2){
#ifdef DEBUG
    Serial.println("button => 2");
#endif
    int led_pin = GetPinMatchDeviceIndex(DEVICE_TYPE_LED, INDEX_LED_1);
    if(HIGH == digitalRead(led_pin)){
      closeLED(INDEX_LED_1);
    }
    else{
      openLED(INDEX_LED_1);
    }
  }
  if(code == BUTTON_POWER){
#ifdef DEBUG
    Serial.println("button => POWER");
#endif
    closeFan(INDEX_FAN_1);
    closeLED(INDEX_LED_1);
  }
}






















































