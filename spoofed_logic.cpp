#include <Arduino.h>
#include <SD.h>
#include <ArduinoJson.h>  // Install via Library Manager

// Pins (same as before, but radar UART not used)
const int PIR_PIN      = 27;   // Can still connect real PIR if you want hybrid test
const int RELAY_PIN    = 26;
const int SD_CS_PIN    = 5;    // SD card chip select

const char* LOG_FILE = "/traffic_log.jsonl";

// Spoof control parameters (tune these!)
const unsigned long SPOOF_INTERVAL_MIN = 2000;   // ms
const unsigned long SPOOF_INTERVAL_MAX = 10000;  // ms
const int FAKE_VEHICLE_PROBABILITY = 60;         // % chance of detection per interval
const int MIN_DISTANCE = 10;
const int MAX_DISTANCE = 35;
const int MIN_SPEED    = 20;
const int MAX_SPEED    = 80;

// Globals
static unsigned long lastSpoofTime = 0;
static unsigned long lastTrigger = 0;
static bool lightOn = false;
const unsigned long LIGHT_TIMEOUT = 60000UL;  // 60 seconds

File logFile;

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Smart Street Light - SPOOF MODE (No real sensors)");

  pinMode(PIR_PIN, INPUT);       // Optional: connect real PIR for mixed testing
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);

  randomSeed(analogRead(0));     // For varied spoofing

  if (!SD.begin(SD_CS_PIN)) {
    Serial.println("SD Card initialization failed! Check wiring/format.");
  } else {
    Serial.println("SD Card ready. Logging to " + String(LOG_FILE));
  }

  Serial.println("Spoofing fake radar + PIR inputs every few seconds...");
}

void loop() {
  unsigned long now = millis();

  // Simulate radar "frame" periodically
  if (now - lastSpoofTime >= random(SPOOF_INTERVAL_MIN, SPOOF_INTERVAL_MAX + 1)) {
    lastSpoofTime = now;

    // Decide if we "see" vehicles this cycle
    bool hasDetection = (random(100) < FAKE_VEHICLE_PROBABILITY);

    int detectedDist  = 0;
    int detectedSpeed = 0;
    int detectedAngle = 0;
    int detectedSNR   = 0;
    bool newDetection = false;

    if (hasDetection) {
      // Spoof 1 fake approaching target (could add loop for 2–3)
      detectedDist  = random(MIN_DISTANCE, MAX_DISTANCE + 1);
      detectedSpeed = random(MIN_SPEED, MAX_SPEED + 1);
      detectedAngle = random(-15, 16);
      detectedSNR   = random(80, 201);

      Serial.printf("SPOOF DETECTION! Dist:%d m, Speed:%d km/h, Angle:%d, SNR:%d\n",
                    detectedDist, detectedSpeed, detectedAngle, detectedSNR);

      newDetection = true;
      lastTrigger = now;

      // Log it
      logDetection(detectedDist, detectedSpeed, detectedAngle, detectedSNR);
    } else {
      Serial.println("SPOOF: No target this cycle");
    }

    // Spoof PIR (correlated to radar for realism)
    bool fakePir = hasDetection && (random(100) < 70);  // 70% chance if detection
    // If you have real PIR connected, you could AND/OR it: fakePir = fakePir && digitalRead(PIR_PIN);
  }

  // Light control (same as real version)
  if (now - lastTrigger < LIGHT_TIMEOUT) {
    if (!lightOn) {
      digitalWrite(RELAY_PIN, HIGH);
      lightOn = true;
      Serial.println("Light ON (triggered)");
    }
  } else {
    if (lightOn) {
      digitalWrite(RELAY_PIN, LOW);
      lightOn = false;
      Serial.println("Light OFF (timeout)");
    }
  }

  delay(50);  // Small loop delay
}

void logDetection(int dist, int speed, int angle, int snr) {
  if (!SD.exists(LOG_FILE)) {
    logFile = SD.open(LOG_FILE, FILE_WRITE);
    if (logFile) logFile.close();  // Create file
  }

  logFile = SD.open(LOG_FILE, FILE_APPEND);
  if (!logFile) {
    Serial.println("Failed to open log file!");
    return;
  }

  StaticJsonDocument<256> doc;
  doc["time"]       = millis();  // ms since boot; add RTC later for real time
  doc["distance_m"] = dist;
  doc["speed_kmh"]  = speed;
  doc["angle_deg"]  = angle;
  doc["snr"]        = snr;
  // doc["pir"]     = ... if you want to log spoofed PIR too

  if (serializeJson(doc, logFile) == 0) {
    Serial.println("Failed to write JSON");
  } else {
    logFile.println();  // JSON Lines format
    Serial.println("Logged spoofed detection to SD");
  }

  logFile.close();
}
