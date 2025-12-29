#include <Servo.h>
#include <DHT.h>
#include <SoftwareSerial.h>

/* ---------- PIN DEFINITIONS ---------- */
#define TRIG_PIN   9
#define ECHO_PIN   10
#define DHT_PIN    8
#define DHT_TYPE   DHT11
#define SERVO_PIN  5
#define LED_PIN    6

/* ---------- OCCUPANCY HYSTERESIS ---------- */
#define OCCUPIED_ON   55   // cm
#define OCCUPIED_OFF  65   // cm

/* ---------- SERIAL TO ESP ---------- */
// Arduino D2 = RX, D3 = TX
SoftwareSerial espSerial(2, 3);

/* ---------- OBJECTS ---------- */
Servo ventServo;
DHT dht(DHT_PIN, DHT_TYPE);

/* ---------- VARIABLES ---------- */
long duration;
int distanceCM;

float temperature;
float humidity;

int occState = 0;
int occupancy = 0;

int servoAngle = 0;
int lastServoAngle = -1;

/* ---------- SETUP ---------- */
void setup() {
  Serial.begin(9600);
  espSerial.begin(9600);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);

  dht.begin();

  Serial.println("UNO READY");
}

/* ---------- ULTRASONIC FUNCTION ---------- */
int getDistanceCM() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH, 30000);
  if (duration == 0) return 300;

  return duration * 0.034 / 2;
}

/* ---------- LOOP ---------- */
void loop() {

  /* ---- SENSOR READ ---- */
  distanceCM = getDistanceCM();

  temperature = dht.readTemperature();
  humidity = dht.readHumidity();

  if (isnan(temperature)) temperature = 0;
  if (isnan(humidity)) humidity = 0;

  /* ---- OCCUPANCY WITH HYSTERESIS ---- */
  if (!occState && distanceCM < OCCUPIED_ON) {
    occState = 1;
  } 
  else if (occState && distanceCM > OCCUPIED_OFF) {
    occState = 0;
  }

  occupancy = occState;

  /* ---- LED CONTROL ---- */
  digitalWrite(LED_PIN, occupancy ? HIGH : LOW);

  /* ---- SERVO CONTROL (ATTACH → MOVE → DETACH) ---- */
  servoAngle = occupancy ? 90 : 0;

  if (servoAngle != lastServoAngle) {
    ventServo.attach(SERVO_PIN);
    ventServo.write(servoAngle);
    delay(300);
    ventServo.detach();
    lastServoAngle = servoAngle;
  }

  /* ---- SEND DATA TO ESP ---- */
  espSerial.print("Occ=");
  espSerial.print(occupancy);
  espSerial.print(" Dist=");
  espSerial.print(distanceCM);
  espSerial.print(" Temp=");
  espSerial.print(temperature, 1);
  espSerial.print(" Hum=");
  espSerial.print((int)humidity);
  espSerial.print(" Servo=");
  espSerial.println(servoAngle);

  /* ---- DEBUG ---- */
  Serial.println("Data sent to ESP");

  delay(1000);
}

