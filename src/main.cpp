#include <Arduino.h>
#include <FastLED.h>

// Both Plasma Stick 2040 W and Plasma 2040 use pin 15 for LED data, see value of DAT in:
// https://github.com/pimoroni/pimoroni-pico/blob/main/libraries/plasma_stick/plasma_stick.hpp
// https://github.com/pimoroni/pimoroni-pico/blob/main/libraries/plasma2040/plasma2040.hpp
#define LED_DAT_PIN     15
#define LED_RGB_ORDER   EOrder::RGB
#define NUM_LEDS        50
#define LED_BRIGHTNESS  64
#define LOOP_DELAY      20

CRGB g_LEDs[NUM_LEDS] = {0};

void TestSolid(const CRGB &colour) {
    fill_solid(g_LEDs, NUM_LEDS, colour);
    FastLED.show();
    delay(1000);
}

void setup() {
    Serial.begin();
    delay(2000); // "while (!Serial) {}" blocks on RP2040 unless USB is actually connected, so we just delay instead
    Serial.printf("\nPlasmaFastLed started\n");

    pinMode(LED_DAT_PIN, OUTPUT);

    FastLED.addLeds<WS2812B, LED_DAT_PIN, LED_RGB_ORDER>(g_LEDs, NUM_LEDS);
    FastLED.setBrightness(LED_BRIGHTNESS);

    // Confirm we have correct RGB order by showing all LEDs in each primary colour for one second
    Serial.println("Starting colour tests");
    TestSolid(CRGB::Red);
    TestSolid(CRGB::Green);
    TestSolid(CRGB::Blue);

    Serial.println("Starting rainbow test");
}

void loop() {
    const uint8_t deltaHue = 256/NUM_LEDS;  // change in hue between LEDs required to display entire rainbow
    const uint8_t stepHue = 1;              // change in initial hue each time we are called
    static uint8_t initialHue = 0;          // current initial hue

    // Display simple moving rainbow effect for testing
    fill_rainbow(g_LEDs, NUM_LEDS, initialHue, deltaHue);
    FastLED.show();
    initialHue += stepHue;

    EVERY_N_SECONDS(3) {
        // Print something to show we are still running, even if LEDs are not working
        Serial.print(".");
    }

    delay(LOOP_DELAY);
}
