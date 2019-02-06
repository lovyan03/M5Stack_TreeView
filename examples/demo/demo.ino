#include <vector>
#include <SD.h>
#include <SPIFFS.h>
#include <M5Stack.h>
#include <M5TreeView.h>
#include <MenuItemWiFiClient.h>
#include <MenuItemSD.h>
#include <MenuItemSPIFFS.h>
#include <M5ButtonDrawer.h>

#include "HeaderSample.h"

#include "SystemInfo.h"
#include "I2CScanner.h"
#include "MPU9250Demo.h"
#include "ScrollDemo.h"

M5TreeView treeView;
M5OnScreenKeyboard osk;
HeaderSample header;

typedef std::vector<MenuItem*> vmi;

template <typename T>
void CallBackExec(MenuItem* sender)
{
  T menucallback;
  menucallback(sender);
}



void setup() {
  M5.begin();
  Wire.begin();

  treeView.clientRect.x = 2;
  treeView.clientRect.y = 16;
  treeView.clientRect.w = 316;
  treeView.clientRect.h = 200;
  treeView.itemWidth    = 220;

  treeView.useFACES       = true;
  treeView.useJoyStick    = true;
  treeView.usePLUSEncoder = true;
  osk.useTextbox     = true;
  osk.useFACES       = true;
  osk.useCardKB      = true;
  osk.useJoyStick    = true;
  osk.usePLUSEncoder = true;
  osk.msecHold   = treeView.msecHold;
  osk.msecRepeat = treeView.msecRepeat;

  treeView.setItems(vmi
               { new MenuItem("Style Sample ", vmi
                 { new MenuItem("Style 1", 1001)
                 , new MenuItem("Style 2", 1002)
                 , new MenuItem("Style 3", 1003)
                 , new MenuItem("Style 4", 1004)
                 , new MenuItem("Style 5", 1005)
                 } )
               , new MenuItem("Demo", vmi
                 { new MenuItem("System Info", CallBackExec<SystemInfo>)
                 , new MenuItem("I2C Scanner", CallBackExec<I2CScanner>)
                 , new MenuItem("MPU9250",     CallBackExec<MPU9250Demo>)
                 , new MenuItem("Scroll Demo", CallBackExec<ScrollDemo>)
                 } )
               , new MenuItem("Numeric Sample ", vmi
                 { new MenuItemNumeric("Brightness", 0, 255, 80, CallBackBrightness)
                 , new MenuItemNumeric("GPIO25 DAC", 0, 255, 0, 25, CallBackDACtest)
                 , new MenuItemNumeric("GPIO26 DAC", 0, 255, 0, 26, CallBackDACtest)
                 } )
               , new MenuItem("WiFi ", vmi
                 { new MenuItemWiFiClient("WiFi Client", CallBackWiFiClient)
                 , new MenuItem("WiFi mode", vmi
                   { new MenuItem("WiFi disconnect(true)", 2000)
                   , new MenuItem("WiFi mode OFF", 2001)
                   , new MenuItem("WiFi mode STA", 2002)
                   , new MenuItem("WiFi mode AP" , 2003)
                   , new MenuItem("WiFi mode AP STA", 2004)
                 } )
               } )
               , new MenuItemSD("SD card", CallBackFS)
               , new MenuItemSPIFFS("SPIFFS", CallBackFS)
               , new MenuItem("Tree sample", vmi
                 { new MenuItem("sub 1", vmi
                   { new MenuItem("sub 1-1", vmi
                     { new MenuItem("sub 1-1-1")
                     , new MenuItem("sub 1-1-2")
                     , new MenuItem("sub 1-1-3")
                     } )
                   , new MenuItem("sub 1-2", vmi
                     { new MenuItem("sub 1-2-1")
                     , new MenuItem("sub 1-2-2")
                     , new MenuItem("sub 1-2-3")
                     } )
                   } )
                 , new MenuItem("sub 2", vmi
                   { new MenuItem("sub 2-1")
                   , new MenuItem("sub 2-2")
                   , new MenuItem("sub 2-3")
                   } )
                 , new MenuItem("sub 3", vmi
                   { new MenuItem("sub 3-1", vmi
                     { new MenuItem("sub 3-1-1")
                     , new MenuItem("sub 3-1-2")
                     , new MenuItem("sub 3-1-3")
                     } )
                   , new MenuItem("sub 3-2")
                   , new MenuItem("sub 3-3", vmi
                     { new MenuItem("sub 3-3-1")
                     , new MenuItem("sub 3-3-2")
                     , new MenuItem("sub 3-3-3")
                     } )
                   } )
                 } )
               } );

  treeView.begin();
}

uint32_t loopcounter = 0;
bool redraw = true;
void loop() {
  bool fld_redraw = redraw;
  if (redraw) {
    Rect16 r = treeView.clientRect;
    r.inflate(1);
    M5.Lcd.fillScreen(treeView.backgroundColor);
    M5.Lcd.drawRect(r.x -1, r.y, r.w +2, r.h, MenuItem::frameColor[1]);
    M5.Lcd.drawRect(r.x, r.y -1, r.w, r.h +2, MenuItem::frameColor[1]);
    redraw = false;
  }
  
  if (!(loopcounter % 10))  header.draw();

  MenuItem* mi = treeView.update(fld_redraw);

  if (mi != NULL) {
    switch (mi->tag) {
    default: return;

    case 1001:
      treeView.fontColor[0]  = 0xFFF0;
      treeView.backColor[0]  = 0x0000;
      treeView.frameColor[0] = 0x0000;
      treeView.fontColor[1]  = 0x0000;
      treeView.backColor[1]  = 0xFFF0;
      treeView.frameColor[1] = 0xFFFF;
      treeView.backgroundColor = 0x0000;
      treeView.itemHeight = 20;
      treeView.font = 1;
      M5ButtonDrawer::height = 14;
      M5ButtonDrawer::width = 64;
      break;

    case 1002:
      treeView.fontColor[0]  = 0xFFE0;
      treeView.backColor[0]  = 0x0100;
      treeView.frameColor[0] = 0x6300;
      treeView.fontColor[1]  = 0x7FE0;
      treeView.backColor[1]  = 0xC400;
      treeView.frameColor[1] = 0xFF00;
      treeView.backgroundColor = 0x1000;
      treeView.itemHeight = 16;
      treeView.font = 1;
      M5ButtonDrawer::height = 14;
      M5ButtonDrawer::width = 64;
      break;

    case 1003:
      treeView.fontColor[0]  = 0xFFFF;
      treeView.backColor[0]  = 0x0000;
      treeView.frameColor[0] = 0xA514;
      treeView.fontColor[1]  = 0x0000;
      treeView.backColor[1]  = 0xFFF8;
      treeView.frameColor[1] = 0xFFFF;
      treeView.backgroundColor = 0x4208;
      treeView.font = 2;
      treeView.itemHeight = 20;
      M5ButtonDrawer::height = 20;
      M5ButtonDrawer::width = 80;
      break;

    case 1004:
      treeView.fontColor[0]  = 0xFFFF;
      treeView.backColor[0]  = 0x6314;
      treeView.frameColor[0] = 0x0118;
      treeView.fontColor[1]  = 0x7FFF;
      treeView.backColor[1]  = 0x441F;
      treeView.frameColor[1] = 0xFFF0;
      treeView.backgroundColor = 0x0008;
      treeView.font = 2;
      treeView.itemHeight = 28;
      M5ButtonDrawer::height = 20;
      M5ButtonDrawer::width = 80;
      break;

    case 1005:
      treeView.fontColor[0]  = 0xFFFF;
      treeView.backColor[0]  = 0x630C;
      treeView.frameColor[0] = 0x0208;
      treeView.fontColor[1]  = 0xFFFF;
      treeView.backColor[1]  = 0x421F;
      treeView.frameColor[1] = 0xFFFF;
      treeView.backgroundColor = 0x0000;
      treeView.font = 1;
      treeView.itemHeight = 18;
      M5ButtonDrawer::height = 14;
      M5ButtonDrawer::width = 64;
      break;

    case 2000:
      WiFi.disconnect(true);
      return;

    case 2001:
      WiFi.mode(WIFI_OFF);
      return;

    case 2002:
      WiFi.mode(WIFI_STA);
      return;

    case 2003:
      WiFi.mode(WIFI_AP);
      return;

    case 2004:
      WiFi.mode(WIFI_AP_STA);
      return;
    }
    for (int i = 0; i < 2; ++i) {
      M5ButtonDrawer::fontColor[i] = treeView.fontColor[i];
      M5ButtonDrawer::backColor[i] = treeView.backColor[i];
      M5ButtonDrawer::frameColor[i] = treeView.frameColor[i];
      osk.fontColor[i] = treeView.fontColor[i];
      osk.backColor[i] = treeView.backColor[i];
      osk.frameColor[i]= treeView.frameColor[i];
    }
    M5ButtonDrawer::font = treeView.font;
    
    redraw = true;
  }
}


String wifiClientPass;

void CallBackWiFiClient(MenuItem* sender)
{
  MenuItemWiFiClient* mi = static_cast<MenuItemWiFiClient*>(sender);
  if (!mi) return;

  if (mi->ssid == "") return;

  if (mi->auth != WIFI_AUTH_OPEN) {
    osk.setup(wifiClientPass);
    while (osk.loop()) { delay(1); }
    wifiClientPass = osk.getString();
    osk.close();

    redraw = true;
    WiFi.disconnect();
    WiFi.begin(mi->ssid.c_str(), wifiClientPass.c_str());
  } else {
    WiFi.disconnect();
    WiFi.begin(mi->ssid.c_str(), "");
  }
  while (M5.BtnA.isPressed()) M5.update();
}

void CallBackBrightness(MenuItem* sender) 
{
  MenuItemNumeric* mi = static_cast<MenuItemNumeric*>(sender);
  if (!mi) return;
  M5.Lcd.setBrightness(mi->value);
}


void CallBackDACtest(MenuItem* sender) 
{
  MenuItemNumeric* mi = static_cast<MenuItemNumeric*>(sender);
  if (!mi) return;

  pinMode(mi->tag, OUTPUT);
  dacWrite(mi->tag, mi->value);
}

void CallBackFS(MenuItem* sender) 
{
  MenuItemFS* mi = static_cast<MenuItemFS*>(sender);
  if (!mi) return;

  if (mi->isDir) return;

  M5.Lcd.clear(0);
  int idx = mi->path.lastIndexOf('.') + 1;
  String ext = mi->path.substring(idx);
  if (ext == "jpg") {
    M5.Lcd.drawJpgFile(mi->getFS(), mi->path.c_str());
  } else {
    File file = mi->getFS().open(mi->path, FILE_READ);
    if (!file.isDirectory()) {
      FileView(file);
    }
    file.close();
  }
  M5ButtonDrawer btnDrawer;
  btnDrawer.setText("Back","","");
  btnDrawer.draw(true);
  while (!M5.BtnA.wasReleased()) M5.update();
  redraw = true;
}

void FileView(File ff){
  M5.Lcd.setTextColor(0xffff,0);
  M5.Lcd.setCursor(0,0);
  size_t len = ff.size();
  uint8_t buf[256];

  if (ff.read(buf, len)) {
    for(int i=0; i<len; ++i){
      M5.Lcd.write(buf[i]);
    }
  }
}