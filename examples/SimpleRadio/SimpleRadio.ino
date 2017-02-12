#include <DSP6951.h>

DSP6951 radio;

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.setVolume(50);
  radio.setMode(DSP6951::FM);
  radio.setChannel(81.3);
}

void loop() {
  Serial.println(radio.getCNR());
  delay(100);
}
