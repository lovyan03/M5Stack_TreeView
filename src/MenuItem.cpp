#include "MenuItem.h"
#ifdef min
#undef min
#endif
#include <algorithm>
#include <M5ButtonDrawer.h>

Rect16 MenuItem::clientRect(0,0,M5.Lcd.width(), M5.Lcd.height() - M5ButtonDrawer::height);

int8_t MenuItem::treeOffset = 10;
uint16_t MenuItem::itemHeight = 20;
uint16_t MenuItem::itemWidth = 240;
uint16_t MenuItem::frameColor[2] = { 0x0208, 0xFFFF };
uint16_t MenuItem::backColor[2]  = { 0x630C, 0x421F };
uint16_t MenuItem::fontColor[2]  = { 0xFFFF, 0xFFFF };
uint16_t MenuItem::backgroundColor = 0x0000;
uint32_t MenuItem::msecHold = 300;
uint32_t MenuItem::msecRepeat = 150;
bool MenuItem::useFACES = false;
bool MenuItem::useCardKB = false;
bool MenuItem::useJoyStick = false;
bool MenuItem::usePLUSEncoder = false;
bool MenuItem::useFACESEncoder = false;
bool MenuItem::useLowClockDelay = false;
bool MenuItem::swapBtnBC = false;

int16_t MenuItem::titleOffset = 14;
int16_t MenuItem::font = 1;
const GFXfont* MenuItem::gfxFont = NULL;

MenuItem* MenuItem::focusItem = NULL;
M5ButtonDrawer MenuItem::_btnDrawer;

MenuItem::MenuItem(const String& titleStr, int tg, TCallBackEnter cb)
: title(titleStr)
, callback(cb)
, tag(tg)
, destRect(0, 0, itemWidth, itemHeight)
, visible(false)
, _hiding(false)
, _moving(false)
, _parentItem(NULL)
{}

MenuItem::MenuItem(const String& titleStr, int tg, TCallBackEnter cb, const std::vector<MenuItem*> &sub) 
: MenuItem(titleStr, tg, cb)
{
  setItems(sub);
}

void MenuItem::addItem(MenuItem* mi)
{
  mi->_parentItem = this;
  Items.push_back(mi);
}

void MenuItem::addItems(const std::vector<MenuItem*> &sub)
{
  Items.reserve(Items.size() + sub.size());
  std::copy(sub.begin(), sub.end(),std::back_inserter(Items));
  for (std::vector<MenuItem*>::const_iterator it = sub.begin(); it != sub.end(); ++it) {
    (*it)->_parentItem = this;
  }
}

void MenuItem::setItems(const std::vector<MenuItem*> &sub)
{
  Items.assign(sub.begin(), sub.end());
  for (std::vector<MenuItem*>::iterator it = Items.begin(); it != Items.end(); ++it) {
    (*it)->_parentItem = this;
  }
}

void MenuItem::deleteItems() {
  for (uint16_t i = 0; i != Items.size(); ++i) {
    Items[i]->deleteItems();
    delete Items[i];
    Items[i] = 0;
  }
  Items.clear();
}

int16_t MenuItem::updateDest()
{
  return updateDestRect(destRect.x + treeOffset, destRect.bottom());
}

int16_t MenuItem::updateDestRect(int16_t x, int16_t y) {
  for (uint16_t i = 0; i != Items.size(); ++i) {
    if (!Items[i]->visible || Items[i]->_hiding) continue;
    Items[i]->destRect.x = x;
    Items[i]->destRect.y = y;
    Items[i]->destRect.h = itemHeight;
    Items[i]->destRect.w = std::min((int)itemWidth, (int)clientRect.right() - x);
    y += Items[i]->destRect.h;
    y = Items[i]->updateDestRect(x + treeOffset, y);
  }
  return y;
}

bool MenuItem::move(bool force)
{
  _oldRect = rect;
  if (rect.equal(destRect)) {
    if (_hiding) {
      visible = false;
      _hiding = false;
    } else {
      _moving = false;
    }
  } else {
    _moving = true;
    rect = force ? destRect : rect.mixRect(destRect);
  }
  bool sub = false;
  for (uint16_t i = 0; i != Items.size(); ++i) {
    sub = Items[i]->move(force) || sub;
  }
  return _moving || sub;
}

void MenuItem::drawTitle(bool selected)
{
  drawTitle(selected, title);
}

int16_t MenuItem::fontHeight()
{
  int16_t fh = M5.Lcd.fontHeight(font);
  if (gfxFont && 12 < fh) fh = fh * 9 / 10;
  return fh;
}

void MenuItem::applyFont()
{
  if (gfxFont) {
    M5.Lcd.setFreeFont(gfxFont);
  } else {
    M5.Lcd.setTextFont(0);
    M5.Lcd.setTextFont(font);
  }
}

void MenuItem::drawTitle(bool selected, const String& text)
{
  int16_t fh = fontHeight();
  M5.Lcd.setTextColor(fontColor[selected ? 1 : 0]);

  int lf = text.indexOf('\n');
  if (lf >= 0) {
    String sub = text.substring(0, lf);
    drawText(sub, rect.x + titleOffset, rect.y + rect.h / 2 - fh);
    sub = text.substring(lf + 1);
    drawText(sub, rect.x + titleOffset, rect.y + rect.h / 2);
  } else {
    drawText(text, rect.x + titleOffset, rect.y + (rect.h - fh) / 2);
  }
}

void MenuItem::drawText(String text, int16_t x, int16_t y)
{
  int rp = getRightPadding();
  while (text != "") {
    int w = M5.Lcd.textWidth(text);
    int over = (x + w + rp) - rect.right();
    if (0 < over) {
      text = text.substring(0, text.length() - 1 - over / 12);
    } else break;
  }
  M5.Lcd.drawString(text, rect.x + titleOffset, y);
}

void MenuItem::focusBack() {
  focusItem->onExit();
  setFocusItem(focusItem->parentItem());
  if (focusItem->parentItem()) {
    for (uint16_t i = 0; i != focusItem->Items.size(); ++i) {
      focusItem->Items[i]->destRect = focusItem->destRect;
      focusItem->Items[i]->_hiding = true;
    }
    updateDest();
    scrollTarget(focusItem->topItem()); // 一旦先頭に移動
    scrollSubitemArea(focusItem->parentItem()); // サブフォルダ末尾が入る位置まで移動
    scrollTarget(focusItem->parentItem()); // 親が映る位置まで移動
    scrollTarget(focusItem);    // 自身が映る位置まで移動
  }
}
void MenuItem::focusNext() {
  const MenuItem *parent = focusItem->parentItem();
  for (uint16_t i = 0; i != parent->Items.size(); ++i) {
    if (parent->Items[i] != focusItem) continue;
    setFocusItem(parent->Items[(i+1) % parent->Items.size()]);
    break;
  }
  scrollTarget(focusItem);
}
void MenuItem::focusPrev() {
  const MenuItem *parent = focusItem->parentItem();
  for (uint16_t i = 0; i != parent->Items.size(); ++i) {
    if (parent->Items[i] != focusItem) continue;
    setFocusItem(parent->Items[(i > 0) ? i - 1 : parent->Items.size() - 1]);
    break;
  }
  scrollTarget(focusItem);
}
bool MenuItem::focusEnter() {
  MenuItem* oldFI = focusItem;
  focusItem->onEnter();
  if (!oldFI->Items.empty())
  { // Expand the tree if the item has children.
    if (oldFI == focusItem)
    { // If focus item isn't changed by onEnter, change focus to the first child.
      MenuItem* mi = oldFI->Items[0];
      setFocusItem(mi);
    }
    updateDest();
    scrollSubitemArea(oldFI);
    scrollTarget(oldFI);
  } else 
  { // Execute callback function if the item has no children.
    MenuItem* mi = oldFI;
    // Find the parent that holds the callback.
    while (mi != this && !mi->callback) { mi = mi->parentItem(); }
    if (mi->callback) {
      mi->callback(oldFI);
#ifndef ARDUINO_ODROID_ESP32
      _btnDrawer.draw(true);
#endif
      return true;
    }
  }
  _btnDrawer.draw(true);
  return false;
}
void MenuItem::setFocusItem(MenuItem* newmi)
{
  if (newmi == focusItem) return;
  if (NULL != focusItem) focusItem->onDefocus();
  focusItem = newmi;
  focusItem->onFocus();
}

MenuItem* MenuItem::getFocusItem() {
  return focusItem;
}

void MenuItem::onAfterDraw()
{
  if (!Items.empty()) {
    int x = rect.x + 5;
    int y = rect.y + rect.h / 2 - 3;
    uint16_t c = fontColor[focusItem == this ? 1 : 0];
    if (Items[0]->visible) {
      M5.Lcd.drawLine( x  , y  , x+4, y  , c);
      M5.Lcd.drawLine( x+1, y+1, x+2, y+4, c);
      M5.Lcd.drawLine( x+3, y+1, x+2, y+4, c);
    } else {
      M5.Lcd.drawLine( x  , y  , x  , y+4, c);
      M5.Lcd.drawLine( x+1, y+1, x+4, y+2, c);
      M5.Lcd.drawLine( x+1, y+3, x+4, y+2, c);
    }
  }
}

MenuItem* MenuItem::draw(bool force, const Rect16* cursorRect, const Rect16* oldCursorRect, MenuItem* nextmi)
{
  if (!Items.empty()) {
    for (std::vector<MenuItem*>::reverse_iterator it = Items.rbegin(); it != Items.rend(); ++it) {
      if ((*it)->visible) {
        nextmi = (*it)->draw(force || (nextmi && nextmi->_moving), cursorRect, oldCursorRect, nextmi);
      }
    }
  }
  if ( force || _moving 
   || (nextmi && nextmi->_moving) 
   || (cursorRect && rect.intersectsWith(*cursorRect))
   || (oldCursorRect && rect.intersectsWith(*oldCursorRect))) {
     Rect16 rtmp = clientRect.intersect(rect);

    if (nextmi && nextmi->_parentItem != this && rtmp.bottom() > nextmi->rect.y) {
      rtmp.h -= rtmp.bottom() - nextmi->rect.y;
    }

    if (_parentItem && _parentItem->rect.intersectsWith(rect)) {
      if (0 < rtmp.h && _parentItem && _parentItem->rect.right() > rtmp.x) {
        Rect16 r = rtmp;
        r.w -= _parentItem->rect.right() - r.x;
        r.x = _parentItem->rect.right();
        if (r.w > 0)
          M5.Lcd.fillRect(r.x, r.y, r.w, r.h, backColor[0]);
      }

      if (_parentItem && _parentItem->rect.bottom() > rtmp.y) {
        rtmp.h -= _parentItem->rect.bottom() - rtmp.y;
        rtmp.y = _parentItem->rect.bottom();
      }
    }

    if (0 < rtmp.h && 0 < rtmp.w) {
     // draw frame line
      M5.Lcd.drawRect(rtmp.x, rtmp.y, rtmp.w, rtmp.h, frameColor[0]);
      rtmp.inflate(-1);
      if (0 < rtmp.h && 0 < rtmp.w) {
        bool cursor = cursorRect && cursorRect->intersectsWith(rtmp);
        // fill item body
       if (cursor) {
          Rect16 r = rtmp.intersect(*cursorRect);
          if (0 < r.h) {
            if (rtmp.y < r.y) {
              M5.Lcd.fillRect(rtmp.x, rtmp.y, rtmp.w, r.y - rtmp.y , backColor[0]);
            }
            if (rtmp.bottom() > r.bottom()) {
              M5.Lcd.fillRect(rtmp.x, r.bottom(), rtmp.w, rtmp.bottom() - r.bottom(), backColor[0]);
            }
            M5.Lcd.fillRect(rtmp.x, r.y, rtmp.w, r.h, backColor[1]); // fill cursor color
          }
        } else {
          M5.Lcd.fillRect(rtmp.x, rtmp.y, rtmp.w, rtmp.h, backColor[0]);
        }
        if (!_hiding && rect.y >= clientRect.y && rect.bottom() <= clientRect.bottom()) {
          applyFont();
          drawTitle(cursor);
          onAfterDraw();
        }
      }
    }
  }
  return this;
}
MenuItem* MenuItem::erase(bool force, MenuItem* nextmi)
{
  if (!Items.empty()) {
    for (std::vector<MenuItem*>::reverse_iterator it = Items.rbegin(); it != Items.rend(); ++it) {
      if ((*it)->visible) {
        nextmi = (*it)->erase(force, nextmi);
      }
    }
  }
  if (nextmi == NULL) {
    if (force || rect.y < _oldRect.y) {
      trimFillRect( force ? clientRect.x : rect.x
                  , rect.bottom()
                  , force ? clientRect.w : (_oldRect.right() - rect.x)
                  , (force ? clientRect.bottom() : _oldRect.bottom()) - rect.y
                  , backgroundColor);
    }
  }

  if (force) {
    trimFillRect( clientRect.x
                , rect.y
                , rect.x - clientRect.x
                , rect.h
                , backgroundColor);
    trimFillRect( rect.right()
                , rect.y
                , clientRect.right() - rect.right()
                , rect.h
                , backgroundColor);
  } else
  if (nextmi != NULL) {
    // left top erase
    if (nextmi->rect.x < rect.x && nextmi->_oldRect.y < nextmi->rect.y) {
      trimFillRect( nextmi->rect.x
                  , nextmi->_oldRect.y
                  , rect.x - nextmi->rect.x
                  , nextmi->rect.y - nextmi->_oldRect.y
                  , backgroundColor);
    }
    // right top erase
    if (rect.right() < nextmi->rect.right() && nextmi->_oldRect.y < nextmi->rect.y) {
      trimFillRect( rect.right()
                  , nextmi->_oldRect.y
                  , nextmi->rect.right() - rect.right()
                  , nextmi->rect.y - nextmi->_oldRect.y
                  , backgroundColor);
    }
    // right bottom erase
    if (nextmi->rect.right() < rect.right() && rect.bottom() < _oldRect.bottom()) {
      trimFillRect( nextmi->rect.right()
                  , rect.bottom()
                  , rect.right() - nextmi->rect.right()
                  , _oldRect.bottom() - rect.bottom()
                  , backgroundColor);
    }
    // right erase
    if (!Items.empty() && nextmi->rect.right() < nextmi->_oldRect.right()) {
      trimFillRect( nextmi->rect.right()
                  , nextmi->_oldRect.y
                  , nextmi->_oldRect.right() - nextmi->rect.right()
                  , Items.back()->_oldRect.bottom() - nextmi->_oldRect.y
                  , backgroundColor);
    }
    // left erase
    if (!Items.empty()) {
      if (nextmi->_oldRect.x < nextmi->rect.x) {
        trimFillRect( nextmi->_oldRect.x
                    , rect.bottom()
                    , nextmi->rect.x - nextmi->_oldRect.x
                    , Items.back()->_oldRect.bottom() - rect.bottom()
                    , backgroundColor);
      }
    // left bottom erase
      if (_oldRect.x < nextmi->rect.x && rect.bottom() < _oldRect.bottom()) {
        trimFillRect( _oldRect.x
                    , rect.bottom()
                    , nextmi->rect.x - _oldRect.x
                    , _oldRect.bottom() - rect.bottom()
                    , backgroundColor);
      }
    }
  }
  return this;
}

void MenuItem::onEnter() {
  if (!Items.empty()) {
    for (uint16_t i = 0; i != Items.size(); ++i) {
      Items[i]->visible = true;
      Items[i]->_hiding = false;
      Items[i]->rect = destRect;
    }
  }
}

void MenuItem::trimFillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
  int16_t r = std::min((int16_t)(x + w), clientRect.right());
  int16_t b = std::min((int16_t)(y + h), clientRect.bottom());
  x = std::max(clientRect.x, x);
  y = std::max(clientRect.y, y);
  if (x < r && y < b) M5.Lcd.fillRect(x, y, r - x, b - y, color);
}

bool MenuItem::compareIgnoreCase(MenuItem* lhs, MenuItem* rhs)
{
  String lstr ( lhs->title );
  String rstr ( rhs->title );
  lstr.toLowerCase();
  rstr.toLowerCase();
  return lstr < rstr;
}

void MenuItem::scrollDestRectY(int16_t add_y)
{
  destRect.y += add_y;
  for (uint16_t i = 0; i != Items.size(); ++i) {
    if (Items[i]->visible) Items[i]->scrollDestRectY(add_y);
  }
}

void MenuItem::scrollSubitemArea(MenuItem* mi) {
  if (mi->Items.empty()) return;
  int16_t bottom = clientRect.bottom();
  int16_t value = mi->Items.back()->destRect.h + mi->Items.back()->destRect.y;
  if (bottom < value) {
    scrollDestRectY(bottom - value);
  }
}

void MenuItem::scrollTarget(MenuItem* mi)
{
  int16_t bottom = clientRect.bottom();
  int16_t value = mi->destRect.h + mi->destRect.y;
  if (bottom < value) {
    scrollDestRectY(bottom - itemHeight / 2 - value);
  }
  if (mi->destRect.y < clientRect.y) {
    scrollDestRectY(-mi->destRect.y + clientRect.y );
  }
}

void MenuItem::updateButtons() {
  if (M5.BtnA.isPressed()) {
    _btnDrawer.setText(0, M5.BtnA.pressedFor(msecHold) ? "Prev" : "Back");
  } else {
    _btnDrawer.setText(0, "Back/Prev");
  }
  _btnDrawer.setText(swapBtnBC ? 1 : 2, "Next");
  _btnDrawer.setText(swapBtnBC ? 2 : 1, "Ok");
}
