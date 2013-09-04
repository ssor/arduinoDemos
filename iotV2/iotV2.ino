
/*
V2:
 * HEX Protocol
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
#define GUID                                   "00000001"
#define HEAD                                   '['
#define TAIL                                   ']'

#define PRO_CMD_TYPE_CHECK_STATE               0x31 //"1"
#define PRO_CMD_TYPE_STATE_RETURN              0x32 //"2"
#define PRO_CMD_TYPE_ACTION_ON_DEVICE          0x33 //"3"
#define PRO_CMD_TYPE_ACTION_ON_DEVICE_RETURN   0x34 //"4"
#define PRO_CMD_TYPE_REQUEST_DEVICE_VALUE      0x35 //"5"
#define PRO_CMD_TYPE_DEVICE_VALUE_RETURN       0x36 //"6"

#define DEVICE_TYPE_ALL                        0x30 //256 * 0x30 + 0x31 //"0001"
#define DEVICE_TYPE_LED                        0x31 //256 * 0x30 + 0x31 //"0001"
#define DEVICE_TYPE_FAN                        0x32 //256 * 0x30 + 0x32 //"0002"
#define DEVICE_TYPE_TEM_SENSOR                 0x33 //256 * 0x30 + 0x33 //"0003"

#define ACTION_OPEN                            0x31
#define ACTION_CLOSE                           0x32

#define FAN                        8
#define LED                        10
#define INDEX_ALL                  0x30
#define INDEX_FAN                  0x31
#define INDEX_LED                  0x32
#define INDEX_TEMPERATURE_SENSOR   0x33
#define BUTTON_1                   0xFF30CF
#define BUTTON_2                   0xFF18E7
#define BUTTON_3                   0xFF7A85
#define BUTTON_POWER               0xFFA25D

#define TEMP_INPUT                 A0
#define PROTOCOL_COUNT             18

//#define DEBUG
// "00000001"
char guid[] = {
  0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x31};
char device_type_led[] = {
  0x30,0x30,0x30,0x31};
char device_tyype_fan[] = {
  0x30,0x30,0x30,0x32};
char device_type_tem_sensor[] = {
  0x30,0x30,0x30,0x33};
char action_open = 0x31;
char action_close = 0x32;
//****************************************

String msg =                       "";
char c;
int RECV_PIN =                     6;//IRemote Port
IRrecv irrecv(RECV_PIN);
decode_results results;
int incomingByte =                 -1;
char total_msg[PROTOCOL_COUNT];
int inputCount = 0;
//*************************************************
void setup() {
  Serial.begin(9600);
  pinMode(LED,OUTPUT);
  pinMode(FAN,OUTPUT);
  //irrecv.enableIRIn(); // Start the receiver
}

void loop() {
  if(Serial.available()){
    while(Serial.available() > 0){
      c = Serial.read();
      inputCount ++;
      total_msg[inputCount -1] = c;
      //msg.concat(c);
    }
    if(inputCount >= PROTOCOL_COUNT){
      inputCount = 0;
      parseProtocol();
      zero_msg();
    }
    /*
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
     */
  }

}
//*************************************************************
void zero_msg(){
  for(int i = 0; i < PROTOCOL_COUNT; i++){
    total_msg[i] = 0;
  }
}
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
        outputDeviceState(INDEX_FAN, DEVICE_TYPE_FAN); 
        break;
      case DEVICE_TYPE_LED:
        outputDeviceState(INDEX_LED, DEVICE_TYPE_LED);    
        break;
      case DEVICE_TYPE_TEM_SENSOR:

        break;
      }
      return;    
    }
    break;
  case PRO_CMD_TYPE_ACTION_ON_DEVICE:
    {
      switch(deviceType){
      case DEVICE_TYPE_FAN:
        if(para == ACTION_OPEN){
          openFan();
        }
        if(para == ACTION_CLOSE){
          closeFan();
        }
        break;
      case DEVICE_TYPE_LED:
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
    break;
  case PRO_CMD_TYPE_REQUEST_DEVICE_VALUE:
    {
      switch(deviceType){
      case DEVICE_TYPE_TEM_SENSOR:
        int temp =getTempValue();
        char pro_temp[PROTOCOL_COUNT+1] = {
          0                                };
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
      return;
    }
    break;
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
int  GetPinMatchDeviceIndex(int deviceIndex)
{
  switch(deviceIndex){
  case INDEX_FAN:
    return FAN;
    break;
  case INDEX_LED:
    return LED;
    break;
  }
}
void outputAllDeviceState(){
  outputDeviceState(INDEX_FAN, DEVICE_TYPE_FAN); 
  outputDeviceState(INDEX_LED, DEVICE_TYPE_LED);
}
void outputDeviceState(int deviceIndex, int deviceType){

  int statePin = digitalRead(GetPinMatchDeviceIndex(deviceIndex));
  int state = ACTION_CLOSE;
  if(statePin == HIGH) state = ACTION_OPEN;
  char pro_temp[PROTOCOL_COUNT+1] = {
    0        };
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
  //  String pro = constructProtocol(PRO_CMD_TYPE_STATE_RETURN,deviceType,strIndex,state);
  Serial.println(pro_temp);  
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
  return valueTemp + 32;//adjust to could-be-printed ascii
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






















































