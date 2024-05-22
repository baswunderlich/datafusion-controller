#include <HardwareSerial.h>

HardwareSerial mySerial(1);

void setup(){
  Serial.begin(115200);
  mySerial.begin(9600, SERIAL_8N1, 17, 16);
}

void loop(){
  String message = "Hello from the other side";

  mySerial.println(message);
  Serial.println("Sent: " + message);

  delay(1000);
}