#include <M5TreeView.h>
#include <M5PLUSEncoder.h>
#include <M5JoyStick.h>
#undef min
#include <algorithm>

static bool active = false;
static bool btnALong = false;

int16_t M5TreeView::updateDestRect(MenuItem* mi, int16_t x, int16_t y) {
  for (uint16_t i = 0; i != mi->Items.size(); ++i) {
    if (!mi->Items[i]->visible || mi->Items[i]->_hiding) continue;
    mi->Items[i]->destRect.x = x;
    mi->Items[i]->destRect.y = y;
    mi->Items[i]->destRect.h = itemHeight;
    mi->Items[i]->destRect.w = std::min((int)itemWidth, (int)clientRect.right() - x);
    y += itemHeight;
    y = mi->Items[i]->updateDestRect(mi->Items[i], x + treeOffset, y);
  }
  return y;
}

void M5TreeView::begin() {
  focusItem = Items[0];
  for (uint16_t i = 0; i != Items.size(); ++i) {
    Items[i]->visible = true;
  }
  destRect.w = 0; // root item hidden
  destRect.h = 0;
  destRect.y = clientRect.y;
  destRect.x = clientRect.x - treeOffset;
  updateDest();
  updateButtons();
  update(true);
}

static char facesKey;
static char facesPrev;
static bool flgFACESKB;

M5TreeView::eCmd M5TreeView::checkKB(char key) {
  switch ((uint8_t)key) {
  case 'w': case 'W': case 0x80: case 0xB5:            return eCmd::PREV;
  case 'a': case 'A': case 0x81: case 0xB4: case 0x08: return eCmd::BACK;
  case 's': case 'S': case 0x82: case 0xB6:            return eCmd::NEXT;
  case 'd': case 'D': case 0x83: case 0xB7: case 0x20: return eCmd::ENTER;
  }
  return eCmd::NONE;
}

M5TreeView::eCmd M5TreeView::checkInput() {
  M5.update();
  eCmd res = eCmd::NONE;
  bool btnA = M5.BtnA.isPressed();
  Button& btn1(swapBtnBC ? M5.BtnB : M5.BtnC);
  Button& btn2(swapBtnBC ? M5.BtnC : M5.BtnB);
  bool press = btnA || btn1.isPressed() || btn2.isPressed();
  bool canRepeat = _repeat == 0 || (_msec - _msecLast) >= (1 < _repeat ? msecRepeat : msecHold);
  if (canRepeat) {
    if (btn2.isPressed())   { res = eCmd::HOLD;  }
    else if (M5.BtnA.wasReleased() && !btnALong) { res = eCmd::BACK; }
    else if (btn1.isPressed() )  { ++_repeat; res = eCmd::NEXT;  }
    else if (btn2.wasReleased()) { res = eCmd::ENTER; }
    else if (btnALong) { ++_repeat; res = eCmd::PREV; }
  }
  btnALong = M5.BtnA.pressedFor(msecHold);
  if (res != eCmd::NONE) return res;

  if (useFACES && Wire.requestFrom(0x08, 1)) {
    while (Wire.available()){
      facesPrev = facesKey;
      facesKey = Wire.read();
      if (facesKey == 0xff)   { flgFACESKB = false; }
      else if (facesKey == 0) { flgFACESKB = true; }
      else press = true;
      if (flgFACESKB) {
        res = checkKB(facesKey);
      } else {
        if (facesKey != 0xff && canRepeat) {
          ++_repeat;
          if (0 == (facesKey & 0x01) ) { res = eCmd::PREV; }
          else if (0 == (facesKey & 0x02) ) { res = eCmd::NEXT; }
        }
        if (0 == (facesKey & 0x08) ) { res = eCmd::HOLD; }
        else if ((0 != (facesKey & 0x04))&&(0 == (facesPrev & 0x04))) { res = eCmd::BACK;  }
        else if ((0 != (facesKey & 0x20))&&(0 == (facesPrev & 0x20))) { res = eCmd::BACK;  }
        else if ((0 != (facesKey & 0x08))&&(0 == (facesPrev & 0x08))) { res = eCmd::ENTER; }
        else if ((0 != (facesKey & 0x10))&&(0 == (facesPrev & 0x10))) { res = eCmd::ENTER; }
      }
    }
    if (res != eCmd::NONE) return res;
  }
  if (useCardKB && Wire.requestFrom(0x5F, 1)) {
    while (Wire.available()){
      char key = Wire.read();
      if (key == 0)  { continue; }
      press = true;
      res = checkKB(key);
    }
    if (res != eCmd::NONE) return res;
  }
  if (usePLUSEncoder && PLUSEncoder.update()) {
    if (PLUSEncoder.wasUp())       { return eCmd::PREV;  }
    if (PLUSEncoder.wasDown())     { return eCmd::NEXT;  }
    if (PLUSEncoder.wasHold())     { return eCmd::BACK;  }
    if (PLUSEncoder.wasClicked())  { return eCmd::ENTER; }
  }

  if (useJoyStick && JoyStick.update()) {
    if (!JoyStick.isNeutral()) {
      press = true;
      if (canRepeat) {
        ++_repeat;
        //if (bool repeat = !active && JoyStick.directionChangedFor(msecHold);
        if (JoyStick.isUp()    ) { return eCmd::PREV;  }
        if (JoyStick.isDown()  ) { return eCmd::NEXT;  }
        if (JoyStick.wasLeft() ) { return eCmd::BACK;  }
        if (JoyStick.wasRight()) { return eCmd::ENTER; }
      }
    }
    if (JoyStick.wasClicked()) { return eCmd::ENTER; }
    if (JoyStick.wasHold()) { return eCmd::BACK; }
  }

  if (!press) {
    _repeat = 0;
  }
  return res;
}

MenuItem* M5TreeView::update(bool redraw) {
  while (millis() - _msec < 16) delay(1);
  _msec = millis();

  redraw |= _redraw;
  _redraw = false;
  eCmd cmd = checkInput();

  if (cmd != eCmd::NONE) {
    _msecLast = _msec;
    active = true;
  }
  bool moveFocus = (!_cursorRect.equal(focusItem->destRect));
  active = move(cmd != eCmd::NONE) || moveFocus;
  if (active || redraw) {
    erase(redraw);
    if (moveFocus) {
      const Rect16& c = _cursorRect;
      Rect16 r = cmd != eCmd::NONE ? focusItem->destRect : c.mixRect(focusItem->destRect);
      draw(redraw, &r, &_cursorRect);
      _cursorRect = r;
      active = true;
    } else {
      draw(redraw, &_cursorRect);
    }
  }

  if (cmd != eCmd::NONE || M5.BtnA.wasPressed() || M5.BtnA.wasReleased()) {
    updateButtons();
  }
  _btnDrawer.draw(redraw);
  MenuItem* oldFocus = focusItem;
  MenuItem* res = NULL;
  switch (cmd) {
  case eCmd::PREV:  focusPrev();   break;
  case eCmd::NEXT:  focusNext();   break;
  case eCmd::BACK:  focusBack();
    res = focusItem;
    break;
  case eCmd::ENTER: case eCmd::HOLD:
    Rect16 rtmp = focusItem->rect;
    rtmp = clientRect.intersect(rtmp);
    M5.Lcd.drawRect(rtmp.x, rtmp.y, rtmp.w, rtmp.h, frameColor[1]);
    if (cmd == eCmd::ENTER) {
      res = focusItem;
      _redraw = focusEnter();
    }
    break;
  }
  if (oldFocus != focusItem) scrollTarget(focusItem);
  return res;
}

