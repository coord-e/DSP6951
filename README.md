# DSP6951
Library for DSP6951 DSP Radio IC.

## Supported devices
- [DSP6951](http://www.aitendo.com/product/7350)
- [M6951 module](http://www.aitendo.com/product/7011)

## Known issue
- Tuning to AM doesn't work.

## Getting started
### Circuit
![circuit](http://blog.y-modify.org/wp-content/uploads/2017/02/m6951-1.png)
According to datasheet, the rated voltage is 3.3v.
However in my environment, it didn't work with 3.3v.
So I powers it with 5v and it worked successfully. (Also maximun voltage is 5.8v, so it is OK for the moment)

### Sketch
```cpp
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
  Serial.println(radio.getCNR()); //get "Carrier to noise ratio" and print it
  delay(300);
}
```

## API Reference
### `void begin(void);`
Initialize IC and prepare to work.

### `void setVolume(uint8_t volume);`
Set output volume to the specified value(0-100).

### `void setChannel(float freq);`
Set channel to the specified frequency.

### `void setMode(bool mode);`
Set working mode (`DSP6951::AM` or `DSP6951::FM`)
Currently only FM is supported. AM support will be added soon.

### `void setMute(bool mute);`
Set mute or not. Set `true` to mute, `false` to unmute.

### `uint8_t getCNR();`
Get CNR(Carrier to noise ratio).

### `uint8_t getRSSI();`
Get RSSI(Received Signal Strength Indication).

### `bool isTuned();`
Get whether it is successfully tuned to the channel or not.

### `uint8_t getVolume();`
Get current volume value. (0-100)

### `float getChannel();`
Get current frequency.

### `bool getMode();`
Get current working mode. (`DSP6951::AM` or `DSP6951::FM`)

### `bool getMute();`
Get current mute state.

### `void tune();`
Tune to current frequency expressly.
It doesn't needed in usual.

## LICENSE
This library is published under MIT LICENSE.
See `LICENSE`.
