/*
 * int ones = t%10;
   int tens = (t/10)%10;
   int hundreds = (t/100)%10;
   int thousands = (t/1000)%10;
 * 
 *                     
 * 
*/
#include "tinyNeoPixel.h"
#include "EncoderStepCounter.h"
#include <EEPROM.h>
#include <stdio.h>


// Rotary Encoder Inputs
#define ENCODER_PIN1 A4
#define ENCODER_PIN2 A2

//----neopixel pin----
#define led_PIN 16
//------------------------------------------------------------
#define BUTTON_PIN 4
byte lastButtonState = LOW;
unsigned long debounceDuration = 50; // millis
unsigned long lastTimeButtonStateChanged = 0;
//int buttonState = 0;
//const int ButtonPin =  4;      // set button gnd side
const int ledPin =  12;        // set button gnd side
int buttonPresses = 0;

//------------------------------------------------------------


String currentDir ="";
//int Pin_TX  = 7;             // use tx pin as signal
//int Pin_RX  = 6;             // use rx pin as signal 

unsigned char pos;

EncoderStepCounter encoder(ENCODER_PIN1, ENCODER_PIN2);

unsigned char lastpos = 0;


int ones;     //I2C  display at address 4
int tens;     //I2C  display at address 5 
int huns;     //I2C  display at address 6

int lednum; 
//---------------------------------------------
                // for hexidecimal display, 
                // or TIL311 emulation



tinyNeoPixel strip = tinyNeoPixel(7, led_PIN, NEO_GRB + NEO_KHZ800);
//------------------------------  

void setup()
{
  strip.begin();
  strip.show(); // Initialize all pixels to 'off' 
  
  pinMode(BUTTON_PIN, INPUT_PULLUP);

 Serial.begin(9600); 

  encoder.begin();
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);         // set button gnd side


  

  
}


void(* resetFunc) (void) = 0;
//-----------------------------------------------------------------


//----------------------------------------------------------------




void loop()
{
//--------------------------------------------------------
  // Some example procedures showing how to display to the pixels:
//  colorWipe(strip.Color(255, 0, 0), 50); // Red
//  colorWipe(strip.Color(0, 255, 0), 50); // Green
//  colorWipe(strip.Color(0, 0, 255), 50); // Blue
  // colorWipe(strip.Color(0, 0, 0, 255), 50); // White RGBW
  // Send a theater pixel chase in...
//  theaterChase(strip.Color(127, 127, 127), 50); // White
//  theaterChase(strip.Color(127, 0, 0), 50); // Red
//  theaterChase(strip.Color(0, 0, 127), 50); // Blue

//  rainbow(20);
//  rainbowCycle(20);
//  theaterChaseRainbow(50);

//--------------------------------------------------------

delay(12);
//----------------------------
 encoder.tick();
     unsigned char pos = encoder.getPosition();
       if (pos > 0) {
         if (pos != lastpos){
          //  delay(12);
              lastpos = pos;

//-------------------------------------------------------------------
                  strip.setPixelColor(pos-1, 5);
                     strip.show();
                   strip.clear();
//-------------------------------------------------------------------
               Serial.println(pos);
               
              
                 int ones = (pos%10);
                 int tens = ((pos/10)%10);
                 int huns = ((pos/100)%10);
               
                 EEPROM.put(1, ones);
                 EEPROM.put(10, tens);
                 EEPROM.put(100, huns);

                 
              
          }
          
        }   

//----------------encoder  button-------------------------

if (millis() - lastTimeButtonStateChanged > debounceDuration) {
     
      byte buttonState = digitalRead(BUTTON_PIN);
      if (buttonState != lastButtonState) {
      lastTimeButtonStateChanged = millis();
      lastButtonState = buttonState;

      if (buttonState == LOW) {
        // do an action, for example print on Serial
      //  Serial.println("down");
          buttonPresses++; 
         //  Serial.println(buttonPresses);
            switch(buttonPresses){
                  case 1:
                           Serial.println("red");
                           colorWipe(strip.Color(5, 0, 0), 50); 
                  break;
                  case 2:
                          Serial.println("green");
                          colorWipe(strip.Color(0, 5, 0), 50); 
                  break;
                  case 3:
                          Serial.println("blue");
                          colorWipe(strip.Color(0, 0, 5), 50); 
                  break;
                  case 4:
                          buttonPresses = 0;
                  break;
            }
      
     // resetFunc();   
      }
      
      if (buttonState == HIGH) {
        // do an action, for example print on Serial
       // Serial.println("up");
      //  Serial.print(buttonPresses); 
      } 
      
      }
      }
      

//------------------------------------------------------

}             // loop end
  
//---------------------------------------------------


// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for (j = 0; j < 256; j++) {
    for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for (j = 0; j < 256 * 5; j++) { // 5 cycles of all colors on wheel
    for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j = 0; j < 10; j++) { // do 10 cycles of chasing
    for (int q = 0; q < 3; q++) {
      for (uint16_t i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, c);  // turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, 0);      // turn every third pixel off
      }
    }
  }
}

// Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j = 0; j < 256; j++) {   // cycle all 256 colors in the wheel
    for (int q = 0; q < 3; q++) {
      for (uint16_t i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, Wheel((i + j) % 255)); // turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, 0);      // turn every third pixel off
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
