#ifndef _MenuItemFS_H_
#define _MenuItemFS_H_

#include <MenuItem.h>
#include <FS.h>

class MenuItemFS : public MenuItem {
public:
  String path;
  bool isDir;

  MenuItemFS(const String& title, std::function<void(MenuItem*)>cb = NULL)
  : MenuItem(title, cb), path(), isDir(true) {};

  MenuItemFS(const String& title, const std::vector<MenuItem*> &items)
  : MenuItem(title, items), path(), isDir(true) {};
  
  MenuItemFS(const String& title, const String& p, bool isdir)
  : MenuItem(title), path(p), isDir(isdir) {};

  virtual void onEnter();
  virtual void onAfterDraw();
  virtual fs::FS& getFS() const {}
protected:
  virtual MenuItemFS* newMenuItemFS(const String& title, const String& path, bool isdir) const {}
};

#endif
