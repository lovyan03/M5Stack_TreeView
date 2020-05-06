#include <MenuItemSD.h>
#include <SD.h>

MenuItemFS* MenuItemSD::newMenuItemFS(const String& title, const String& path, bool isdir) const
{
  return new MenuItemSD(title, path, isdir);
}

fs::FS& MenuItemSD::getFS() const {
  if (0 == path.length()) {
    SD.end();
    SD.begin( TFCARD_CS_PIN, SPI, 20000000);
  }
  return SD;
}
