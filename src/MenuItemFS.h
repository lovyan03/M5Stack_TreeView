#ifndef _MENUITEMFS_H_
#define _MENUITEMFS_H_

#include <MenuItem.h>
#include <FS.h>

class MenuItemFS : public MenuItem {
public:
  String path;
  bool isDir;

  MenuItemFS(const String& title, TCallBackEnter cb = NULL)
  : MenuItem(title, cb), path(), isDir(true) {};

  MenuItemFS(const String& title, const std::vector<MenuItem*> &items)
  : MenuItem(title, items), path(), isDir(true) {};
  
  MenuItemFS(const String& title, const String& p, bool isdir)
  : MenuItem(title), path(p), isDir(isdir) {};

  virtual void onEnter();
  virtual void onAfterDraw();
  virtual fs::FS& getFS() const = 0;
protected:
  virtual MenuItemFS* newMenuItemFS(const String& title, const String& path, bool isdir) const = 0;
};

#endif
