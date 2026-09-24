// digipet_eyes_simple.ino
// Simple friendly black/white circle eyes with pupils, on Adafruit_GC9A01A
// (the stack that actually works - no LVGL/TFT_eSPI).
//
// Shake animation: while shaking, eyes scroll straight down and wrap back in
// from the top (like a slot-machine reel) - reads as spinning, no rotation
// math needed. When the shake stops, eyes glide smoothly back to their
// resting spot instead of snapping.
//
// Arduino Library Manager:
//   Adafruit GFX Library
//   Adafruit GC9A01A  (pulls in Adafruit BusIO)
//
// Board: XIAO_ESP32S3   Serial: 115200
//
// J2 (square pad = pin 1):
//   1 RST   2 CS   3 DC   4 SDA/MOSI   5 SCL/SCK   6 GND   7 3V3

enum FaceMood { FACE_LOOK, FACE_DIZZY, FACE_SURPRISE, FACE_FREEFALL, FACE_SLEEPY, FACE_UPSIDE };

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_GC9A01A.h>
#include <math.h>

static const int PIN_IMU_CS = D1;
static const int PIN_TFT_DC = D2;
static const int PIN_TFT_CS = D3;
static const int PIN_TFT_RST = D0;
static const int PIN_TFT_SCK = D8;
static const int PIN_TFT_MOSI = D10;

static const uint8_t ADDR_LSM6_LOW = 0x6A;
static const uint8_t ADDR_LSM6_HIGH = 0x6B;
static const uint8_t LSM6_WHO_AM_I = 0x0F;
static const uint8_t LSM6_CTRL1 = 0x10;
static const uint8_t LSM6_CTRL2 = 0x11;
static const uint8_t LSM6_CTRL3 = 0x12;
static const uint8_t LSM6_CTRL8 = 0x17;
static const uint8_t LSM6_OUTX_L_G = 0x22;
static const uint8_t LSM6_CTRL3_BDU_INC = 0x44;
static const uint8_t LSM6_ODR_60HZ = 0x05;
static const uint8_t LSM6_CTRL8_4G = 0x01;
static const float ACCEL_G_PER_LSB = 0.000122f;
static const float GYRO_DPS_PER_LSB = 0.00875f;

static const float SX_FROM_AX = -1.0f;
static const float SY_FROM_AY = -1.0f;

Adafruit_GC9A01A tft(PIN_TFT_CS, PIN_TFT_DC, PIN_TFT_RST);
GFXcanvas16 canvas(240, 240);

// ---- Eye shape ----
static const int EYE_R = 38;     // main white circle radius
static const int PUPIL_R = 15;   // black pupil radius
static const int EYE_DX = 44;    // horizontal distance from center
static const int EYE_CY = 118;   // resting vertical position (screen center-ish)

// ---- Dizzy scroll-wrap animation ----
static const float DIZZY_SCROLL_SPEED = 260.0f; // px/sec, downward
static const uint32_t SETTLE_DURATION_MS = 450;  // how long the smooth glide-back takes

// ---- Sensitivity tuning ----
static const float GYRO_DIZZY_THRESHOLD = 420.0f;
static const uint32_t GYRO_SUSTAIN_MS = 180;
static const float ACCEL_SURPRISE_DELTA = 2.0f;
static const float ACCEL_FREEFALL_G = 0.35f;

uint8_t imuAddr = 0;
bool imuReady = false;

bool i2cWrite8(uint8_t addr, uint8_t reg, uint8_t value) {
  Wire.beginTransmission(addr);
  Wire.write(reg);
  Wire.write(value);
  return Wire.endTransmission() == 0;
}

bool i2cReadBytes(uint8_t addr, uint8_t reg, uint8_t *buf, uint8_t len) {
  Wire.beginTransmission(addr);
  Wire.write(reg);
  if (Wire.endTransmission(false) != 0) return false;
  if (Wire.requestFrom(addr, len) != len) return false;
  for (uint8_t i = 0; i < len; i++) buf[i] = Wire.read();
  return true;
}

bool probeImu(uint8_t addr, uint8_t *whoami) {
  return i2cReadBytes(addr, LSM6_WHO_AM_I, whoami, 1);
}

bool initImu() {
  pinMode(PIN_IMU_CS, OUTPUT);
  digitalWrite(PIN_IMU_CS, HIGH);
  delay(10);

  Wire.begin(D4, D5);
  Wire.setClock(400000);
  delay(20);

  uint8_t who = 0;
  uint8_t addrs[] = {ADDR_LSM6_LOW, ADDR_LSM6_HIGH};
  for (uint8_t i = 0; i < 2; i++) {
    if (!probeImu(addrs[i], &who)) continue;
    imuAddr = addrs[i];
    Serial.print(F("IMU WHO_AM_I @ 0x"));
    Serial.print(imuAddr, HEX);
    Serial.print(F(" = 0x"));
    Serial.println(who, HEX);
    i2cWrite8(imuAddr, LSM6_CTRL3, LSM6_CTRL3_BDU_INC);
    i2cWrite8(imuAddr, LSM6_CTRL8, LSM6_CTRL8_4G);
    i2cWrite8(imuAddr, LSM6_CTRL1, LSM6_ODR_60HZ);
    i2cWrite8(imuAddr, LSM6_CTRL2, LSM6_ODR_60HZ);
    delay(20);
    return true;
  }
  Serial.println(F("IMU not found on D4/D5. Trying D9/D8..."));
  Wire.end();
  Wire.begin(D9, D8);
  Wire.setClock(400000);
  delay(20);
  for (uint8_t i = 0; i < 2; i++) {
    if (!probeImu(addrs[i], &who)) continue;
    imuAddr = addrs[i];
    i2cWrite8(imuAddr, LSM6_CTRL3, LSM6_CTRL3_BDU_INC);
    i2cWrite8(imuAddr, LSM6_CTRL8, LSM6_CTRL8_4G);
    i2cWrite8(imuAddr, LSM6_CTRL1, LSM6_ODR_60HZ);
    i2cWrite8(imuAddr, LSM6_CTRL2, LSM6_ODR_60HZ);
    Serial.println(F("IMU found on D9/D8"));
    return true;
  }
  return false;
}

bool readImu(float *ax, float *ay, float *az, float *gx, float *gy, float *gz) {
  uint8_t raw[12];
  if (!i2cReadBytes(imuAddr, LSM6_OUTX_L_G, raw, 12)) return false;
  auto s16 = [](uint8_t lo, uint8_t hi) -> int16_t {
    return (int16_t)((uint16_t)hi << 8 | lo);
  };
  *gx = s16(raw[0], raw[1]) * GYRO_DPS_PER_LSB;
  *gy = s16(raw[2], raw[3]) * GYRO_DPS_PER_LSB;
  *gz = s16(raw[4], raw[5]) * GYRO_DPS_PER_LSB;
  *ax = s16(raw[6], raw[7]) * ACCEL_G_PER_LSB;
  *ay = s16(raw[8], raw[9]) * ACCEL_G_PER_LSB;
  *az = s16(raw[10], raw[11]) * ACCEL_G_PER_LSB;
  return true;
}

// ---- Drawing helpers ----

float smoothlerp(float a, float b, float t) {
  return a + (b - a) * t;
}

float smoothstep01(float t) {
  if (t < 0) t = 0;
  if (t > 1) t = 1;
  return t * t * (3.0f - 2.0f * t);
}

void fillEllipse(int cx, int cy, int rx, int ry, uint16_t color) {
  if (rx < 1) rx = 1;
  if (ry < 1) ry = 1;
  for (int y = -ry; y <= ry; y++) {
    float ny = (float)y / (float)ry;
    int xspan = (int)(rx * sqrtf(1.0f - ny * ny) + 0.5f);
    canvas.drawFastHLine(cx - xspan, cy + y, xspan * 2 + 1, color);
  }
}

// Draws one eye: white circle (squashed vertically for blinking), black pupil
// offset inside it for gaze.
void drawEye(int cx, int cy, float openScale, float pupilOffX, float pupilOffY, float pupilScale) {
  int ry = (int)(EYE_R * openScale);
  if (ry < 3) ry = 3;
  fillEllipse(cx, cy, EYE_R, ry, GC9A01A_WHITE);

  if (openScale < 0.2f) return; // eye closed - no pupil to show

  int maxOffX = EYE_R - (int)(PUPIL_R * pupilScale) - 4;
  int maxOffY = ry - (int)(PUPIL_R * pupilScale) - 4;
  if (maxOffX < 2) maxOffX = 2;
  if (maxOffY < 2) maxOffY = 2;
  int px = cx + (int)(constrain(pupilOffX, -1.0f, 1.0f) * maxOffX);
  int py = cy + (int)(constrain(pupilOffY, -1.0f, 1.0f) * maxOffY);
  int pr = (int)(PUPIL_R * pupilScale);
  if (pr < 3) pr = 3;
  canvas.fillCircle(px, py, pr, GC9A01A_BLACK);
}

const char *moodName(FaceMood m) {
  switch (m) {
    case FACE_DIZZY: return "DIZZY";
    case FACE_SURPRISE: return "SURPRISE";
    case FACE_FREEFALL: return "FREEFALL";
    case FACE_SLEEPY: return "SLEEPY";
    case FACE_UPSIDE: return "UPSIDE";
    default: return "LOOK";
  }
}

FaceMood mood = FACE_LOOK, lastMood = FACE_LOOK;

uint32_t dizzyUntil = 0, dizzySustainStart = 0, surpriseUntil = 0, freefallUntil = 0;
uint32_t stillSince = 0, nextBlinkAt = 0, blinkUntil = 0;
float openAmt = 1.0f;
float pupilOffX = 0, pupilOffY = 0, pupilScale = 1.0f;

float dizzyScrollY = 0;
bool settling = false;
uint32_t settleStart = 0;
float settleFromY = EYE_CY;
float currentEyeY = EYE_CY;

uint32_t lastMs = 0;

void setup() {
  Serial.begin(115200);
  delay(200);
  Serial.println(F("Simple eyes"));

  SPI.begin(PIN_TFT_SCK, -1, PIN_TFT_MOSI, PIN_TFT_CS);
  tft.begin(40000000);
  tft.setRotation(0);

  canvas.fillScreen(GC9A01A_BLACK);
  drawEye(120 - EYE_DX, EYE_CY, 1.0f, 0, 0, 1.0f);
  drawEye(120 + EYE_DX, EYE_CY, 1.0f, 0, 0, 1.0f);
  tft.drawRGBBitmap(0, 0, canvas.getBuffer(), 240, 240);
  Serial.println(F("Display up"));

  imuReady = initImu();
  if (imuReady) Serial.println(F("IMU OK"));
  else Serial.println(F("IMU missing — idle blink only"));

  lastMs = millis();
  stillSince = lastMs;
  nextBlinkAt = lastMs + 1800;
}

void loop() {
  float ax = 0, ay = 0, az = 1, gx = 0, gy = 0, gz = 0;
  if (imuReady && !readImu(&ax, &ay, &az, &gx, &gy, &gz)) {
    ax = 0; ay = 0; az = 1;
  }

  uint32_t now = millis();
  float dt = (now - lastMs) / 1000.0f;
  lastMs = now;
  if (dt <= 0 || dt > 0.2f) dt = 0.02f;

  float sx = SX_FROM_AX * ax;
  float sy = SY_FROM_AY * ay;
  float gyroMag = sqrtf(gx * gx + gy * gy + gz * gz);
  float aMag = sqrtf(ax * ax + ay * ay + az * az);

  if (gyroMag > GYRO_DIZZY_THRESHOLD) {
    if (dizzySustainStart == 0) dizzySustainStart = now;
    if (now - dizzySustainStart > GYRO_SUSTAIN_MS) dizzyUntil = now + 1200;
  } else {
    dizzySustainStart = 0;
  }

  if (fabsf(aMag - 1.0f) > ACCEL_SURPRISE_DELTA && gyroMag < 160.0f) surpriseUntil = now + 450;
  if (aMag < ACCEL_FREEFALL_G) freefallUntil = now + 500;

  bool still = (gyroMag < 18.0f && fabsf(sx) < 0.18f && fabsf(sy) < 0.18f);
  if (still) { if (stillSince == 0) stillSince = now; } else { stillSince = now; }

  FaceMood prevMood = mood;
  if (now < freefallUntil) mood = FACE_FREEFALL;
  else if (now < dizzyUntil) mood = FACE_DIZZY;
  else if (now < surpriseUntil) mood = FACE_SURPRISE;
  else if (az < -0.35f) mood = FACE_UPSIDE;
  else if (now - stillSince > 3500) mood = FACE_SLEEPY;
  else mood = FACE_LOOK;

  if (mood != lastMood) {
    Serial.println(moodName(mood));
    lastMood = mood;
  }

  // ---- Dizzy just ended: start the smooth glide back to resting position ----
  if (prevMood == FACE_DIZZY && mood != FACE_DIZZY) {
    settling = true;
    settleStart = now;
    settleFromY = currentEyeY;
  }

  float tOpen = 1.0f;
  float tPupilX = 0, tPupilY = 0, tPupilScale = 1.0f;

  float tLookX = constrain(sx * 1.6f, -1.0f, 1.0f);
  float tLookY = constrain(sy * 1.6f, -1.0f, 1.0f);

  if (mood == FACE_DIZZY) {
    // ---- Scroll eyes straight down, wrapping back in from the top ----
    dizzyScrollY += DIZZY_SCROLL_SPEED * dt;
    float wrapRange = 240.0f + 2 * EYE_R;
    float wrapped = fmodf(dizzyScrollY, wrapRange) - EYE_R;
    currentEyeY = wrapped;
    tOpen = 1.0f;
    tPupilScale = 0.7f;
  } else if (settling) {
    float t = (now - settleStart) / (float)SETTLE_DURATION_MS;
    if (t >= 1.0f) {
      settling = false;
      currentEyeY = EYE_CY;
    } else {
      currentEyeY = smoothlerp(settleFromY, EYE_CY, smoothstep01(t));
    }
  } else {
    dizzyScrollY = 0;
    currentEyeY = EYE_CY;
  }

  if (mood == FACE_SURPRISE) {
    tOpen = 1.15f;
    tPupilScale = 0.5f;
  } else if (mood == FACE_FREEFALL) {
    tOpen = 1.2f;
    tPupilScale = 0.5f;
  } else if (mood == FACE_SLEEPY) {
    tOpen = 0.25f;
    tPupilY = 0.5f;
  } else if (mood == FACE_UPSIDE) {
    tOpen = 1.0f;
    tPupilY = -0.6f; // pupils look "up" since the device is flipped
  } else if (mood == FACE_LOOK && !settling) {
    tPupilX = tLookX;
    tPupilY = tLookY;
    if (now > nextBlinkAt && blinkUntil == 0) {
      blinkUntil = now + 120;
      nextBlinkAt = now + 1600 + (now % 2200);
    }
  }

  if (now < blinkUntil && mood != FACE_DIZZY) {
    tOpen = 0.06f;
  } else if (now >= blinkUntil) {
    blinkUntil = 0;
  }

  openAmt = smoothlerp(openAmt, tOpen, 0.3f);
  pupilOffX = smoothlerp(pupilOffX, tPupilX, 0.25f);
  pupilOffY = smoothlerp(pupilOffY, tPupilY, 0.25f);
  pupilScale = smoothlerp(pupilScale, tPupilScale, 0.25f);

  canvas.fillScreen(GC9A01A_BLACK);
  drawEye(120 - EYE_DX, currentEyeY, openAmt, pupilOffX, pupilOffY, pupilScale);
  drawEye(120 + EYE_DX, currentEyeY, openAmt, pupilOffX, pupilOffY, pupilScale);
  tft.drawRGBBitmap(0, 0, canvas.getBuffer(), 240, 240);
}