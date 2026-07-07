#ifndef TAO_MOOD_H
#define TAO_MOOD_H

#include "config.h"
#include "eyes.h"

enum MoodState {
  MOOD_STILL,
  MOOD_MOVING,
  MOOD_SHAKE
};

class MoodEngine {
  public:
    MoodEngine() {} 

  // call
  bool update(float accelDeltaMs2, unsigned long nowMs) {
      MoodState previous = state;
 
      if (accelDeltaMs2 > SHAKE_THRESHOLD_MS2) {
        state = MOOD_SHAKE;
        shakeHoldUntilMs = nowMs + SHAKE_HOLD_MS;
      } else if (nowMs < shakeHoldUntilMs) {
        state = MOOD_SHAKE; // still holding the shake pose briefly
      } else if (accelDeltaMs2 < STILL_THRESHOLD_MS2) {
        state = MOOD_STILL;
      } else {
        state = MOOD_MOVING;
      }
 
      return state != previous;
    }
 
    MoodState getState() const {
      return state;
    }

    // eye shape and stuff (will change later)

    static EyeParams targetForState(MoodState s) {
      switch (s) {
        case MOOD_STILL:
          return {1.3f, 0.6f, 40.0f, 0.0f};   // squashed, wide
        case MOOD_SHAKE:
          return {0.7f, 1.4f, 55.0f, 20.0f};  // tall, narrow, tips flung outward
        case MOOD_MOVING:
        default:
          return {1.0f, 1.0f, 40.0f, 0.0f};   // normal upright
      }
    }
 
    static const char *nameForState(MoodState s) {
      switch (s) {
        case MOOD_STILL:  return "STILL";
        case MOOD_SHAKE:  return "SHAKE";
        case MOOD_MOVING: return "MOVING";
        default:          return "UNKNOWN";
      }
    }
 
  private:
    MoodState state = MOOD_STILL;
    unsigned long shakeHoldUntilMs = 0;
};

#endif