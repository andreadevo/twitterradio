#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN 6   // pin controlling strip
#define NUM_LIGHTS 19

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LIGHTS, PIN, NEO_GRB + NEO_KHZ800);

char val;
char quitVal = 'q';
char inVal; 
boolean setColor = false;
uint32_t low; 
uint32_t high;

void setup() {
  // put your setup code here, to run once:
  strip.begin();
  strip.show();
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available()){
    // incoming serial data
    inVal = Serial.read();
    if (inVal != ' '){
      val = inVal;
    }
//    Serial.println(val, DEC);
    
    // set color based on emotion: anger, disgust, fear, joy, sadness, undefined
    if (val == 'a'){
      // ANGER
      quitVal = 'p';
      low = strip.Color(255, 0, 0);     // red
      setColors(low); 
    }
    else if (val == 'd'){
      // DISGUST
      quitVal = 'p';
      low = strip.Color(0, 255, 0);     // red
      setColors(low); 
    }
    else if (val == 'f'){
      // FEAR
      quitVal = 'p'; 
      low = strip.Color(128,0,128);     // purple
      setColors(low);
    }
    else if (val == 'j'){
      // JOY
      quitVal = 'p';
      low = strip.Color(250, 150, 0);  // yellow
      setColors(low); 
    }
    else if (val == 's'){
      // SADNESS
      quitVal = 'p';
      low = strip.Color(0, 0, 255);    // blue
      setColors(low);
    }
    else if (val == 'u'){
      // undefined
      quitVal = 'u';
      rainbow(20);
    }
    else if (val == 'q'){
      // song stopped playing
      quitVal = 'q';
      rainbowCycle(20);
    }
  }
  if (quitVal == 'q'){
      rainbowCycle(20);
  }
  else if (quitVal == 'u'){
    rainbow(20);
  }
}

void setColors(uint32_t c){
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
  }
}

// Shift through colors of the rainbow
void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

void fadeInAndOut(uint8_t red, uint8_t green, uint8_t blue, uint8_t wait) {

  for(uint8_t b=30; b <255; b++) {
     for(uint8_t i=0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, red*b/255, green*b/255, blue*b/255);
     }
     strip.show();
     delay(wait);
  }

  for(uint8_t b=255; b > 30; b--) {
     for(uint8_t i=0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, red*b/255, green*b/255, blue*b/255);
     }
     strip.show();
     delay(wait);
  }
}

