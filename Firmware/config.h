#ifndef TAO_CONFIG_H
#define TAO_CONFIG_H

static const int SCREEN_W = 240;
static const int SCREEN_H = 240;

static const float STILL_THRESHOLD_MS2 = 0.15f;  // below is still
static const float SHAKE_THRESHOLD_MS2 = 6.0f; // above is shake
static const unsigned long SHAKE_HOLD_MS = 600; 

static const float EYE_LERP_SPEED = 0.15f;
static const int FRAME_DELAY_MS = 33;

static const uint8_t HAPTIC_EFFECT_SHAKE = 47;
static const uint8_t HAPTIC_EFFECT_MOVING = 14;

#endif