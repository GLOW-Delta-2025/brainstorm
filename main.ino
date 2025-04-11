#include <FastLED.h>

#define NUM_LEDS_1 60
#define LED_PIN_1 6
#define TRAIL_LENGTH_1 10

#define NUM_LEDS_2 45
#define LED_PIN_2 5
#define TRAIL_LENGTH_2 10

CRGB leds1[NUM_LEDS_1];
CRGB leds2[NUM_LEDS_2];

void setup() {
  FastLED.addLeds<NEOPIXEL, LED_PIN_1>(leds1, NUM_LEDS_1);
  FastLED.addLeds<NEOPIXEL, LED_PIN_2>(leds2, NUM_LEDS_2);
}

void loop() {
  for (int i = 0; i < max(NUM_LEDS_1 + TRAIL_LENGTH_1, NUM_LEDS_2 + TRAIL_LENGTH_2); i++) {
    // --- Strip 1: Purple trail ---
    if (i < NUM_LEDS_1) {
      leds1[i] = CRGB::Purple;
    }
    int offIndex1 = i - TRAIL_LENGTH_1;
    if (offIndex1 >= 0 && offIndex1 < NUM_LEDS_1) {
      leds1[offIndex1] = CRGB::Black;
    }

    // --- Strip 2: Blue trail ---
    if (i < NUM_LEDS_2) {
      leds2[i] = CRGB::Blue;
    }
    int offIndex2 = i - TRAIL_LENGTH_2;
    if (offIndex2 >= 0 && offIndex2 < NUM_LEDS_2) {
      leds2[offIndex2] = CRGB::Black;
    }

    FastLED.show();
    delay(30);
  }

  delay(500);
}
