#include <vector>
#include <M5Stack.h>
#include <M5TreeView.h>

#include "MenuItemSDUpdater.h"

M5TreeView treeView;

typedef std::vector<MenuItem*> vmi;

void setup() {
  M5.begin();
  Wire.begin();

  treeView.clientRect.x = 2;
  treeView.clientRect.y = 12;
  treeView.clientRect.w = 196;
  treeView.clientRect.h = 210;
  treeView.itemWidth = 186;
  treeView.itemHeight = 18;
  treeView.font = 1;

  treeView.useFACES = true;
  treeView.useJoyStick = true;
  treeView.usePLUSEncoder = true;

  treeView.setItems(vmi
               { new MenuItemSDUpdater("SD Updater") }
               );
  treeView.begin();

  M5.Lcd.drawRect(0,11,200,212,MenuItem::frameColor[1]);
  M5.Lcd.drawRect(1,10,198,214,MenuItem::frameColor[1]);
}

bool force = false;
void loop() {
  treeView.update();
}