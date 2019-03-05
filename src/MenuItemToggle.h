#ifndef _MENUITEMTOGGLE_H_
#define _MENUITEMTOGGLE_H_

#include <MenuItem.h>

class MenuItemToggle : public MenuItem {
public:
  bool value = false;

  MenuItemToggle(const String& title, bool value = false, int tg = 0, TCallBackEnter cb = 0)
  : MenuItem(title, tg, cb), value(value) {};

  MenuItemToggle(const String& title, int value, TCallBackEnter cb)
  : MenuItem(title, cb), value(value) {};

  virtual void onAfterDraw();
  virtual void onEnter();
  void setValue(bool value);
private:
  void drawParts(bool value, int flg);
};

#endif
