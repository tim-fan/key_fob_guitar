#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioInputAnalog         adc1;           //xy=274,245
AudioOutputUSB           usb1;           //xy=656,252
AudioConnection          patchCord1(adc1, 0, usb1, 0);
AudioConnection          patchCord2(adc1, 0, usb1, 1);
// GUItool: end automatically generated code


void setup() {
  // put your setup code here, to run once:
  AudioMemory(30);
}

void loop() {
  // put your main code here, to run repeatedly:

}
