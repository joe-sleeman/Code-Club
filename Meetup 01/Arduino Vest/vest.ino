#include <Adafruit_NeoPixel.h>
 
#define VESTRHSPIN 7 //Right hand side of vest pin number
#define NOPINS 10    //Number of pins in each 'strand'
#define MIC_PIN   A0 // Microphone is attached to this analog pin

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel vestRHSstrip = Adafruit_NeoPixel(NOPINS, VESTRHSPIN, NEO_GRB + NEO_KHZ800);
// Set up pixel strips

int history[NOPINS];
int currentPos=0; //Current pointer in history array

void setup() {
  memset(history,0,sizeof(history));
  //Set up both sides
  vestRHSstrip.begin();
  
  //Set up brightness
  vestRHSstrip.setBrightness(30);
  
  //Initialize all pixels to 'off'
  vestRHSstrip.show();
  
  //Do something pretty for startup
  rainbowCycle(5);
}

//Main program loop
void loop(){
  uint8_t i;
  int n;//Raw data from microphone
  
  //Read value from Microphone
  n   = analogRead(MIC_PIN);
  
  //Rotate around history, add one and flick back to zero if need be
  currentPos++;
  currentPos%=NOPINS;
  
  //Store current reading in history
  history[currentPos]=n;
  
  //Set the pixel colours
 
  for (i=0;i<NOPINS;i++){
    //Choose a color based on history
    uint32_t colorNum=Wheel(map(history[(i+currentPos)%NOPINS],300,500,0,255)&205);
    vestRHSstrip.setPixelColor(i,colorNum);
  }
  //Push pixel color changes out to the strip
  vestRHSstrip.show();
  delay(100);
  //Make some pretty colours for testing
  //TODO: Need to rewrite all the example code so that both side can be set at the same time..
  //TODO: Button inputs to make various patterns go off, instead of the microphone reaction?
  //TODO: Make different patterns go off at regular intervals? Racing LED up one side, down the other etc
  //TODO: Microphone leveling/averaging
}


// -----------------------------------------------
// Imported colour methods from adafruit
// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<vestRHSstrip.numPixels(); i++) {
      vestRHSstrip.setPixelColor(i, c);
      vestRHSstrip.show();
      delay(wait);
  }
}
 
void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<vestRHSstrip.numPixels(); i++) {
      vestRHSstrip.setPixelColor(i, Wheel((i+j) & 255));
    }
    vestRHSstrip.show();
    delay(wait);
  }
}

 
// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;
 
  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< vestRHSstrip.numPixels(); i++) {
      vestRHSstrip.setPixelColor(i, Wheel(((i * 256 / vestRHSstrip.numPixels()) + j) & 255));
    }
    vestRHSstrip.show();
    delay(wait);
  }
}
 
// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return vestRHSstrip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return vestRHSstrip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return vestRHSstrip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}
