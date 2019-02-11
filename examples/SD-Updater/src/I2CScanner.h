#ifndef _I2CSCANNER_H_
#define _I2CSCANNER_H_

#include <MenuCallBack.h>

class I2CScanner : public MenuCallBack
{
public:
  bool loop()
  {
    M5.Lcd.setTextSize(1);
    M5.Lcd.setTextColor(0xFFFF, 0);
    byte dummy = 0;
    for(byte adr = 0; adr <= 0x7F; adr++) {
      int x = (adr % 16) * 20;
      int y = (adr / 16) * 20;
      M5.Lcd.setCursor(4 + x, 6 + y);
      Wire.beginTransmission(adr);
      Wire.write(&dummy, 0);
      uint16_t color = (Wire.endTransmission() == 0) ? 0xFFFF : 0x39E7;
      M5.Lcd.setTextColor(color, 0);
      M5.Lcd.drawRect(x,y,19,19, color);
      M5.Lcd.printf("%02X", adr);
    }
    delay(10);

    return true;
  }
};

#endif
