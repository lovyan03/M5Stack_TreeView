#ifndef _MENUITEMSPIFFS_H_
#define _MENUITEMSPIFFS_H_

#include <MenuItemFS.h>

class MenuItemSPIFFS : public MenuItemFS
{
public:
  MenuItemSPIFFS(const String& title, TCallBackEnter cb = NULL)
  : MenuItemFS(title, cb) {};

  MenuItemSPIFFS(const String& title, const std::vector<MenuItem*> &items)
  : MenuItemFS(title, items) {};

  MenuItemSPIFFS(const String& title, const String& path, bool isdir)
  : MenuItemFS(title, path, isdir) {};


protected:
  MenuItemFS* newMenuItemFS(const String& title, const String& path, bool isdir) const override;
  fs::FS& getFS() const override;
};

#endif
