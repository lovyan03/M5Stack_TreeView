#ifndef _MENUITEMNUMERIC_H_
#define _MENUITEMNUMERIC_H_

#include <MenuItem.h>

class MenuItemNumeric : public MenuItem {
public:
  int minimum = 0;
  int maximum = 1;
  int value = 0;

  MenuItemNumeric(const String& title, int min, int max, int value, int tg = 0, TCallBackEnter cb = 0)
  : MenuItem(title, tg, cb), minimum(min), maximum(max), value(value) {};

  MenuItemNumeric(const String& title, int min, int max, int value, TCallBackEnter cb)
  : MenuItem(title, cb), minimum(min), maximum(max), value(value) {};

  virtual void onAfterDraw();
  virtual void onEnter();
  void setValue(int value);
private:
  void drawNum(int value, int flg);
};

#endif
