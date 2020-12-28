#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

#define FIREBASE_HOST "HOSTNAME"
#define FIREBASE_AUTH "SECRET KEY"
#define WIFI_SSID "SSID"
#define WIFI_PASSWORD "PASSWORD"

#define path "/devices/LED1"
  
const int LED = 12; // Pin D6

void setup() {
  Serial.begin(9600);
  pinMode(LED, OUTPUT);

  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

void loop() {
  bool lightStatus = Firebase.getBool(path);
  if (lightStatus)
  {
    digitalWrite(LED, HIGH);
    Serial.println("LED On");
  }
  else
  {
    digitalWrite(LED, LOW); 
    Serial.println("LED Off");
  }
}
