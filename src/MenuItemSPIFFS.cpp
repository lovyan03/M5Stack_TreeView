#include <MenuItemSPIFFS.h>
#include <SPIFFS.h>
#include <SD.h>

MenuItemFS* MenuItemSPIFFS::newMenuItemFS(const String& title, const String& path, bool isdir) const
{
  return new MenuItemSPIFFS(title, path, isdir);
}

void MenuItemSPIFFS::onEnter() {
  if (path.length()) {
    
    fs::FS spiffs(getFS());
    SD.begin();
    
    File srcFile = spiffs.open(path);
    File destFile = SD.open( path, FILE_WRITE );
    
    if( destFile ) {
      static uint8_t buf[512];
      size_t packets = 0;
      while( srcFile.read( buf, 512 ) ) {
        destFile.write(buf, 512);
        packets++;
      }
      destFile.close();
      //spiffs.remove( path );
      ESP.restart();
    }
  }
  MenuItemFS::onEnter();
}


fs::FS& MenuItemSPIFFS::getFS() const {
  if (0 == path.length()) SPIFFS.begin();
  return SPIFFS;
}
