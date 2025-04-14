#include <FastLED.h>

// Define pins and number of LEDs
#define NUM_LEDS_1    60
#define LED_PIN_1     6
#define TRAIL_LENGTH_1 5
#define NUM_LEDS_2    45
#define LED_PIN_2     5
#define TRAIL_LENGTH_2 5

// Create LED arrays
CRGB leds1[NUM_LEDS_1];
CRGB leds2[NUM_LEDS_2];

// Animation parameters
const unsigned long TOTAL_ANIMATION_TIME = 25000;  // 20 seconds in milliseconds
const int NUM_SECTIONS = 3;                       // Number of colored sections
const int SECTION_LENGTH = 5;                     // Default section length (for calculations)
const int FLASH_COUNT = 3;                        // Number of yellow flashes at peak
const int FLASH_DURATION = 1000;                   // Duration of each yellow flash in ms
const int FLASH_PAUSE = 200;                      // Pause between flashes in ms
const int COLOR_CHANGE_INTERVAL = 1000;           // Change colors every 1 second

// Predefined colors (RGB values)
const CRGB COLORS[] = {
  CRGB(255, 0, 0),    // Red
  CRGB(0, 255, 0),    // Green
  CRGB(0, 0, 255),    // Blue
  CRGB(255, 0, 255),  // Purple
  CRGB(255, 165, 0),  // Orange
  CRGB(0, 255, 255)   // Cyan
};
const int NUM_COLORS = sizeof(COLORS) / sizeof(COLORS[0]);

// Animation state variables
unsigned long startTime;
unsigned long currentTime;
unsigned long lastColorChangeTime = 0;
bool animationComplete = false;
CRGB sectionColors[NUM_SECTIONS];

void setup() {
  // Initialize FastLED
  FastLED.addLeds<NEOPIXEL, LED_PIN_1>(leds1, NUM_LEDS_1);
  FastLED.addLeds<NEOPIXEL, LED_PIN_2>(leds2, NUM_LEDS_2);

  // Initial brightness
  FastLED.setBrightness(150);

  // Set random colors for each section
  randomSeed(analogRead(0));  // Seed random number generator
  for (int i = 0; i < NUM_SECTIONS; i++) {
    sectionColors[i] = COLORS[random(NUM_COLORS)];
  }

  // Record start time
  startTime = millis();

  // Clear all LEDs
  clearAllLEDs();
}

void loop() {
  currentTime = millis() - startTime;

  // If we're within the animation time, run the accelerating animation
  if (currentTime < TOTAL_ANIMATION_TIME) {
    runAcceleratingAnimation();
  } 
  // If we're at the peak, flash yellow
  else if (!animationComplete) {
    flashYellow();
    animationComplete = true;
    clearAllLEDs();
  } 
  // After animation is complete, restart it
  else {
    // Reset animation
    startTime = millis();
    animationComplete = false;

    // Choose new random colors
    for (int i = 0; i < NUM_SECTIONS; i++) {
      sectionColors[i] = COLORS[random(NUM_COLORS)];
    }
  }
}

void runAcceleratingAnimation() {
  // Calculate progress (0.0 to 1.0)
  float progress = (float)currentTime / TOTAL_ANIMATION_TIME;

  // Calculate speed using steps instead of exponential curve
  // Define speed steps with maximum of 1.0
  float speedMultiplier;

  if (progress < 0.2) {
    // First 20% of time - very slow
    speedMultiplier = 0.1;
  } else if (progress < 0.4) {
    // 20%-40% of time - slightly faster
    speedMultiplier = 0.25;
  } else if (progress < 0.6) {
    // 40%-60% of time - medium speed
    speedMultiplier = 0.5;
  } else if (progress < 0.85) {
    // 60%-80% of time - faster
    speedMultiplier = 0.75;
  } else {
    // Last 20% - maximum speed
    speedMultiplier = 1.0;
  }

  // Check if it's time to change colors
  if (currentTime - lastColorChangeTime >= COLOR_CHANGE_INTERVAL) {
    // Update one random section with a new random color
    int sectionToChange = random(NUM_SECTIONS);
    sectionColors[sectionToChange] = COLORS[random(NUM_COLORS)];
    lastColorChangeTime = currentTime;
  }

  // Calculate base position that cycles through the strip
  int maxPosition = max(NUM_LEDS_1, NUM_LEDS_2) + max(TRAIL_LENGTH_1, TRAIL_LENGTH_2);
  float basePosition = (currentTime * speedMultiplier / 1000.0) * maxPosition;

  // Clear LEDs
  clearAllLEDs();

  // Draw sections on both strips
  for (int section = 0; section < NUM_SECTIONS; section++) {
    // Calculate this section's position with offset
    float sectionPos = fmod(basePosition + (maxPosition / NUM_SECTIONS) * section, maxPosition);
    int pos = (int)sectionPos;

    // Draw section on strip 1
    drawSectionOnStrip(leds1, NUM_LEDS_1, pos, sectionColors[section]);

    // Draw section on strip 2
    drawSectionOnStrip(leds2, NUM_LEDS_2, pos, sectionColors[section]);
  }

  // Show the LEDs
  FastLED.show();
}

void drawSectionOnStrip(CRGB* strip, int stripLength, int position, CRGB color) {
  // Draw the section if it's visible on the strip
  int trailLength = (strip == leds1) ? TRAIL_LENGTH_1 : TRAIL_LENGTH_2;

  for (int i = 0; i < trailLength; i++) {
    int pixelPos = position - i;
    if (pixelPos >= 0 && pixelPos < stripLength) {
      strip[pixelPos] = color;
    }
  }
}

void flashYellow() {
  CRGB brightYellow = CRGB(255, 255, 0);

  // Perform the flashes
  for (int flash = 0; flash < FLASH_COUNT; flash++) {
    // Turn all LEDs bright yellow
    fill_solid(leds1, NUM_LEDS_1, brightYellow);
    fill_solid(leds2, NUM_LEDS_2, brightYellow);
    FastLED.setBrightness(255);  // Full brightness
    FastLED.show();
    delay(FLASH_DURATION);

    // Turn off all LEDs between flashes (except for the last one)
    if (flash < FLASH_COUNT - 1) {
      clearAllLEDs();
      FastLED.show();
      delay(FLASH_PAUSE);
    }
  }

  // Reset brightness to normal
  FastLED.setBrightness(150);
}

void clearAllLEDs() {
  // Clear both LED strips
  fill_solid(leds1, NUM_LEDS_1, CRGB::Black);
  fill_solid(leds2, NUM_LEDS_2, CRGB::Black);
}
