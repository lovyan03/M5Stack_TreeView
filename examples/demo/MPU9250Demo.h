#ifndef _MPU9250DEMO_H_
#define _MPU9250DEMO_H_

#include <MenuCallBack.h>
#undef min
#include <algorithm>

#include "utility/MPU9250.h"


class MPU9250Demo : public MenuCallBack
{
  MPU9250 IMU;
  uint16_t _x;
  int _style;
public:
  MPU9250Demo() : _x(0), _style(0) {}

  bool setup()
  {
    btnDrawer.setText(2, "Style");
    M5.Lcd.setTextColor(0xffff, 0);
    if (0x71 != IMU.readByte(MPU9250_ADDRESS, WHO_AM_I_MPU9250)) {
      M5.Lcd.println("MPU9250 not connected...");
      delay(2000);
      return false;
    }
    IMU.initMPU9250();
    if ( 0x48 != IMU.readByte(AK8963_ADDRESS, WHO_AM_I_AK8963)) {
      M5.Lcd.println("AK8963 not connected...");
      delay(2000);
      return false;
    }

    IMU.initAK8963(IMU.magCalibration);

    _x = 0;
    return true;
  }

  bool loop()
  {
    if (IMU.readByte(MPU9250_ADDRESS, INT_STATUS) & 0x01)
    {
      IMU.readAccelData(IMU.accelCount);
      IMU.getAres();
      IMU.ax = (float)IMU.accelCount[0]*IMU.aRes;
      IMU.ay = (float)IMU.accelCount[1]*IMU.aRes;
      IMU.az = (float)IMU.accelCount[2]*IMU.aRes;

      IMU.readGyroData(IMU.gyroCount);
      IMU.getGres();
      IMU.gx = (float)IMU.gyroCount[0]*IMU.gRes;
      IMU.gy = (float)IMU.gyroCount[1]*IMU.gRes;
      IMU.gz = (float)IMU.gyroCount[2]*IMU.gRes;

      IMU.readMagData(IMU.magCount);
      IMU.getMres();
      IMU.mx = (float)IMU.magCount[0]*IMU.mRes;
      IMU.my = (float)IMU.magCount[1]*IMU.mRes;
      IMU.mz = (float)IMU.magCount[2]*IMU.mRes;
    
      if (M5.BtnC.wasPressed()) {
        _style = (1+_style) % 2;
        M5.Lcd.fillScreen(0);
        _x = 0;
      }
      switch (_style) {
        default: style0(); break;
        case 1:  style1(); break;
      }
    }

    return true;
  }

  void gyromagSleep()
  {
    uint8_t temp = 0;
    temp = IMU.readByte(AK8963_ADDRESS, AK8963_CNTL);
    IMU.writeByte(AK8963_ADDRESS, AK8963_CNTL, temp & ~(0x0F) ); // Clear bits 0 - 3 to power down magnetometer
    temp = IMU.readByte(MPU9250_ADDRESS, PWR_MGMT_1);
    IMU.writeByte(MPU9250_ADDRESS, PWR_MGMT_1, temp | 0x10);     // Write bit 4 to enable gyro standby
    delay(10); // Wait for all registers to reset
  }
  void MPU9250sleep()
  {
    // Put the device to sleep
    uint8_t c = IMU.readByte(MPU9250_ADDRESS, PWR_MGMT_1);
    IMU.writeByte(MPU9250_ADDRESS, PWR_MGMT_1, c | 0x40); // Set sleep mode bit(6), disable all sensors
    delay(100); // Wait for all registers to reset
  }

  void close() 
  {
    M5.Lcd.drawString("closing", 100, 100, 1);
    MPU9250sleep();
    gyromagSleep();
  }

private:
  bool style0()
  {
    M5.Lcd.drawLine(_x, 0, _x, TFT_WIDTH, 0);

    M5.Lcd.setTextSize(1);
    M5.Lcd.setCursor( 10, 5);
    M5.Lcd.setTextColor(0xffff,0);    M5.Lcd.print("accel (mg) ");

    M5.Lcd.setTextColor(0xf800,0);    M5.Lcd.printf("  x:%+6d", (int)(1000 * IMU.ax));
    M5.Lcd.setTextColor(0x07e0,0);    M5.Lcd.printf("  y:%+6d", (int)(1000 * IMU.ay));
    M5.Lcd.setTextColor(0x001f,0);    M5.Lcd.printf("  z:%+6d", (int)(1000 * IMU.az));

    M5.Lcd.setCursor( 10, 80);
    M5.Lcd.setTextColor(0xffff,0);    M5.Lcd.print("gyro (o/s) ");
    M5.Lcd.setTextColor(0xf800,0);    M5.Lcd.printf("  x:%+6d", (int)(IMU.gx));
    M5.Lcd.setTextColor(0x07e0,0);    M5.Lcd.printf("  y:%+6d", (int)(IMU.gy));
    M5.Lcd.setTextColor(0x001f,0);    M5.Lcd.printf("  z:%+6d", (int)(IMU.gz));

    M5.Lcd.setCursor( 10,160);
    M5.Lcd.setTextColor(0xffff,0);    M5.Lcd.print("  mag (mG) ");
    M5.Lcd.setTextColor(0xf800,0);    M5.Lcd.printf("  x:%+6d", (int)(IMU.mx/2));
    M5.Lcd.setTextColor(0x07e0,0);    M5.Lcd.printf("  y:%+6d", (int)(IMU.my/2));
    M5.Lcd.setTextColor(0x001f,0);    M5.Lcd.printf("  z:%+6d", (int)(IMU.mz/2));

    plotpoint(_x, 40, IMU.ax * 20, IMU.ay * 20, IMU.az * 20);
    plotpoint(_x,120, IMU.gx / 10, IMU.gy / 10, IMU.gz / 10);
    plotpoint(_x,200, IMU.mx /300, IMU.my /300, IMU.mz /300);

    _x = (1+_x) % TFT_HEIGHT;
    M5.Lcd.drawLine(_x+1, 0, _x+1, TFT_WIDTH, 0xffff);

    return true;
  }

  void plotpoint(uint16_t x, uint16_t y, int r, int g, int b)
  {
    if (r == g && g == b) {
      M5.Lcd.drawPixel(x, y + r, 0xFFFF );
    } else if (r == g) {
      M5.Lcd.drawPixel(x, y + r, 0xFFE0 );
      M5.Lcd.drawPixel(x, y + b, 0x001F);
    } else if (r == b) {
      M5.Lcd.drawPixel(x, y + r, 0xF81F);
      M5.Lcd.drawPixel(x, y + g, 0x07E0);
    } else if (g == b) {
      M5.Lcd.drawPixel(x, y + r, 0xF800);
      M5.Lcd.drawPixel(x, y + g, 0x07FF);
    } else {
      M5.Lcd.drawPixel(x, y + r, 0xF800);
      M5.Lcd.drawPixel(x, y + g, 0x07E0);
      M5.Lcd.drawPixel(x, y + b, 0x001F);
    }
  }

  bool style1()
  {
    M5.Lcd.setTextSize(1);
    M5.Lcd.setCursor( 0,  0); M5.Lcd.setTextColor(0xffff,0);    M5.Lcd.print("accel (mg) ");
    M5.Lcd.setCursor(10, 10); M5.Lcd.setTextColor(0xf800,0);    M5.Lcd.printf("  x:%+6d", (int)(1000 * IMU.ax));
    M5.Lcd.setCursor(10, 30); M5.Lcd.setTextColor(0x07e0,0);    M5.Lcd.printf("  y:%+6d", (int)(1000 * IMU.ay));
    M5.Lcd.setCursor(10, 50); M5.Lcd.setTextColor(0x001f,0);    M5.Lcd.printf("  z:%+6d", (int)(1000 * IMU.az));

    M5.Lcd.setCursor( 0, 70); M5.Lcd.setTextColor(0xffff,0);    M5.Lcd.print("gyro (o/s) ");
    M5.Lcd.setCursor(10, 80); M5.Lcd.setTextColor(0xf800,0);    M5.Lcd.printf("  x:%+6d", (int)(IMU.gx));
    M5.Lcd.setCursor(10,100); M5.Lcd.setTextColor(0x07e0,0);    M5.Lcd.printf("  y:%+6d", (int)(IMU.gy));
    M5.Lcd.setCursor(10,120); M5.Lcd.setTextColor(0x001f,0);    M5.Lcd.printf("  z:%+6d", (int)(IMU.gz));

    M5.Lcd.setCursor( 0,140); M5.Lcd.setTextColor(0xffff,0);    M5.Lcd.print("  mag (mG) ");
    M5.Lcd.setCursor(10,150); M5.Lcd.setTextColor(0xf800,0);    M5.Lcd.printf("  x:%+6d", (int)(IMU.mx/2));
    M5.Lcd.setCursor(10,170); M5.Lcd.setTextColor(0x07e0,0);    M5.Lcd.printf("  y:%+6d", (int)(IMU.my/2));
    M5.Lcd.setCursor(10,190); M5.Lcd.setTextColor(0x001f,0);    M5.Lcd.printf("  z:%+6d", (int)(IMU.mz/2));

    drawBar(IMU.ax * 100, 20, 0xf800);
    drawBar(IMU.ay * 100, 40, 0x07e0);
    drawBar(IMU.az * 100, 60, 0x001f);
    drawBar(IMU.gx /   2, 90, 0xf800);
    drawBar(IMU.gy /   2,110, 0x07e0);
    drawBar(IMU.gz /   2,130, 0x001f);
    drawBar(IMU.mx /  50,160, 0xf800);
    drawBar(IMU.my /  50,180, 0x07e0);
    drawBar(IMU.mz /  50,200, 0x001f);
    return true;
  }
  void drawBar(int value, uint16_t y, uint16_t color) {
    value = std::max(-160, std::min(160, value));
    int x = 160 + (value < 0 ? value : 0);
    int w = abs(value);
    M5.Lcd.fillRect(0  , y, x, 7, 0);
    M5.Lcd.fillRect(x+w, y,320-(x+w),7,0);
    M5.Lcd.fillRect(x  , y, w, 7, color);
  }
};

#endif
