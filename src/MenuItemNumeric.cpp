#include <MenuItemNumeric.h>
#include <M5PLUSEncoder.h>
#include <M5FACESEncoder.h>
#include <M5JoyStick.h>
#include <Rect16.h>

void MenuItemNumeric::onAfterDraw(){
  drawNum(value, 0);
}

void MenuItemNumeric::setValue(int value)
{
  if (minimum <= value && value <= maximum && this->value != value) {
    this->value = value;
    if (callback) callback(this);
  }
}

void MenuItemNumeric::onEnter() {
  draw();
  M5ButtonDrawer btnDrawer;
#ifndef ARDUINO_ODROID_ESP32
  btnDrawer.setText(0, "-");
  btnDrawer.setText(swapBtnBC ? 1 : 2, "+");
  btnDrawer.setText(swapBtnBC ? 2 : 1, "Ok");
  btnDrawer.draw(true);
  Button& btn1(swapBtnBC ? M5.BtnB : M5.BtnC);
  Button& btn2(swapBtnBC ? M5.BtnC : M5.BtnB);
#endif
  drawNum(value, 1);
  int pv = value;
  int repeat = 0;
  char facesPrev = 0xFF;
  char facesKey = 0xFF;
  bool flgFACESKB = false;
  for (;;) {
    M5.update();
#ifdef ARDUINO_ODROID_ESP32
    if (M5.BtnB.wasReleased()) break;
    if (M5.JOY_X.isAxisPressed() == DPAD_V_FULL) {
      while (M5.JOY_X.isPressed()) { M5.update(); }
      break;
    }
    if (M5.JOY_Y.isAxisPressed()) {
      ++repeat;
      if (M5.JOY_Y.isAxisPressed() == DPAD_V_FULL) {
        setValue(value + 1);
      } else {
        setValue(value - 1);
      }
    }
#else
    if (btn2.wasReleased()) break;
    if (M5.BtnA.wasPressed() || M5.BtnA.pressedFor(msecHold)) { ++repeat; setValue(value - 1); }
    if (   btn1.wasPressed() ||    btn1.pressedFor(msecHold)) { ++repeat; setValue(value + 1); }
    if (usePLUSEncoder && PLUSEncoder.update()) {
      if (PLUSEncoder.wasClicked() || PLUSEncoder.wasHold()) break;
      if (PLUSEncoder.wasUp()  ) { setValue(value + 1); }
      if (PLUSEncoder.wasDown()) { setValue(value - 1); }
    }
    if (useFACESEncoder && FACESEncoder.update()) {
      if (FACESEncoder.wasClicked() || FACESEncoder.wasHold()) break;
      if (FACESEncoder.wasUp()  ) { setValue(value + 1); }
      if (FACESEncoder.wasDown()) { setValue(value - 1); }
    }
    if (useJoyStick && JoyStick.update()) {
      if (JoyStick.isUp()   ) { ++repeat; setValue(value + 1); }
      if (JoyStick.isDown() ) { ++repeat; setValue(value - 1); }
      if (JoyStick.wasLeft() || JoyStick.wasClicked() || JoyStick.wasHold()) { break; }
    }
    if (useFACES && Wire.requestFrom(0x08, 1)) {
      if (!flgFACESKB) facesPrev = facesKey;
      while (Wire.available()) {
        facesKey = Wire.read();
      }
      if (facesKey == 0)    { flgFACESKB = true; }
      if (facesKey == 0xFF) { flgFACESKB = false;}

      if (!flgFACESKB) {
        if (0 == (facesKey & 0x01) && value < maximum) { ++repeat; setValue(value + 1); }
        if (0 == (facesKey & 0x02) && value > minimum) { ++repeat; setValue(value - 1); }
        if ((0 == (facesPrev & 0x04)) && (0 != (facesKey & 0x04))) break;
      }
    }
    btnDrawer.draw();
#endif
    if (pv != value) {
      drawNum(value, 1);
      pv = value;
      if (0 < repeat && repeat < 100) delay(100 - repeat);
    } else {
      repeat = 0;
      delay(10);
    }
  }
}

void MenuItemNumeric::drawNum(int value, int flg)
{
  applyFont();
  String str(value);
  int w = std::max(M5.Lcd.textWidth(str)+4, 32);
  Rect16 r ( rect.right() - (w + 8)
           , rect.y + 2
           , w
           , rect.h - 4);
  M5.Lcd.drawRect(r.x+1, r.y, r.w-2, r.h, frameColor[flg]);
  M5.Lcd.drawRect(r.x, r.y+1, r.w, r.h-2, frameColor[flg]);
  r.inflate(-2);

  M5.Lcd.setTextColor(fontColor[flg]);
  M5.Lcd.fillRect(r.x, r.y, r.w, r.h, backColor[flg]);
  M5.Lcd.drawRightString(str,r.right(), rect.y + (rect.h - fontHeight()) / 2, font);
}

