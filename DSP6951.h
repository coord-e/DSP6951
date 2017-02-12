#ifndef _DSP6951_H
#define _DSP6951_H

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

class DSP6951{

public:

  DSP6951(uint8_t addr = 0x21);

  void     begin();
  void     setVolume(uint8_t volume);
  void     setChannel(float freq);
  void     setMode(bool mode);
  void     setMute(bool mute);
  uint8_t  getVolume();
  float    getChannel();
  bool     getMode();
  bool     getMute();
  void     tune();
  uint8_t  getCNR();
  uint8_t  getRSSI();
  bool  isTuned();

  static const bool FM = true;
  static const bool AM = false;

private:

  void I2CWrite(int reg, int val);
  uint8_t I2CRead(int reg);

  uint8_t _addr, _volume;
  float _fmfreq, _amfreq;
  bool _mode, _mute;
};
#endif // _DSP6951_H
