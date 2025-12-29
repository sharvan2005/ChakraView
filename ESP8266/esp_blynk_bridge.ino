#define BLYNK_PRINT Serial

/* -------- BLYNK CONFIG (DO NOT COMMIT REAL VALUES) -------- */
#define BLYNK_TEMPLATE_ID   "TMPLxxxxxxx"
#define BLYNK_TEMPLATE_NAME "ESP8266"
#define BLYNK_AUTH_TOKEN    "YOUR_BLYNK_AUTH_TOKEN"

/* -------- LIBRARIES -------- */
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

/* -------- WIFI CREDENTIALS -------- */
char ssid[] = "YOUR_WIFI_SSID";
char pass[] = "YOUR_WIFI_PASSWORD";

/* -------- DATA VARIABLES -------- */
int occ, dist, hum, servo;
float temp;

void setup() {
  Serial.begin(9600);
  delay(2000);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // Handshake message (debug)
  Serial.println("ESP_READY");
}

void loop() {
  Blynk.run();

  if (Serial.available()) {
    String line = Serial.readStringUntil('\n');
    line.trim();

    // Expected format:
    // Occ=1 Dist=45 Temp=23.8 Hum=56 Servo=90
    if (sscanf(
          line.c_str(),
          "Occ=%d Dist=%d Temp=%f Hum=%d Servo=%d",
          &occ, &dist, &temp, &hum, &servo
        ) == 5) {

      Blynk.virtualWrite(V0, occ);
      Blynk.virtualWrite(V1, temp);
      Blynk.virtualWrite(V2, hum);
      Blynk.virtualWrite(V3, dist);
      Blynk.virtualWrite(V4, servo);
    }
  }
}
