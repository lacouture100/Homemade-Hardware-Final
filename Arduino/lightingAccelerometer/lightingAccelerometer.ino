// Neopixels
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

// ACCELEROMTER LIS3DH
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_LIS3DH.h>
#include <Adafruit_Sensor.h>

#define PIN 6
#define NUMPIXELS 64
#define PIXELTYPE NEO_GRB + NEO_KHZ800

#define CLICKTHRESHHOLD 80


//define the neopixels as pixels
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, PIXELTYPE);

// define the Accelerometer 12C as lis
Adafruit_LIS3DH lis = Adafruit_LIS3DH();

void setup() {

  Serial.begin(9600);

  // NEOPIXELS
  pixels.begin(); // Initialize the pixels
  pixels.setBrightness(10); // Set brightness. Max is 255
  pixels.show(); // Initialize all pixels to 'off'

  // ACCELEROMETER
  lis.begin(0x18); // begins the accelerometer
  lis.setClick(4, CLICKTHRESHHOLD); // this sets up the Accelerometer to read taps
  lis.setRange(LIS3DH_RANGE_4_G);   // 2, 4, 8 or 16 G!
}

void loop() {
  //Get a new accelerometer event, normalized 
  sensors_event_t event;
  lis.getEvent(&event);

  // Run the animation
  animation_rainbow(2, event.acceleration.x, event.acceleration.z);
}


void tapBrightness() {

}
// Fill the dots in a sequence one by one
void colorWipe(uint32_t c, uint8_t wait) {
  for (uint16_t i = 0; i < pixels.numPixels(); i++) {
    pixels.setPixelColor(i, c);
    pixels.show();
    delay(wait);
  }
}

void animation_rainbow(int wait, int accX, int accZ) {
  uint16_t i, j;
  int speedVal = abs(int(accX) + 1);
  int brightVal = abs(accZ * 20) + 10;
  speedVal = map(speedVal, 0, 10, 0, 7);
  pixels.setBrightness(brightVal); // Set brightness. Max is 255
  for (j = 0; j < 255; j++) {
    for (i = 0; i < pixels.numPixels(); i++) {
      pixels.setPixelColor(i, Wheel(((i + j)*speedVal) & 255));
    }
    pixels.show();
    delay(wait);//this delay makes it work a bit better
  }
}


// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    //The lower the number, the more red it is
    // When it gets higher, Blue starts going upwards and Red goes down
    // When Wheelpos is 0, (255,0,0), when it's 85, (0,0,255)
    //RED TO BLUE
    return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    //BLUE TO GREEN
    //0, (0,0,255) // 170, (0,255,0)
    return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  //GREEN TO RED
  WheelPos -= 170;
  return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
