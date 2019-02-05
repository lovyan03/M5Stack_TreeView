#include <M5TreeView.h>
#include <M5PLUSEncoder.h>
#include <M5JoyStick.h>

static bool active = false;
static bool btnALong = false;

int16_t M5TreeView::updateDestRect(MenuItem* mi, int16_t x, int16_t y) {
  for (uint16_t i = 0; i != mi->Items.size(); ++i) {
    if (!mi->Items[i]->visible || mi->Items[i]->_hiding) continue;
    mi->Items[i]->destRect.x = x;
    mi->Items[i]->destRect.y = y;
    mi->Items[i]->destRect.h = itemHeight;
    mi->Items[i]->destRect.w = itemWidth;
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

M5TreeView::eCmd M5TreeView::checkInput() {
  M5.update();
  eCmd res = eCmd::NONE;
  bool btnA = M5.BtnA.isPressed();
  bool btnB = M5.BtnB.isPressed();
  bool btnC = M5.BtnC.isPressed();
  bool press = btnA || btnB || btnC;
  bool canRepeat = _repeat == 0 || (_msec - _msecLast) >= (1 < _repeat ? msecRepeat : msecHold);
  if (canRepeat) {
    if (btnC)   { res = eCmd::HOLD;  }
    else if (M5.BtnA.wasReleased() && !btnALong) { res = eCmd::BACK; }
    else if (btnB                )  { ++_repeat; res = eCmd::NEXT;  }
    else if (M5.BtnC.wasReleased()) { res = eCmd::ENTER; }
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
      if (!flgFACESKB) {
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

MenuItem* M5TreeView::update(bool force) {
  if (millis() - _msec < 16) return NULL;
  _msec = millis();

  eCmd cmd = checkInput();

  if (force) M5.Lcd.fillRect(clientRect.x, clientRect.y, clientRect.w, clientRect.h, backgroundColor);

  if (cmd != eCmd::NONE)
  {
    _msecLast = _msec;
    if (move(true) || !_cursorRect.equal(focusItem->destRect)) {
      draw(NULL, true, &focusItem->destRect, &_cursorRect);
      _cursorRect = focusItem->destRect;
      //move();
    }
    active = true;
  }

  if (cmd != eCmd::NONE || M5.BtnA.wasPressed() || M5.BtnA.wasReleased()) {
    updateButtons();
    _btnDrawer.draw(true);
  } else {
    _btnDrawer.draw(force);
  }

  MenuItem* res = NULL;
  switch (cmd) {
  case eCmd::PREV:  focusPrev();   break;
  case eCmd::NEXT:  focusNext();   break;
  case eCmd::BACK:  focusBack();   res = focusItem; break;
  case eCmd::ENTER: res = focusItem; force = focusEnter(); break;
  }
  if (cmd == eCmd::HOLD) {
    Rect16 rtmp = focusItem->rect;
    rtmp = clientRect.intersect(rtmp);
    M5.Lcd.drawRect(rtmp.x, rtmp.y, rtmp.w, rtmp.h, frameColor[1]);
  } else
  if (active || force) {
    scrollTarget(focusItem);

    active = move();
    if (!_cursorRect.equal(focusItem->destRect)) { // カーソル移動
      const Rect16& c = _cursorRect;
      Rect16 r = c.mixRect(focusItem->destRect);
      draw(NULL, force, &r, &_cursorRect);
      _cursorRect = r;
      active = true;
    } else {
      draw(NULL, force, &_cursorRect);
    }
  }
  return res;
}

