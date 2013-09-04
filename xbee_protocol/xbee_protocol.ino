#define GUID "1000000000000001"
//#define DEBUG
String msg="";
char c;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly: 
  if(Serial.available()){
    while(Serial.available() > 0){
      c = Serial.read();
#ifdef DEBUG
      Serial.println(c);
#endif
      msg.concat(c);
    }
    if(msg != ""){
      prepareProtocol();
    }
  }
}
//  7E 00 10 10 01 00 13 A2 00 40 9B AE C5 FF FE 00 00 31 32 8B
//  7E 00 0E 90 00 13 A2 00 40 9B AE AC 00 00 01 31 32 21
void prepareProtocol(){
#ifdef DEBUG
  Serial.print("prepareProtocol msg => ");
  Serial.println(msg);
#endif
  if(msg.length() < 3) return;
  int left = msg.indexOf(char(0x7e));
  if(left < 0) 
  {
#ifdef DEBUG
    Serial.println("not complete left");
#endif
    return;//not complete
  }
  if(left > 0){
    msg = msg.substring(left, msg.length());//not a pair
#ifdef DEBUG
    Serial.print("reset pair => ");
    Serial.println(msg);
#endif
  }
  Serial.println(msg);
  int hLength = msg.substring(1, 1).toInt() * 256;
//  Serial.print("H L => ");
  Serial.println(String(hLength));
  int lLength = msg.substring(1, 1).toInt();
//  Serial.print("L L => ");
  Serial.println(String(lLength));
  int totalLength = hLength + lLength;
  //Serial.println(totalLength);
  return;
  int right = msg.indexOf('{', left + 1);

  if(right < 0) 
  {
#ifdef DEBUG
    Serial.println("not complete right");
#endif
    return;//not complete
  }


  if(left < right)
  {
    String info = msg.substring(left,right+1);
    msg = msg.substring(right+1, msg.length());
    parseProtocol(info);
  }

  prepareProtocol();
}
void parseProtocol(String info){
#ifdef DEBUG
  Serial.print("info => ");
  Serial.println(info);
#endif
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










