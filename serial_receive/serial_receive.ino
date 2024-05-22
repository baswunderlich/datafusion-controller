#include <HardwareSerial.h>

HardwareSerial mySerial(1);

void setup(){
  Serial.begin(115200);
  mySerial.begin(9600, SERIAL_8N1, 17, 16);
}

void loop(){
  if(mySerial.available()){
    String message = mySerial.readStringUntil('\n');
    Serial.println("Received: " + message);
  }
}