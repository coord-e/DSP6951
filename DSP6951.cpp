#include "DSP6951.h"

#include <Wire.h>
#if defined(ARDUINO_SAM_DUE)
 #define _WIRE Wire1
#else
 #define _WIRE Wire
#endif

DSP6951::DSP6951(uint8_t addr) : _mode(FM), _mute(false), _volume(50), _amfreq(0), _fmfreq(0)
{
  _addr = addr >> 1;
}

void DSP6951::begin(void)
{
  _WIRE.begin();
  I2CWrite(0x00,0b11100000); //on,fm,tune,no-seek,no-seekup,no-mute,rsv,rsv
  I2CWrite(0x01,0b00010001); //FM2(76-108 MHz),00001 -MW, 520-1710KHz, 5KHz Stepとして設定
  I2CWrite(0x09,0b00001111);//Volume with I2C これを設定しないとI2Cによるボリュームコントロール出来ない。
}

void DSP6951::setVolume(uint8_t volume)
{
  _volume = volume;
  I2CWrite(0x06,(((int)((float)volume/100*39)+24)<<2) | 0b0000001);//Volume値
}

void DSP6951::setChannel(float freq)
{
  int channel;
  if(_mode){
    _fmfreq = freq;
    channel = (freq-30)/0.025;//データシートによるchannel計算値；以下、13ビットであるものとして扱う
  }else{
    _amfreq = freq;
    channel = freq/3;//データシートによるchannel計算値；以下、13ビットであるものとして扱う
  }
  int channel_upper_bits = (channel >> 8) | 0x40;// channelの冒頭5ビット分をとりだし、レジスタデフォルトの冒頭三ビット分をその左に付加して、レジスタ2にいれる。
  int channel_lower_bits = channel & 0xff;//channelの右八ビット分をとりだし、レジスタ３に入れる。
  I2CWrite(0x02,channel_upper_bits);
  I2CWrite(0x03,channel_lower_bits);
  tune();
}

void DSP6951::setMode(bool mode)
{
  _mode = mode;
  I2CWrite(0x00, 0b10000000 | _mute << 2 | _mode << 6);
  I2CWrite(0x01,0b00010001); //FM4(76-108 MHz),00001 -MW, 520-1710KHz, 5KHz Stepとして設定
}

void DSP6951::setMute(bool mute)
{
  _mute = mute;
  I2CWrite(0x00, 0b10000000 | _mute << 2 | _mode << 6);
}

void DSP6951::tune(){
  I2CWrite(0x00, 0b10100000 | _mute << 2 | _mode << 6);
  I2CWrite(0x00, 0b10000000 | _mute << 2 | _mode << 6);
}

uint8_t DSP6951::getVolume()
{
  return _volume;
}

float DSP6951::getChannel()
{
  if(_mode)
    return _fmfreq;
  else
    return _amfreq;
}

bool DSP6951::getMode()
{
  return _mode;
}

uint8_t DSP6951::getCNR()
{
  uint8_t res;
  if(_mode)
    res = I2CRead(0x17);
  else
    res = I2CRead(0x16);
  return res & 0b01111111;
}

uint8_t DSP6951::getRSSI()
{
  return I2CRead(0x1B) & 0b01111111;
}

bool DSP6951::isTuned()
{
  return (I2CRead(0x14) & 0b00100000) >> 5;
}

void DSP6951::I2CWrite(int reg, int val)
{
  _WIRE.beginTransmission(_addr);
  _WIRE.write(reg);
  _WIRE.write(val);
  _WIRE.endTransmission();
  delay(3);
}

uint8_t DSP6951::I2CRead(int reg)
{
  _WIRE.beginTransmission(_addr);
  _WIRE.write(reg);
  _WIRE.endTransmission();
  _WIRE.requestFrom((int)_addr, (int)8);
  return _WIRE.read();
}
