#include <MenuItemFS.h>
#include <Rect16.h>
#undef min
#include <algorithm>

void MenuItemFS::onEnter() {
  deleteItems();

  fs::FS fs(getFS());

  std::vector<MenuItem*> filesItems;
  File root = fs.open(path.length() ? path : "/");
  File file = root.openNextFile();
  MenuItemFS* mi;
  while (file) {
    String ptmp = file.name();
    mi = newMenuItemFS(ptmp.substring(path.length()), ptmp, file.isDirectory());
    // ディレクトリは先に追加する
    if (file.isDirectory()) addItem(mi);
    else filesItems.push_back(mi);
    file = root.openNextFile();
  }
  root.close();
  std::sort(Items.begin(), Items.end(), compareIgnoreCase);
  if (!filesItems.empty()) {
    // ファイルがあればまとめて追加する
    std::sort(filesItems.begin(), filesItems.end(), compareIgnoreCase);
    addItems(filesItems);
  }
  MenuItem::onEnter();
}

void MenuItemFS::onAfterDraw() {
  if (path.length() == 0) return;
  Rect16 r ( rect.x + 4
            , rect.y + (rect.h - 8) / 2
            , 8
            , 8);
  if (isDir) {
    M5.Lcd.fillRect(r.x, r.y+1, r.w  , r.h-1, 0x0ff0);
    M5.Lcd.drawFastHLine(r.x  , r.y  , 3, 0x0ff0);
    M5.Lcd.drawFastHLine(r.x  , r.y+2, 4, 0x0410);
    M5.Lcd.drawFastHLine(r.x+4, r.y+1, 4, 0x0410);
  } else {
    M5.Lcd.fillRect(r.x, r.y+1, r.w, r.h - 1, 0xFFFF);
    M5.Lcd.drawFastHLine(r.x, r.y  , r.w - 1, 0xFFFF);
    M5.Lcd.drawFastVLine(r.x+5, r.y  , 2, 0);
    M5.Lcd.drawFastHLine(r.x+5, r.y+2, 3, 0);
    M5.Lcd.drawFastHLine(r.x+1, r.y+2, 3, 0);
    M5.Lcd.drawFastHLine(r.x+1, r.y+4, 6, 0);
    M5.Lcd.drawFastHLine(r.x+1, r.y+6, 6, 0);
  }
}
