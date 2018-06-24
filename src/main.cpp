#include <Arduino.h>

#include "decoder.h"
#include "lcd.h"
#include "bargraph.h"

LCD tft;

void setup() {
  // Configure pins
  GPIOB->regs->CRL = 0x33333333;
  GPIOB->regs->CRH = 0x33333333;
  pinMode(PB13, INPUT);
  pinMode(PB14, INPUT);
  pinMode(PB15, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
#ifdef DEBUG
  Serial.begin(9600);
  // Wait for serial
  while (!Serial)
    delay(100);
#endif
  tft.begin();
#ifndef USE_SSD1322_DISPLAY
  tft.invertDisplay(true);
  tft.setRotation(1);
#endif
  tft.fillScreen(LCD_BLACK);
  enableBar();
  Timer4.setMode(TIMER_CH1, TIMER_OUTPUTCOMPARE);
  Timer4.setPeriod(1000000); // in microseconds
  Timer4.setCompare(TIMER_CH1, 1);      // overflow might be small
  Timer4.attachInterrupt(TIMER_CH1, &fpsInterrupt);  // Start sniffing
  attachInterrupt(PB13, &sckInterrupt, RISING);
  digitalWrite(LED_BUILTIN, HIGH);
}

void loop() {
  process();
}
