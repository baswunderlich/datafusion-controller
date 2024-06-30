/*
  Example from WiFi > WiFiScan
  Complete details at https://RandomNerdTutorials.com/esp32-useful-wi-fi-functions-arduino/
*/

#include "WiFi.h"
#include <HTTPClient.h>
#include "stdlib.h"
#include <HardwareSerial.h>
#include "pthread.h"

HardwareSerial mySerial(1);


#define WIFI_SSID "Puzzlebot-ta"
#define WIFI_PASS "Puzzlebot72"

#define SERVER_ADRESSE "http://192.168.1.1/velocity_set"

String value_left = "0.0";
String value_right = "0.0";

void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(
    WIFI_SSID,
    WIFI_PASS
  );
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
}

void setRobosVeocity(String velo_set_left, String velo_set_right) {
  Serial.println("Set robos velocity");
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    // Specify the URL
    http.begin(SERVER_ADRESSE);
  
    // Specify content-type header
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    // Data to send in HTTP POST request
    String httpRequestData = "";
    String vright = "set_velocity_right=";
    String vleft =  "&set_velocity_left=";
    vright.concat(velo_set_right);
    vleft.concat(velo_set_left);
    httpRequestData.concat(vright);
    httpRequestData.concat(vleft);

    // Send the request
    Serial.println(httpRequestData);
    int httpResponseCode = http.POST(httpRequestData);

    // Check the returning code
    if (httpResponseCode > 0) {
      // Print the response payload
      String response = http.getString();
      Serial.println(httpResponseCode);
      Serial.println(response);
    } else {
      Serial.print("Error on sending POST request: ");
      Serial.println(httpResponseCode);
    }

    // Close the connection
    http.end();
  } else {
    Serial.println("WiFi not connected");
  }

}

void setup() {
  Serial.begin(115200);
  mySerial.begin(9600, SERIAL_8N1, 17, 16);

  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  Serial.println("Setup done");
  
  Serial.println("scan start");

  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0) {
      Serial.println("no networks found");
  } else {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i) {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN)?" ":"*");
      delay(10);
    }
  }
  Serial.println("");

  initWiFi();
}


int tick = 0;
void loop() {
  tick++;
  if(mySerial.available()){
    String message = mySerial.readStringUntil('\n');
    Serial.println("Received: " + message);
    
    // Parse the received message to extract the float values
    int commaIndex = message.indexOf(',');
    if (commaIndex > 0) {
      value_left = message.substring(0, commaIndex);
      value_right = message.substring(commaIndex + 1);

      // Print the parsed float values to the USB serial for debugging
      Serial.print("Parsed value1: ");
      Serial.println(value_left);
      Serial.print("Parsed value2: ");
      Serial.println(value_right);
      setRobosVeocity(value_left, value_right);
    }

    // if(tick >= 10000){
    //   pthread_t thread;
    //   pthread_create(thread, NULL, printThreadId, void);
    //   tick = 0;
    // }
  }
}

