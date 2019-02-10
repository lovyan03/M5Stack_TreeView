#ifndef _M5TREEMENU_H_
#define _M5TREEMENU_H_

#include <MenuItem.h>
#include <MenuItemNumeric.h>

class M5TreeView : public MenuItem {
public:
  M5TreeView() : MenuItem() {};

  void begin();
  MenuItem* update(bool force = false);
  bool isRedraw() const { return _redraw; }
protected:
  virtual int16_t updateDestRect(MenuItem* mi, int16_t x = 0, int16_t y = 0);
  enum eCmd
  { NONE
  , BACK
  , NEXT
  , PREV
  , HOLD
  , ENTER
  };

  eCmd checkKB(char key);
  eCmd checkInput();
private:
  Rect16 _cursorRect;
  uint32_t _repeat;
  uint32_t _msecLast;
  uint32_t _msec = 0;
  bool _redraw;
};

#endif
