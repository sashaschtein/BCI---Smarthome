# Lit Synaptix

## Project Overview
The goal of this project was to use alpha waves to turn an LED light on and off. This project acts as a proof of concept for creating an ecosystem of household appliances controlled by the user’s brainwaves. This can be achieved through existing APIs and technologies, making this concept viable for consumer use. In our prototype, the LED light reacts to the alpha relative values, which is dependent on whether the user is in a meditative state. 

We decided to use the Muse headset for its readily accessible SDK, which woud allow us to focus more on data processing rather than data acquisition for our first time working with EEGs, and the Google Firebase API to communicate between the LED and the Muse because it is relatively easy to set up and use.

## Project Setup
#### Setting up the Muse EEG Headset
For this project, we used the 2016 Muse Headset and the sample TestLibMuseAndroid app, which you can find 
[here](http://developer.choosemuse.com/sdk/android/getting-started-with-libmuse-android). Follow the guide to download Android Studio and run the app on your Android device. Make sure the Muse headset is on (the light should be on but not solid) and press refresh in the app. Once the device shows up, select it and press connect. Note that you should not connect the Muse headset directly to your phone via bluetooth connection; the app does that for you.

#### Setting Up Firebase
Go to the [Firebase website](firebase.google.com) and set up your own Firebase Realtime Database. Near the top, you will see a link with the format https://example.firebaseio.com/. This is the hostname of the realtime database, and you will need it later to connect your Wifi module to Firebase. Under data, add a child with value false. We decided to call the child corresponding to our device `LED1` and nested it under a child called `devices`. 

#### Integrating Firebase to the Muse App
The first thing that you will need to do is connect your app to Firebase. In Android Studio, go to Tools > Firebase > Realtime Database and follow the instructions there to connect your app to Firebase. Once the app has been connected, add the following lines to the top of `MainActivity`.

```
DatabaseReference databaseRef = FirebaseDatabase.getInstance().getReference();
DatabaseReference LED1 = databaseRef.child(“devices”).child(“LED1”);
```
Note: the value of LED1 will differ based on how you nested your children.

Above the `receiveMuseDataPacket` function, place this function:

```
private void determineDeviceState(double [] aBuffer, double aThreshold)
{
   double totalAlpha = 0;
   for (int i = 0; i < aBuffer.length; i++)
       totalAlpha += aBuffer[i];
   double avgAlpha = totalAlpha/aBuffer.length;
   if (avgAlpha > aThreshold) {
       LED1.setValue(true);
   }
   else if (avgAlpha <= aThreshold) {
       LED1.setValue(false);
   }
}
```
After that, call the function as shown:

```
case ALPHA_RELATIVE:
   assert(alphaBuffer.length >= n);
   getEegChannelValues(alphaBuffer,p);
   determineDeviceState(alphaBuffer, threshold);
   alphaStale = true;
   break;
```

Note: `threshold` is a double between 0 and 1. We chose 0.2 as our threshold, but you can adjust this value depending on how responsive you want your devices to be.

#### Setting Up Your Circuit
For the circuit we chose to use an [ESP8266 NodeMCU microcontroller](https://www.amazon.com/gp/product/B010N1SPRK/ref=ppx_yo_dt_b_asin_title_o00_s00?ie=UTF8&psc=1) because it had the Wifi module built in, which eliminated for the need for a logic level converter between an Arduino and the ESP8266 Wifi module. The NodeMCU can be programmed using the [Arduino IDE](https://www.arduino.cc/en/main/software) if the [driver](https://www.silabs.com/products/development-tools/software/usb-to-uart-bridge-vcp-drivers) is installed. You will also need to install the 
[FirebaseArduino library](https://github.com/FirebaseExtended/firebase-arduino/tree/master/examples/FirebaseDemo_ESP8266) and the [ArduinoJson library](https://www.arduinolibraries.info/libraries/arduino-json). For ArduinoJson, we could not get version 6 to work with FirebaseArduino, so we used ArduinoJson 5.13.5. You will also need to install the Arduino core for the ESP8266 chip, so follow this [tutorial](https://github.com/esp8266/Arduino#installing-with-boards-manager). 

This is the Arduino code:

```
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
```
Replace `HOSTNAME` with the hostname that you found earlier, but remove the `https://` at the front and the `/` at the end. For example, if your hostname was `https://example.firebaseio.com/`, you should put `example.firebaseio.com`. For `SECRET KEY`, you will have to go your `Firebase > Project Settings > Service Accounts > Database Secrets` and copy the secret key found there. For `SSID` and `PASSWORD`, put the name of your Wifi network and the password. You should modify `path` so that it matches how you organized your database, starting with a "/" and listing every child. 

For the breadboard, all you will need is the NodeMCU, an LED attached to pin D6, and a resistor, which is connected to the LED on one end and grounded on the other. 

#### Testing the System
Everything should now be properly set up. Connect the Muse headset to the app, make sure the Arduino is connected to Firebase (check through the serial monitor) and try to relax. Once the threshold is passed, the LED should light up, and it should turn off when you start concentrating again. Now you have an understanding of how to use IoT technology and EEG signals to control devices!

## Conclusion
Our first experience with working with neurotechnology provided insight into how existing EEG technologies could be integrated as consumer-minded devices. Along with learning how EEGs work, we gained valuable skills in developing Android applications, working with backend APIs, and using IoT devices. Expanding upon this current iteration of the project, we look forward to further develop our project to support consumer products such as Phillips Hue lightbulbs and Chromecast TVs. As we provided a proof of concept of working with IoT devices, we hope to analyze other types of brainwaves and see how different combinations of brainwaves indicate different states of mind. A more sophisticated level of control would enable users to control multiple devices, leading to a comprehensive realization of a brain-controlled modern smart home.
