
Eyes · H
#ifndef TAO_EYES_H
#define TAO_EYES_H
 
#include <TFT_eSPI.h>
#include "config.h"

struct EyeParams [
  float scaleX;
  float scaleY;
  float spacing;
  float rotation;
];

class EyeRenderer {
    public:
    EyeRenderer(TFT_eSprite &spr) : sprite(spr) {}
 
    void setImmediate(const EyeParams &params) {
      current = params;
      target = params;
    }
 
    void setTarget(const EyeParams &params) {
      target = params;
    }
 
    void update() {
      current.scaleX   = lerp(current.scaleX,   target.scaleX,   EYE_LERP_SPEED);
      current.scaleY   = lerp(current.scaleY,   target.scaleY,   EYE_LERP_SPEED);
      current.spacing  = lerp(current.spacing,  target.spacing,  EYE_LERP_SPEED);
      current.rotation = lerp(current.rotation, target.rotation, EYE_LERP_SPEED);
    }
 
    void draw() {
      sprite.fillSprite(TFT_BLACK);
      int cx = SCREEN_W / 2;
      int cy = SCREEN_H / 2;
      int spacing = (int)current.spacing;
 
      drawTeardrop(cx - spacing, cy, current.rotation);
      drawTeardrop(cx + spacing, cy, -current.rotation);
 
      sprite.pushSprite(0, 0);
}

private: 
TFT_eSprite &sprite;
EyeParams current = {1.0f, 1.0f, 40.0f, 0.0f};
    EyeParams target   = {1.0f, 1.0f, 40.0f, 0.0f};
 
    static float lerp(float a, float b, float t) {
      return a + (b - a) * t;
    }

void drawTeardrop(int cx, int cy, float rotationDeg) {
      const int POINT_COUNT = 10;
      float localX[POINT_COUNT];
      float localY[POINT_COUNT];

      for (int i = 0; i < POINT_COUNT - 1; i++) {
        float t = (float)i / (float)(POINT_COUNT - 2);
        float angle = PI * t;
        localX[i] = cos(angle) * 18.0f;
        localY[i] = sin(angle) * 14.0f + 6.0f;
      }

      localX[POINT_COUNT - 1] = 0.0f;
      localY[POINT_COUNT - 1] = -22.0f;
 
      float rad = rotationDeg * DEG_TO_RAD;
      float cosR = cos(rad);
      float sinR = sin(rad);

      int16_t px[POINT_COUNT], py[POINT_COUNT];
      long sumX = 0, sumY = 0;
 
      for (int i = 0; i < POINT_COUNT; i++) {
        float sx = localX[i] * current.scaleX;
        float sy = localY[i] * current.scaleY;
        float rx = sx * cosR - sy * sinR;
        float ry = sx * sinR + sy * cosR;
        px[i] = cx + (int16_t)rx;
        py[i] = cy + (int16_t)ry;
        sumX += px[i];
        sumY += py[i];
      }

      int16_t centroidX = sumX / POINT_COUNT;
      int16_t centroidY = sumY / POINT_COUNT;

    for (int i = 0; i < POINT_COUNT; i++) {
        int j = (i + 1) % POINT_COUNT;
        sprite.fillTriangle(centroidX, centroidY, px[i], py[i], px[j], py[j], TFT_WHITE);
      }
    }
};
 
#endif
