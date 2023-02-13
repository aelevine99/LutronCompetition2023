/*
Synesthesia
Written by Alexander Levine for Boston University SILab Lutron Lighting Innovation Competition 2023
https://www.bu.edu/eng/academics/teaching-and-innovation/singh-imagineering-lab-silab/competitions/lutron-lighting-competition/

MIT License
Copyright (c) 2023 Alexander levine
*/

// ~~~~~ LIBRARIES ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include <math.h>
#include <Adafruit_NeoPixel.h>  // documentation https://github.com/kosme/arduinoFFT

// ~~~~~ CONSTANTS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define PIN_MIC_A A0  // audio input, analog
#define PIN_MIC_D 3   // audio input, digital
#define PIN_LED 10    // light output, digital
#define PIX 16        // number of neopixels

#define size 5  // will use for array size later, didn't want to be a variable :(

// ~~~~~ GLOBALS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Adafruit_NeoPixel lx = Adafruit_NeoPixel(PIX, PIN_LED, NEO_GRB + NEO_KHZ800);  // instantiate Neopixel object

int i = 0;          // loop counter
int va = 0;         // store volume level
bool vd = 0;        // store volume trigger
int va_read[size];  // store past volume levels
float va_avg = 0;   // store average volume level
float delta = 0;    // sotre std dev of va_read
float bright = 0;   // brightness level
int color_cur = 0;  // current color selection, used to iterate



uint32_t r = lx.Color(255, 0, 0);  // pack rgb value into a 32 bit val
uint32_t o = lx.Color(255, 165, 0);
uint32_t y = lx.Color(255, 255, 0);
uint32_t g = lx.Color(0, 255, 0);
uint32_t b = lx.Color(0, 0, 255);
uint32_t v = lx.Color(127, 0, 255);
uint32_t w = lx.Color(255, 255, 255);

// ~~~~~ MAIN FUNCTIONS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void setup() {
  // put your setup code here, to run once:
  pinMode(PIN_MIC_A, INPUT);
  pinMode(PIN_MIC_D, INPUT);
  pinMode(PIN_LED, OUTPUT);
  Serial.begin(9600);                         // initiate serial transmission
  Serial.println("\n\n\nInitializing...");    //
  lx.begin();                                 // intialize neopixel object
  lx.setBrightness(255);                      // set brightness max
  lx.fill(w, 0);                              //
  lx.show();                                  //
  delay(1000);                                //
  for (int k = 0; k < 6; k++) {               // run through all colors
    color_sel(k);                             //
    lx.show();                                //
    delay(250);                               //
  }                                           //
  lx.fill(w, 0);                              //
  lx.show();                                  //
  delay(1000);                                //
  Serial.println("Neopixel initialized...");  //
  lx.clear();                                 // clear neopixels
  Serial.println("Neopixel cleared...");      //
  lx.show();                                  // push data to neopixels
  Serial.println("Initialization complete");  //
  delay(2000);                                //
}

void loop() {
  // Read in values
  va = analogRead(PIN_MIC_A);   // read value of analog audio input
  vd = digitalRead(PIN_MIC_D);  // read value of digital pin

  va_read[i] = va;

  // Set brightness
  bright = map(va, 1023, 0, 0, 255);  // map volume level to brightness. inverted
  lx.setBrightness(bright);

  // Color select
  va_avg = avger(va_read);                 // running average of volume
  i = (i + 1) % size;                      // find loop val in terms of size
  if ((va - va_avg) > 7) { color_cur++; }  // increment color select if va is n above avg
  color_sel(color_cur);                    // set color

  // Serial stuff
  Serial.print("Volume: ");
  Serial.print(va);
  Serial.print("\t Average: ");
  Serial.print(va_avg, 0);
  Serial.print("\t Threshold: ");
  Serial.print(vd);
  Serial.print("\t Brightness: ");
  Serial.println(bright, 0);

  // Push updated vals to neopixel
  lx.show();
  delay(5);  // neopixels require a buffer time of 300 μs
}

// ~~~~~ ADD'L FUNCTIONS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void color_sel(int col) {  // selects color based on input value
  switch (col % 6) {
    case 0:              // red
      lx.fill(r, 0, 0);  // set whole string to color
      break;
    case 1:  // orange
      lx.fill(o, 0, 0);
      break;
    case 2:  // yellow
      lx.fill(y, 0, 0);
      break;
    case 3:  // green
      lx.fill(g, 0, 0);
      break;
    case 4:  // blue
      lx.fill(b, 0, 0);
      break;
    case 5:  // violet
      lx.fill(v, 0, 0);
      break;
  }
}

float avger(int* array) {
  int sum = 0;                                         // running sum var
  for (int k = 0; k < size; k++) { sum += array[k]; }  // sum all elements in va_read
  return sum / size;                                   // return average of va_read
}
