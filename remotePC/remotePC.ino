#include <IRremote.h>

//#define DEBUG

#define TV_BUTTON_SHUTDOWN    0x8010260C
#define TV_VOICE_BUTTON_UP    0x8010A610
#define TV_VOICE_BUTTON_UP2   0x80102610
#define TV_VOICE_BUTTON_DOWN  0x80102611
#define TV_VOICE_BUTTON_DOWN2 0x8010A611
#define TV_BUTTON_MUTE        0x8010260D
#define TV_BUTTON_MUTE2       0x8010A60D
#define TV_BUTTON_INFO        0x8010260F
#define TV_BUTTON_INFO2       0x8010A60F
#define TV_BUTTON_OK          0x8010265C
#define TV_BUTTON_OK2         0x8010A65C


#define BUTTON_POWER          0xFFA25D
#define BUTTON_MUTE           0xFFE21D
#define BUTTON_ADD            0xFF906F
#define BUTTON_PLUS           0xFFA857
#define BUTTON_MODE           0xFF629D
#define BUTTON_PLAY           0xFF22DD

int    totalDelay = 0;
int    RECV_PIN   = 7;
bool   timeOut    = false;

IRrecv irrecv(RECV_PIN);

decode_results results;

void setup() {
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
}

void loop() {
  if (irrecv.decode(&results)) {
#ifdef DEBUG
    Serial.println(results.value, HEX);
#endif
    acceptIR(results.value);
    irrecv.resume(); // Receive the next value
  } 
delay(500);
}
void acceptIR(unsigned long code){
#ifdef DEBUG
  Serial.println("acceptIR => ");
#endif
  if(code == TV_BUTTON_OK || code == TV_BUTTON_OK2 || BUTTON_PLAY == code){
#ifdef DEBUG
    Serial.println("button => TV_BUTTON_OK");
#endif
    Serial.println("[confirm]");
  }
  if(code == TV_BUTTON_INFO || code == TV_BUTTON_INFO2 || BUTTON_MODE == code){
#ifdef DEBUG
    Serial.println("button => TV_BUTTON_INFO");
#endif
    Serial.println("[remotePC]");
  }
  if(code == TV_BUTTON_SHUTDOWN || code == BUTTON_POWER){
#ifdef DEBUG
    Serial.println("button => TV_BUTTON_SHUTDOWN");
#endif
    Serial.println("[shutdown]");
  }
  if(code == TV_VOICE_BUTTON_UP || code == TV_VOICE_BUTTON_UP2 || BUTTON_ADD == code){
#ifdef DEBUG
    Serial.println("button => TV_VOICE_BUTTON_UP");
#endif
    Serial.println("[up]");
  }
  if(code == TV_VOICE_BUTTON_DOWN || code == TV_VOICE_BUTTON_DOWN2 || BUTTON_PLUS == code){
#ifdef DEBUG
    Serial.println("button => TV_VOICE_BUTTON_DOWN");
#endif
    Serial.println("[down]");
  }
  if(code == TV_BUTTON_MUTE || code == TV_BUTTON_MUTE2 || BUTTON_MUTE == code){
#ifdef DEBUG
    Serial.println("button => TV_BUTTON_MUTE");
#endif
    Serial.println("[mute]");
  }
}






