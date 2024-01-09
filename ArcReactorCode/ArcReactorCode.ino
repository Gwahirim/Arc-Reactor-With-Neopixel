#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// ... (other variable declarations and setup functions)
#define BUTTON_PIN   9   // Butonun bağlı olduğu pin

#define PIXEL_PIN    6  // Pixellerin bağlı olduğu pin

int R=3;
int G=4;
int B=5;

#define PIXEL_COUNT 24 

#define BATTERY_PIN A0


 int TOTAL_LEDS = 24;
 float MaximumBrightness = 255;
 float SpeedFactor = 0.008;
 float StepDelay = 5;
// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
bool rainbowMode = false;
int currentColor = 0;
boolean oldState = HIGH;
int     mode     = 0;    // Currently-active animation mode, 0-3
void setup() {
  pinMode(R,OUTPUT);
  pinMode(G,OUTPUT);
  pinMode(B,OUTPUT);

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  strip.begin(); // Initialize NeoPixel strip object (REQUIRED)
  strip.show();  // Initialize all pixels to 'off'
}

void loop() {
  analogWrite(R,255);
  analogWrite(G,255);
  analogWrite(B,255);
  // Get current button state.
  boolean newState = digitalRead(BUTTON_PIN);

  // Check if state changed from high to low (button press).
  if ((newState == LOW) && (oldState == HIGH)) {
    // Short delay to debounce button.
    delay(20);
    // Check if button is still low after debounce.
    newState = digitalRead(BUTTON_PIN);
    if (newState == LOW) {      // Eğer buton LOW ise
      if (++mode > 4) mode = 0; // Modlar arası geçiş ve 3. moddan sonra başa sarış
      switch (mode) {           // Animasyon başlatma
        case 0:
         analogWrite(R,255);
         analogWrite(G,255);
         analogWrite(B,255);
         animation(50, 10, 0.008, 255, 0, 0, 0);// Kapalı mod
          break;
        case 1:
          analogWrite(R,255);
          analogWrite(G,127);
          analogWrite(B,0);
          animation(50, 15, 0.015, 255, 0, 127, 255); //(10 saniyede mavi) Tüm ledlerin yanma süresi, açık kalma süresi , breathe efekti süresi genellikle sabit, parlaklık, kırmızı, mavi, yeşil
          break;
        case 2:
          analogWrite(R,0);
          analogWrite(G,255);
          analogWrite(B,255);
          animation(30, 0.5, 0.008, 255, 255, 0, 0);//(0.5 saniyede kırmızı) Tüm ledlerin yanma süresi, açık kalma süresi , breathe efekti süresi genellikle sabit, parlaklık, kırmızı, mavi,

          break;
        case 3:
        analogWrite(R,255);
        analogWrite(G,127);
        analogWrite(B,0);
        rainbow();
          break;
      }
    }
  } 
}

void animation(int wait, float Stepdelay, float SpeedFactor, float MaximumBrightness, int r, int g, int b){
int total =24;
bool stopBothLoops = false;
for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, r, g, b);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
  for (int i = 0; i < 65535 && !stopBothLoops; i++) {
  float intensity = MaximumBrightness / 2.0 * (1.0 + sin(SpeedFactor * i));
  strip.setBrightness(intensity);

  for (int ledNumber = 0; ledNumber < total && !stopBothLoops; ledNumber++) {
    strip.setPixelColor(ledNumber, r, g, b);

    // Düğme basıldığında her iki döngüyü birden durdurun
    if (digitalRead(BUTTON_PIN) == LOW) {
      stopBothLoops = true;
      break; // İç içe olan ikinci for döngüsünü sonlandır
    }
  }

  strip.show();

  // Düğme basıldığında her iki döngüyü birden durdurun
  if (digitalRead(BUTTON_PIN) == LOW) {
    stopBothLoops = true;
    break; // Dıştaki for döngüsünü sonlandır
  }

  // Wait a bit before continuing to breathe
  delay(Stepdelay);
}
}

void rainbow() {
  bool stopBothLoops = false;
  // Hue of first pixel runs 3 complete loops through the color wheel.
  // Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to 3*65536. Adding 256 to firstPixelHue each time
  // means we'll make 3*65536/256 = 768 passes through this outer loop:
    for (int j = 0; j < 65535 && !stopBothLoops; j++) {
    for(long firstPixelHue = 0; firstPixelHue < 3*65536; firstPixelHue += 256) {

    for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...

      // Offset pixel hue by an amount to make one full revolution of the
      // color wheel (range of 65536) along the length of the strip
      // (strip.numPixels() steps):
      int pixelHue = firstPixelHue + (i * 65536 / strip.numPixels());
      // strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
      // optionally add saturation and value (brightness) (each 0 to 255).
      // Here we're using just the single-argument hue variant. The result
      // is passed through strip.gamma32() to provide 'truer' colors
      // before assigning to each pixel:
      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));



    }
    strip.show(); // Update strip with new contents

    delay(5);
                  if (digitalRead(BUTTON_PIN) == LOW && j >= 1 ) {
                stopBothLoops= true;
                break;
  
  }  
  }
  
  }
}

