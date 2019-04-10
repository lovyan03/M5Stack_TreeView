#include <MenuItemToggle.h>
#include <Rect16.h>

void MenuItemToggle::onAfterDraw(){
  drawParts(value, this == focusItem);
}

void MenuItemToggle::setValue(bool value)
{
  this->value = value;
  if (callback) callback(this);
}

void MenuItemToggle::onEnter() {
  value = !value;
  drawParts(value, true);
}

void MenuItemToggle::drawParts(bool mode, int flg)
{
  int w = 32;
  Rect16 r ( rect.right() - (w + 8)
           , rect.y + 2
           , w
           , rect.h - 4);
  M5.Lcd.drawRect(r.x+1, r.y, r.w-2, r.h, frameColor[0]);
  M5.Lcd.drawRect(r.x, r.y+1, r.w, r.h-2, frameColor[0]);
  r.inflate(-2);

  w = 16;
  if (mode) {
    M5.Lcd.drawRect(r.right()-w, r.y, w    , r.h, fontColor[flg]);
    M5.Lcd.fillRect(r.x        , r.y, r.w-w, r.h, backgroundColor);
  } else {
    M5.Lcd.fillRect(r.x + w, r.y, r.w-w, r.h, backgroundColor);
  }
}

