#ifndef _MENUITEMSD_H_
#define _MENUITEMSD_H_

#include <MenuItemFS.h>

class MenuItemSD : public MenuItemFS
{
public:
  MenuItemSD(const String& title, TCallBackEnter cb = NULL)
  : MenuItemFS(title, cb) {};

  MenuItemSD(const String& title, const std::vector<MenuItem*> &items)
  : MenuItemFS(title, items) {};

  MenuItemSD(const String& title, const String& path, bool isdir)
  : MenuItemFS(title, path, isdir) {};


protected:
  MenuItemFS* newMenuItemFS(const String& title, const String& path, bool isdir) const override;
  fs::FS& getFS() const override;
};

#endif
