#include <Arduino.h>
#include <HardwareSerial.h>
#include <SD.h>
#include <ArduinoJson.h>  // Install via Library Manager

// Pins
const int PIR_PIN      = 27;/ Passive IR
const int RELAY_PIN    = 26; // LED Trigger 
const int RADAR_RX     = 16;
const int RADAR_TX     = 17;
const int SD_CS_PIN    = 5;   // SD card chip select

HardwareSerial radarSerial(2);

const byte HEADER[4] = {0xF4, 0xF3, 0xF2, 0xF1};
const byte FOOTER[4] = {0xF8, 0xF7, 0xF6, 0xF5};

const char* LOG_FILE = "/traffic_log.jsonl";

File logFile;

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Smart Street Light - Government");

  radarSerial.begin(115200, SERIAL_8N1, RADAR_RX, RADAR_TX);/ / init radar coms

  pinMode(PIR_PIN, INPUT); //init PIR input
  pinMode(RELAY_PIN, OUTPUT);//init relay output 
  digitalWrite(RELAY_PIN, LOW);

  if (!SD.begin(SD_CS_PIN)) {
    Serial.println("SD Card initialization failed! Check wiring/format.");
    // You can add LED blink error here
  } else {
    Serial.println("SD Card ready.");
  }

  Serial.println("Waiting for radar data...");
}

void loop() {
  static bool lightOn = false;
  static unsigned long lastTrigger = 0;
  const unsigned long LIGHT_TIMEOUT = 60000UL;  // 60 s

  if (radarSerial.available() >= 10) {
    if (checkHeader()) {
      processRadarFrame(lastTrigger);
    }
  }

  // Light control
  if (millis() - lastTrigger < LIGHT_TIMEOUT) {
    if (!lightOn) {
      digitalWrite(RELAY_PIN, HIGH);
      lightOn = true;
      Serial.println("Light ON");
    }
  } else {
    if (lightOn) {
      digitalWrite(RELAY_PIN, LOW);
      lightOn = false;
      Serial.println("Light OFF");
    }
  }

  delay(50);
}

bool checkHeader() {
  for (int i = 0; i < 4; i++) {
    if (radarSerial.read() != HEADER[i]) return false;
  }
  return true;
}

void processRadarFrame(unsigned long& lastTrigger) {
  byte lenLow  = radarSerial.read();
  byte lenHigh = radarSerial.read();
  uint16_t dataLen = (lenHigh << 8) | lenLow;

  if (dataLen < 2 || dataLen > 100) return;

  byte buffer[128];
  size_t readBytes = radarSerial.readBytes(buffer, dataLen + 4);

  if (readBytes != dataLen + 4) return;

  // Verify footer
  if (buffer[dataLen] != FOOTER[0] || buffer[dataLen+1] != FOOTER[1] ||
      buffer[dataLen+2] != FOOTER[2] || buffer[dataLen+3] != FOOTER[3]) {
    return;
  }

  int offset = 0;
  // byte numTargets = buffer[offset++];  // Some firmwares have count byte first; test yours
  // For now assume direct targets (common in examples)

  bool newDetection = false;
  int detectedDist = 0;
  int detectedSpeed = 0;
  int detectedAngle = 0;
  int detectedSNR   = 0;

  for (int t = 0; t < 5 && offset < dataLen - 4; t++) {
    byte alarm    = buffer[offset++];
    byte angleRaw = buffer[offset++];
    byte distRaw  = buffer[offset++];
    byte dirRaw   = buffer[offset++];
    byte speedRaw = buffer[offset++];
    byte snr      = buffer[offset++];

    int angle   = (int)angleRaw - 0x80;
    int distance = (int)distRaw;
    int speedKmh = (int)speedRaw;
    bool approaching = (dirRaw == 0x01);

    if (alarm && approaching && distance > 0 && distance <= 35 && speedKmh > 5) {
      newDetection = true;
      detectedDist  = distance;
      detectedSpeed = speedKmh;
      detectedAngle = angle;
      detectedSNR   = snr;

      Serial.printf("APPROACHING! Dist:%d m, Speed:%d km/h, Angle:%d, SNR:%d\n",
                    distance, speedKmh, angle, snr);

      // Trigger light
      lastTrigger = millis();

      // Log this detection
      logDetection(distance, speedKmh, angle, snr);
    }
  }
}

void logDetection(int dist, int speed, int angle, int snr) {
  if (!SD.exists(LOG_FILE)) {
    logFile = SD.open(LOG_FILE, FILE_WRITE);
    if (logFile) logFile.close();  // Create empty file
  }

  logFile = SD.open(LOG_FILE, FILE_APPEND);
  if (!logFile) {
    Serial.println("Failed to open log file!");
    return;
  }

  StaticJsonDocument<256> doc;
  doc["time"]     = millis();           // Upgrade to RTC: doc["time"] = getRTCTime();
  doc["distance_m"] = dist;
  doc["speed_kmh"]  = speed;
  doc["angle_deg"]  = angle;
  doc["snr"]        = snr;
  // Add more: doc["pir"] = digitalRead(PIR_PIN);

  if (serializeJson(doc, logFile) == 0) {
    Serial.println("Failed to write JSON");
  } else {
    logFile.println();  // New line for JSONL
    Serial.println("Logged detection to SD");
  }

  logFile.close();
}
