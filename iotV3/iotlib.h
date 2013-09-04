#include "iot_device_def.h"

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

#define INDEX_ALL                  0x30
#define INDEX_FAN_1                  0x31
#define INDEX_LED_1                  0x32
#define INDEX_TEMPERATURE_SENSOR_1   0x33

#define PROTOCOL_COUNT             18

// "00000001"
extern char guid[];
extern char total_msg[PROTOCOL_COUNT];
extern int inputCount;
/*
char device_type_led[] = {
 0x30,0x30,0x30,0x31};
 char device_tyype_fan[] = {
 0x30,0x30,0x30,0x32};
 char device_type_tem_sensor[] = {
 0x30,0x30,0x30,0x33};
 */
//****************************************
// Function Defination
void doActionOnDevice(long protocolType ,long deviceType, long deviceIndex,long para);
void zero_msg();
int  GetPinMatchDeviceIndex(int deviceType,int deviceIndex);

