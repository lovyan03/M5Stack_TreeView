#ifndef _MENUITEMSDUPDATER_H_
#define _MENUITEMSDUPDATER_H_

#include <MenuItem.h>

class MenuItemSDUpdater : public MenuItem {
  static uint16_t maxFileCount;
public:
  String filename;

  MenuItemSDUpdater(const String& titleStr, std::function<void(MenuItem*)>cb = NULL)
  : MenuItem(titleStr, cb), filename() {};

  MenuItemSDUpdater(const String& t, const String& p)
  : MenuItem(t), filename(p) {};

  virtual void onFocus();
  virtual void onDefocus();
  virtual void onEnter();
  virtual void onAfterDraw();
};

#endif
