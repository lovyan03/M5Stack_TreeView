#include <MenuItemSPIFFS.h>
#include <SPIFFS.h>

MenuItemFS* MenuItemSPIFFS::newMenuItemFS(const String& title, const String& path, bool isdir) const
{
  return new MenuItemSPIFFS(title, path, isdir);
}

fs::FS& MenuItemSPIFFS::getFS() const {
  if (0 == path.length()) {
    SPIFFS.begin();
  }
  return SPIFFS;
}
