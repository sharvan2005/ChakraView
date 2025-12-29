# Smart Occupancy System (IoT)

An IoT-based smart occupancy detection system using Arduino Uno and ESP8266.

## Features
- Ultrasonic-based occupancy detection
- Hysteresis to prevent false toggling 
- Automatic lighting control
- Servo-based ventilation control
- Real-time monitoring via Blynk

## Hardware Used
- Arduino Uno
- ESP8266
- HC-SR04 Ultrasonic Sensor
- DHT11
- Servo Motor
- LED

## Folder Structure
- Arduino/ → Sensor, control & actuation logic
- ESP8266/ → WiFi + Blynk dashboard bridge

## Notes
- Replace WiFi and Blynk credentials before uploading
- Serial communication is used between Arduino and ESP8266
