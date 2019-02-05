#include "MenuItemSDUpdater.h"

#include <M5StackUpdater.h>   // https://github.com/tobozo/M5Stack-SD-Updater
#include <SD.h>

static SDUpdater sdUpdater;

void MenuItemSDUpdater::onEnter() {
  if (!filename.length()) {

    // SDのルートフォルダから *.bin ファイルを探す。
    deleteItems();
    SD.begin();
    File root = SD.open("/");
    File file = root.openNextFile();
    MenuItemSDUpdater* mi;
    while (file) {
      if (!file.isDirectory()) {
        String name = file.name();
        int idx = name.lastIndexOf('.');
        String ext = name.substring(idx + 1);
        if (ext == "bin") {
          name = name.substring(1, idx);
          mi = new MenuItemSDUpdater(name, name);
          addItem(mi);
        }
      }
      file = root.openNextFile();
    }
    root.close();
  } else {
    // 選択されたbinファイルでupdateを実行する。
    String bin = "/" + filename + ".bin";
    sdUpdater.updateFromFS(SD, bin);
    ESP.restart();
  }

  MenuItem::onEnter();
}

void MenuItemSDUpdater::onFocus() {
  String jpg = "/jpg/" + filename + ".jpg";
  M5.Lcd.setTextColor(0xFFFF,0);
  M5.Lcd.drawJpgFile(SD, jpg.c_str(), 200, 40);
}

void MenuItemSDUpdater::onDefocus() {
  M5.Lcd.fillRect(200, 30, 120, 140, backgroundColor);
}

void MenuItemSDUpdater::onAfterDraw()
{
}
