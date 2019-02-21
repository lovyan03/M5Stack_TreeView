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

  enum eCmd
  { NONE
  , BACK
  , NEXT
  , PREV
  , HOLD
  , ENTER
  };
  eCmd checkInput();
protected:
  eCmd checkKB(char key);
private:
  Rect16 _cursorRect;
  uint32_t _repeat;
  uint32_t _msecLast;
  uint32_t _msec = 0;
  bool _redraw;
};

#endif
