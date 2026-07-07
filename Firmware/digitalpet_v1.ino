#include <SPI.h>
#include <Wire.h>
#include <TFT_eSPI.h>
#include <Adafruit_LSM6DS33.h>

#include "config.h"
#include "eyes.h"
#include "mood.h"
#include "haptics.h"

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite sprite = TFT_eSprite(&tft);
EyeRenderer eyes(sprite);

Adafruit_LSM6DS33 accelerometer;
bool accelReady = false;
float lastAccelMagnitude = 9.8f; // assume resting under gravity at boot

MoodEngine mood;
HapticController haptics;

void setup() {
  Serial.begin(115200);
  delay(300);
  Serial.println(F("Tao minimal demo starting..."));

  tft.init();
  tft.setRotation(0);
  sprite.createSprite(SCREEN_W, SCREEN_H);
  eyes.setImmediate(MoodEngine::targetForState(mood.getState()));
  eyes.draw();
  Serial.println(F("Display initialized."));

  Wire.begin();

  accelReady = accelerometer.begin_I2C();
  if (accelReady) {
    accelerometer.setAccelRange(LSM6DS_ACCEL_RANGE_4_G);
    accelerometer.setAccelDataRate(LSM6DS_RATE_104_HZ);
    Serial.println(F("Accelerometer OK."));
  } else {
    Serial.println(F("Accelerometer NOT FOUND - check wiring and I2C address."));
  }

  if (haptics.begin()) {
    Serial.println(F("Haptic driver OK."));
  } else {
    Serial.println(F("Haptic driver NOT FOUND - check wiring and I2C address."));
  }
}

void loop() {
  unsigned long now = millis();
  float accelDelta = readAccelDelta();

  bool stateChanged = mood.update(accelDelta, now);
  if (stateChanged) {
    MoodState newState = mood.getState();
    eyes.setTarget(MoodEngine::targetForState(newState));
    haptics.playForTransition(newState);

    Serial.print(F("-> "));
    Serial.println(MoodEngine::nameForState(newState));
  }

  eyes.update();
  eyes.draw();

  delay(FRAME_DELAY_MS);
}

// Reads the accelerometer and returns how much the acceleration magnitude
// changed since the last reading. Returns 0 if the accelerometer isn't
// available, which keeps the demo running (in MOOD_STILL) even without it.
float readAccelDelta() {
  if (!accelReady) return 0.0f;

  sensors_event_t accelEvent, gyroEvent, tempEvent;
  accelerometer.getEvent(&accelEvent, &gyroEvent, &tempEvent);

  float ax = accelEvent.acceleration.x;
  float ay = accelEvent.acceleration.y;
  float az = accelEvent.acceleration.z;
  float magnitude = sqrt(ax * ax + ay * ay + az * az);

  float delta = fabs(magnitude - lastAccelMagnitude);
  lastAccelMagnitude = magnitude;
  return delta;
}
