#ifndef _MENUITEMSD_H_
#define _MENUITEMSD_H_

#include <MenuItemFS.h>

class MenuItemSD : public MenuItemFS
{
public:
  MenuItemSD(const String& title, std::function<void(MenuItem*)>cb = NULL)
  : MenuItemFS(title, cb) {};

  MenuItemSD(const String& title, const std::vector<MenuItem*> &items)
  : MenuItemFS(title, items) {};

  MenuItemSD(const String& title, const String& path, bool isdir)
  : MenuItemFS(title, path, isdir) {};


protected:
  virtual MenuItemFS* newMenuItemFS(const String& title, const String& path, bool isdir) const;
  virtual fs::FS& getFS() const;
};

#endif
