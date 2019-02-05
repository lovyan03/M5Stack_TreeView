#ifndef _SCROLLDEMO_H_
#define _SCROLLDEMO_H_

#include <MenuCallBack.h>

class ScrollDemo : public MenuCallBack
{
  const uint16_t colorWay = 0x8410;
  const uint16_t hHeader = 16;
  const uint16_t hFooter = 16;
  const uint16_t hArea = TFT_WIDTH - hHeader - hFooter;
  uint16_t way[TFT_WIDTH];
  int f;
  float x,y;
  float angle;
  float sx;
  float cy;
  int wy;
  int wx;
  int tx;
  int ww;

public:
  ScrollDemo() {}
  bool setup()
  {
    f = 0;
    x = 160;
    y = 200;
    angle = 0;
    wy = 0;
    wx = 100;
    tx = 100;
    ww = 120;
    for (int i = 0; i < hArea; ++i) {
      drawWay(hHeader+i);
      way[i] = wx;
    }
    btnDrawer.setText(1, "<<");
    btnDrawer.setText(2, ">>");
    return true;
  }
  void close()
  {
    scrollAddress(0);
  }
  bool loop()
  {
    M5.update();
delay(7);
    M5.Lcd.setTextColor(0xFFFF, 0);
    uint16_t xWay = way[((int)y + hArea - hHeader) % hArea];
    if ((uint16_t)x < xWay || (uint16_t)x > xWay+ww) {
      M5.Lcd.setCursor(120,y);
      M5.Lcd.print(" !! Crash !! ");
      return !M5.BtnA.wasReleased();
    }

    drawCar(x, y, colorWay);
    if (y < hHeader) {
      drawCar(x, y + hArea, colorWay);
    }
    if (M5.BtnB.isPressed()) angle+=0.02;
    if (M5.BtnC.isPressed()) angle-=0.02;
    sx = sin(angle)*1.1;
    cy = cos(angle)*1.1;
    x -= sx;
    y -= cy;
    drawCar(x, y, 0xffff);
    if (y < hHeader) {
      drawCar(x, y + hArea, 0xffff);
      M5.Lcd.fillRect(x-10,0,30,hHeader,0);
      M5.Lcd.fillRect(x-10,hHeader+hArea,30,hFooter,0);
    }
    if (y+10 < hHeader) y+=hArea;

    M5.Lcd.setCursor(10,2);
    ++f;
    M5.Lcd.printf("score : %d", f);

    wy = (wy+hArea-1) % hArea;
    if (0 == wy) --ww;
    int hy = hHeader + wy;
    setupScrollArea(hHeader, hFooter);
    scrollAddress(hy);
    drawWay(hy);
    way[wy] = wx;
    if ( wx == tx ) {
      tx = (millis() % (TFT_HEIGHT - ww -20)) + 10;
    }
    wx += (tx < wx ? -1 : 1);
    return !M5.BtnA.wasReleased();
  }
  void drawWay(int hy)
  {
    int tmp;
    for (int i = 0; i < wx;i+=tmp) {
      tmp = 10+ (wx+micros()) % 20;
      M5.Lcd.drawFastHLine( i, hy, tmp, (((i + hy + micros()) % 32) << 5)|0x2404);
    }
    for (int i = wx+ww; i < 320;i+=tmp) {
      tmp = 10+ (wx+micros()) % 20;
      M5.Lcd.drawFastHLine( i, hy, tmp, (((i + hy + micros()) % 32) << 5)|0x2404);
    }
    M5.Lcd.drawFastHLine(wx-10, hy, ww+20, ((hy-hHeader)*20/hArea%2) ? 0xffff:0xf800);
    M5.Lcd.drawFastHLine(wx, hy, ww, colorWay);
  }

  void drawCar(float x, float y, uint16_t color)
  {
    M5.Lcd.drawLine( x, y, x + sx*8 + cy*2, y + cy*8 - sx*2, color);
    M5.Lcd.drawLine( x, y, x + sx*8 - cy*2, y + cy*8 + sx*2, color);
    M5.Lcd.drawLine( x + sx*8 + cy*2, y + cy*8 - sx*2
                   , x + sx*8 - cy*2, y + cy*8 + sx*2, color);
  }
  void setupScrollArea(uint16_t tfa, uint16_t bfa) {
    M5.Lcd.writecommand(ILI9341_VSCRDEF); // Vertical scroll definition
    M5.Lcd.writedata(tfa >> 8);           // Top Fixed Area line count
    M5.Lcd.writedata(tfa);
    M5.Lcd.writedata((TFT_WIDTH-tfa-bfa)>>8);  // Vertical Scrolling Area line count
    M5.Lcd.writedata(TFT_WIDTH-tfa-bfa);
    M5.Lcd.writedata(bfa >> 8);           // Bottom Fixed Area line count
    M5.Lcd.writedata(bfa);
  }

  void scrollAddress(uint16_t vsp) {
    M5.Lcd.writecommand(ILI9341_VSCRSADD); // Vertical scrolling pointer
    M5.Lcd.writedata(vsp>>8);
    M5.Lcd.writedata(vsp);
  }

};

#endif
