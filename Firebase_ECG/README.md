# Firebase Integration with Arduino ESP8266

#### Preparations
1. Install the necessary libraries for [ESP8266](https://arduino-esp8266.readthedocs.io/en/latest/installing.html)
2. Follow instructions on integrating [Firebase library for Arduino](https://github.com/FirebaseExtended/firebase-arduino) with ESP8266
3. Create Firebase project and set up [Realtime Database](https://firebase.google.com/docs/database/)

#### Implementation
1. Connect the ESP8266 Wifi Module and LED to Arduino.
2. To connect the module to Wifi, provide the SSID and password of the wifi (mobile hotspot works best)
3. Connect to Firebase through properties listed in code
4. LED setup should be synchronized with the Firebase database properties!
