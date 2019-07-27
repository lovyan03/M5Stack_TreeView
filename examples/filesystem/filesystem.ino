#include <vector>
#include <M5Stack.h>
#include <M5TreeView.h>
#include <MenuItemSD.h>
#include <MenuItemSPIFFS.h>

M5TreeView treeView;

typedef std::vector<MenuItem*> vmi;

void CallBackFS(MenuItem* sender)
{
  M5.Lcd.fillRect(0, 0, M5.Lcd.width(), 10, 0);

  MenuItemFS* mi = (MenuItemFS*)sender;
  if (mi->isDir) return;

  String filepath = mi->path;

  M5.Lcd.drawString(filepath, 0, 0, 1);
}

void setup() {
  M5.begin();
  Wire.begin();

  treeView.clientRect.x = 2;
  treeView.clientRect.y = 16;
  treeView.clientRect.w = 316;
  treeView.clientRect.h = 200;
  treeView.setTextFont(2);
  treeView.itemHeight   = 18;
  treeView.itemWidth    = 220;

  treeView.useFACES       = true;
  treeView.useJoyStick    = true;
  treeView.usePLUSEncoder = true;
  treeView.useFACESEncoder= true;

  treeView.setItems(vmi
               { new MenuItemSD("SD", CallBackFS)
               , new MenuItemSPIFFS("SPIFFS", CallBackFS)
               } );

  treeView.begin();
}

void loop() {
  treeView.update();
}
