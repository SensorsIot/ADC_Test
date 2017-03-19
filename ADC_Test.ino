#include <TFT.h>  // Arduino LCD library
#include <SPI.h>
#include <stdlib.h>

// pin definition for the Uno
#define cs   10
#define dc   9
#define rst  8

char buff[10];

TFT TFTscreen = TFT(cs, dc, rst);

void setup() {
  Serial.begin(115200);
  initTFT();
  pinMode(A0, INPUT);
}

void loop() {

  // read normal Arduino value

  int in0 = analogRead(A0);
  float val0 = in0 * 5.0 / 1024.0;

  // read correct supply voltage

  float supply = readVcc() / 1000.0;
  float val0Corrected = supply / 5 * val0;

  TFTscreen.background(0, 0, 0);

  dtostrf(val0, 4, 3, buff);
  TFTscreen.text("Orig: ", 0, 10);
  TFTscreen.text(buff, 100, 10);

 dtostrf(supply, 4, 3, buff);
  TFTscreen.text("VCC: ", 0, 30);
  TFTscreen.text(buff, 100, 30);

  dtostrf(val0Corrected, 4, 3, buff);
  TFTscreen.text("Corr: ", 0, 50);
  TFTscreen.text(buff, 100, 50);
  
  delay(500);
}


void initTFT() {
  // Put this line at the beginning of every sketch that uses the GLCD:
  TFTscreen.begin();

  // clear the screen with a black background
  TFTscreen.background(0, 0, 0);
  // write the static text to the screen
  // set the font color to white
  TFTscreen.stroke(255, 255, 255);
  // set the font size
  TFTscreen.setTextSize(2);
}

long readVcc() {
  long result;
  // Read 1.1V reference against AVcc
#if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
  ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
#elif defined (__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
  ADMUX = _BV(MUX5) | _BV(MUX0);
#elif defined (__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
  ADMUX = _BV(MUX3) | _BV(MUX2);
#else
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
#endif
  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Convert
  while (bit_is_set(ADCSRA, ADSC));
  result = ADCL;
  result |= ADCH << 8;
  result = 1126400L / result; // Calculate Vcc (in mV); 1126400 = 1.1*1024*1000
  return result;
}
