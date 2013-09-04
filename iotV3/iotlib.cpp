#include "iotlib.h"

char total_msg[PROTOCOL_COUNT];
int inputCount = 0;
char guid[] = {
  0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x31};

// ***************************************************************
// Function
int  GetPinMatchDeviceIndex(int deviceType,int deviceIndex)
{
  switch(deviceType)
  {
  case DEVICE_TYPE_LED:
    switch(deviceIndex){
    case INDEX_LED_1:
      return LED_1;
      break;
    }
    break;
  case DEVICE_TYPE_FAN:
    switch(deviceIndex){
    case INDEX_FAN_1:
      return FAN_1;
      break;
    }
    break;
  case DEVICE_TYPE_TEM_SENSOR:
    switch(deviceIndex)
    {
    case INDEX_TEMPERATURE_SENSOR_1:
      return TEMP_INPUT_1;
      break;
    }
    break;
  }

}

void zero_msg(){
  for(int i = 0; i < PROTOCOL_COUNT; i++){
    total_msg[i] = 0;
  }
}




