#ifndef TAO_HAPTIC_H
#define TAO_HAPTICS_H

#include <Adafruit_DRV2605.h>
#include "config.h"
#include "mood.h"
 
class HapticController {
  public:
    bool begin() {
      ready = driver.begin();
      if (ready) {
        driver.selectLibrary(1);
        driver.setMode(DRV2605_MODE_INTTRIG);
      }
      return ready;
    }

    void playForTransition(MoodState newState) {
      if (!ready) return;
 
      switch (newState) {
        case MOOD_SHAKE:
          play(HAPTIC_EFFECT_SHAKE);
          break;
        case MOOD_MOVING:
          play(HAPTIC_EFFECT_MOVING);
          break;
        case MOOD_STILL:
        default:
          break; 
      }
    }

    bool isReady() const {
      return ready;
    }

    private:

    Adafruit_DRV2605 driver;
    bool ready = false;
 
    void play(uint8_t effect) {
      driver.setWaveform(0, effect);
      driver.setWaveform(1, 0); // terminates the sequence
      driver.go();
    }
};

#endif
    