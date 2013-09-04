#define GUID "1000000000000001"
#define HEAD "["
#define TAIL "]"
//#define DEBUG 
//*****************************************

String msg="";
char c;

//******************************************

void setup() {
  Serial.begin(9600);
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

  //test
  //  constructProtocol("05","0002","03","04");
  //test
  //  parseProtocol("[10000000000000010500020304]");
  //  delay(3000);
}
//

// 1111111
// 11111[10000000000000010500020304]
// 10000000000000010500020304]
// 0500020304][10000000000000010500020304]
//[10000000000000010300011001]
void parseProtocol(){
  Serial.print("parseProtocol msg => ");
  Serial.println(msg);
  int right = msg.indexOf(']');

  if(right < 0) 
  {
#ifdef DEBUG
    Serial.println("not complete right");
#endif
    return;//not complete
  }
  int left = msg.indexOf('[');
  if(left < 0) 
  {
#ifdef DEBUG
    Serial.println("not complete left");
#endif
    return;//not complete
  }
  if(left > right){
    msg = msg.substring(left, msg.length());//not a pair
    Serial.print("reset pair => ");
    Serial.println(msg);
  }
  else{
    String info = msg.substring(left,right+1);
    Serial.print("info => ");
    Serial.println(info);
    msg = msg.substring(right+1, msg.length());
    Serial.print("new msg => ");
    Serial.println(msg);
    if(info.startsWith("[") && info.endsWith("]")){
      String guid = info.substring(1,17);
      if(guid == GUID){
#ifdef DEBUG
        Serial.println("guid => right");
#endif
      }
      else{
#ifdef DEBUG
        Serial.println(guid);
#endif
        return;
      }
      String protocolType = info.substring(17,19);
#ifdef DEBUG
      if(protocolType == "05"){
        Serial.println("protocolType => right");
      }
      else
      {
        Serial.println(protocolType);
      }
#endif


      String deviceType = info.substring(19,23);
#ifdef DEBUG
      if(deviceType == "0002"){
        Serial.println("deviceType => right");
      }
      else
      {
        Serial.println(deviceType);
      }
#endif

      String deviceIndex = info.substring(23,25);
#ifdef DEBUG
      if(deviceIndex == "03"){
        Serial.println("deviceIndex => right");
      }
      else
      {
        Serial.println(deviceIndex);
      }
#endif
      int length = info.length();
      String para = info.substring(25,length-1);
#ifdef DEBUG
      if(para == "04"){
        Serial.println("para => right");
      }
      else
      {
        Serial.println(para);
      }
#endif
    }
  }
  parseProtocol();
}
void constructProtocol(String protocolType,String deviceType, String deviceIndex,  String para){
  String info = "";
  info = String(info + HEAD);
  info = String(info + GUID);
  info = String(info + protocolType);
  info = String(info + deviceType);
  info = String(info + deviceIndex);
  info = String(info + para);
  info = String(info + TAIL);
#ifdef DEBUG

  if(info == "[10000000000000010500020304]"){
    Serial.println("constructProtocol => right");
  }
  else
  {
    Serial.println(info);
  }

#endif
}

void test(char info[],int len){
  int left=-1;
  int right = -1;
  for(int i=0; i<len; i++){
    if(info[i] == '['){
      left = i;
    }
    if(info[i] == ']'){
      right = i;
    }
    if(left >= 0 && right >= 0){
      break;
    }
  }
  if(left >= 0 && right >= 0){
    Serial.println("data => ");
    Serial.println(right - left);
  }

}

















































