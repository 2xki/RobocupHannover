#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif
#include "MPU9250.h"
#define LED_PIN   D3
#define LED_COUNT 72

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
MPU9250 IMU(Wire,0x68);
int status;
float x = 0;
float y = 0;
float tx = 0;
float ty = 0;
void setup() {
  Serial.begin(9600);
  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(255); // Set BRIGHTNESS to about 1/5 (max = 255)
  status = IMU.begin();
  delay(2000);
  IMU.readSensor();
  tx = IMU.getAccelX_mss();
  ty = IMU.getAccelY_mss();
}

void loop() {
  IMU.readSensor();
  x = IMU.getAccelX_mss();
  y = IMU.getAccelY_mss();
  Serial.println(x);
  Serial.println(y);
  if(x < tx-0.08 || x > tx+0.08 || y < ty-0.08 || y > ty+0.08){
    Serial.println("Achtung");
    colorWipe(strip.Color(255,   0,   0),1);
    delay(500);
  }
  else{
    colorWipe(strip.Color(0,   255,   0),0.1);
  }

  

}

void rainbow(int wait) {
  // Hue of first pixel runs 5 complete loops through the color wheel.
  // Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to 5*65536. Adding 256 to firstPixelHue each time
  // means we'll make 5*65536/256 = 1280 passes through this outer loop:
  for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {
    for(int i=0; i<9; i++) { // For each pixel in strip...
      // Offset pixel hue by an amount to make one full revolution of the
      // color wheel (range of 65536) along the length of the strip
      // (strip.numPixels() steps):
      int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
      // strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
      // optionally add saturation and value (brightness) (each 0 to 255).
      // Here we're using just the single-argument hue variant. The result
      // is passed through strip.gamma32() to provide 'truer' colors
      // before assigning to each pixel:
      layer(i, strip.gamma32(strip.ColorHSV(pixelHue)));
    }
    strip.show(); // Update strip with new contents
    delay(wait);  // Pause for a moment
  }
} 

void layer(int layer, uint32_t color) { // 
  for(int i=1; i<8; i++) {
    strip.setPixelColor(i * layer - 1, color);
    strip.show(); 
  }
}

void colorWipe(uint32_t color, int wait) {
  for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}
