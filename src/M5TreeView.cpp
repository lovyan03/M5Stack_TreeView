#include <M5TreeView.h>
#include <M5PLUSEncoder.h>
#include <M5FACESEncoder.h>
#include <M5JoyStick.h>
#undef min
#include <algorithm>

void M5TreeView::begin() {
  for (uint16_t i = 0; i != Items.size(); ++i) {
    Items[i]->visible = true;
  }
  destRect.w = 0; // root item hidden
  destRect.h = 0;
  destRect.y = clientRect.y;
  destRect.x = clientRect.x - treeOffset;
  updateDest();
  updateButtons();
  setFocusItem(Items[0]);
  update(true);
}

static char facesKey = 0xff;
static char facesPrev = 0xff;
static bool flgFACESKB = false;

M5TreeView::eCmd M5TreeView::checkKB(char key) {
  switch ((uint8_t)key) {
  case 'w': case 'W': case 0x80: case 0xB5:            return eCmd::PREV;
  case 'a': case 'A': case 0x81: case 0xB4: case 0x08: return eCmd::BACK;
  case 's': case 'S': case 0x82: case 0xB6:            return eCmd::NEXT;
  case 'd': case 'D': case 0x83: case 0xB7: case 0x20: return eCmd::ENTER;
  }
  return eCmd::NONE;
}

#ifdef ARDUINO_ODROID_ESP32
M5TreeView::eCmd M5TreeView::checkInput() {
  _msec = millis();
  M5.update();
  eCmd res = eCmd::NONE;
  bool press = M5.BtnA.isPressed()
            || M5.BtnB.isPressed()
            || M5.JOY_X.isPressed()
            || M5.JOY_Y.isPressed();
  bool canRepeat = _repeat == 0 || (_msec - _msecLast + _repeat) >= (1 < _repeat ? msecRepeat : msecHold);
  if (canRepeat) {
    if (M5.BtnA.isPressed())  { res = eCmd::HOLD;  }
    else if (M5.BtnA.wasReleased() || (M5.JOY_X.isAxisPressed() == DPAD_V_HALF))  { ++_repeat; res = eCmd::ENTER;  }
    else if (M5.BtnB.wasReleased() || (M5.JOY_X.isAxisPressed() == DPAD_V_FULL))  { ++_repeat; res = eCmd::BACK; }
    else if (M5.JOY_Y.isAxisPressed() == DPAD_V_HALF )  { ++_repeat; res = eCmd::NEXT;  }
    else if (M5.JOY_Y.isAxisPressed() == DPAD_V_FULL )  { ++_repeat; res = eCmd::PREV; }
  }

  if (!press) {
    _repeat = 0;
  }
  if (res != eCmd::NONE) {
    _msecLast = millis();
  }
  return res;
}
#else
M5TreeView::eCmd M5TreeView::checkInput() {
  bool btnALong = M5.BtnA.pressedFor(msecHold);
  _msec = millis();
  M5.update();
  eCmd res = eCmd::NONE;
  Button& btnB(swapBtnBC ? M5.BtnC : M5.BtnB);
  Button& btnC(swapBtnBC ? M5.BtnB : M5.BtnC);
  bool press = M5.BtnA.isPressed() || btnB.isPressed() || btnC.isPressed();
  bool canRepeat = _repeat == 0 || (_msec - _msecLast + _repeat) >= (1 < _repeat ? msecRepeat : msecHold);
  if (canRepeat) {
    if (btnB.isPressed())   { res = eCmd::HOLD;  }
    else if (M5.BtnA.wasReleased() && !btnALong) { res = eCmd::BACK; }
    else if (btnC.isPressed() )  { ++_repeat; res = eCmd::NEXT;  }
    else if (btnB.wasReleased()) { res = eCmd::ENTER; }
    else if (btnALong) { ++_repeat; res = eCmd::PREV; }
  }
  if (res == eCmd::NONE
   && useFACES && Wire.requestFrom(0x08, 1)) {
    while (Wire.available()){
      facesPrev = facesKey;
      facesKey = Wire.read();
      if (facesKey == 0) { flgFACESKB = true; }
      else {
        if (facesKey == 0xff)   { flgFACESKB = false; }
        else press = true;
        if (flgFACESKB) {
          res = checkKB(facesKey);
        } else {
          if (facesKey != 0xff && canRepeat) {
            if (     0 == (facesKey & 0x01) ) { res = eCmd::PREV; ++_repeat; }
            else if (0 == (facesKey & 0x02) ) { res = eCmd::NEXT; ++_repeat; }
          }
          if (0 == (facesKey & 0x08) || 0 == (facesKey & 0x10)) { res = eCmd::HOLD; }
          else if ((0 != (facesKey & 0x04))&&(0 == (facesPrev & 0x04))) { res = eCmd::BACK;  }
          else if ((0 != (facesKey & 0x20))&&(0 == (facesPrev & 0x20))) { res = eCmd::BACK;  }
          else if ((0 != (facesKey & 0x08))&&(0 == (facesPrev & 0x08))) { res = eCmd::ENTER; }
          else if ((0 != (facesKey & 0x10))&&(0 == (facesPrev & 0x10))) { res = eCmd::ENTER; }
        }
      }
    }
  }
  if (res == eCmd::NONE
   && useCardKB && Wire.requestFrom(0x5F, 1)) {
    while (Wire.available()){
      char key = Wire.read();
      if (key == 0)  { continue; }
      press = true;
      res = checkKB(key);
    }
  }
  if (res == eCmd::NONE
   && usePLUSEncoder && PLUSEncoder.update()) {
    if (     PLUSEncoder.wasUp())       { res = eCmd::PREV;  }
    else if (PLUSEncoder.wasDown())     { res = eCmd::NEXT;  }
    else if (PLUSEncoder.wasHold())     { res = eCmd::BACK;  }
    else if (PLUSEncoder.wasClicked())  { res = eCmd::ENTER; }
  }
  if (res == eCmd::NONE
   && useFACESEncoder && FACESEncoder.update()) {
    if (     FACESEncoder.wasUp())       { res = eCmd::PREV;  }
    else if (FACESEncoder.wasDown())     { res = eCmd::NEXT;  }
    else if (FACESEncoder.wasHold())     { res = eCmd::BACK;  }
    else if (FACESEncoder.wasClicked())  { res = eCmd::ENTER; }
  }
  if (res == eCmd::NONE
   && useJoyStick && JoyStick.update()) {
    if (!JoyStick.isNeutral()) {
      press = true;
      if (canRepeat) {
        ++_repeat;
        if (JoyStick.isUp()    ) { res = eCmd::PREV;  }
        if (JoyStick.isDown()  ) { res = eCmd::NEXT;  }
        if (JoyStick.wasLeft() ) { res = eCmd::BACK;  }
        if (JoyStick.wasRight()) { res = eCmd::ENTER; }
      }
    }
    if (JoyStick.wasClicked()) { res = eCmd::ENTER; }
    if (JoyStick.wasHold())    { res = eCmd::BACK; }
  }

  if (!press) {
    _repeat = 0;
  }
  if (res != eCmd::NONE) {
    _msecLast = millis();
  }
  return res;
}
#endif

MenuItem* M5TreeView::update(bool redraw) {
  if (millis() - _msec < 16) {
    uint32_t freq = getCpuFrequencyMhz();
    if (useLowClockDelay) setCpuFrequencyMhz(getXtalFrequencyMhz() / 4);
    delay(16 - (millis() - _msec));
    if (useLowClockDelay) setCpuFrequencyMhz(freq);
  }

  redraw |= _redraw;
  _redraw = false;

  eCmd cmd = checkInput();
  if (cmd != eCmd::NONE || M5.BtnA.wasPressed() || M5.BtnA.wasReleased() || redraw) {
    updateButtons();
  }
#ifndef ARDUINO_ODROID_ESP32
  _btnDrawer.draw(redraw);
#endif

  bool moveFocus = (!_cursorRect.equal(focusItem->destRect));
  if (move(cmd != eCmd::NONE) || moveFocus || redraw) {
    erase(false);
    if (moveFocus) {
      const Rect16& c = _cursorRect;
      Rect16 r = cmd != eCmd::NONE ? focusItem->destRect : c.mixRect(focusItem->destRect);
      draw(redraw, &r, &_cursorRect);
      _cursorRect = r;
    } else {
      draw(redraw, &_cursorRect);
    }
  }

  MenuItem* oldFocus = focusItem;
  MenuItem* res = NULL;
  switch (cmd) {
  case eCmd::NONE:  break;
  case eCmd::PREV:  focusPrev();   break;
  case eCmd::NEXT:  focusNext();   break;
  case eCmd::BACK:  focusBack();
    res = focusItem;
    if (res == this) focusItem = oldFocus;
    break;
  case eCmd::ENTER: case eCmd::HOLD:
    {
      Rect16 rtmp = focusItem->rect;
      rtmp = clientRect.intersect(rtmp);
      M5.Lcd.drawRect(rtmp.x, rtmp.y, rtmp.w, rtmp.h, frameColor[1]);
      if (cmd == eCmd::ENTER) {
        res = focusItem;
        _redraw = focusEnter();
        if (_redraw) {
          updateDest();
          erase(true);
          focusItem->onFocus();
        }
      }
    }
    break;
  }
  if (oldFocus != focusItem) {
    scrollTarget(focusItem);
  }
  if (focusItem) focusItem->onFocusLoop();

  return res;
}

