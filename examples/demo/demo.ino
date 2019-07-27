#include <vector>
#include <SD.h>
#include <SPIFFS.h>
#include <M5Stack.h>
#include <M5TreeView.h>
#include <M5OnScreenKeyboard.h> // https://github.com/lovyan03/M5OnScreenKeyboard/
#include <MenuItemSD.h>
#include <MenuItemSPIFFS.h>
#include <MenuItemToggle.h>
#include <MenuItemWiFiClient.h>
#include <M5ButtonDrawer.h>
#include <Preferences.h>

#include "HeaderSample.h"

M5TreeView treeView;
M5OnScreenKeyboard osk;
HeaderSample header;

typedef std::vector<MenuItem*> vmi;

void drawFrame() {
  Rect16 r = treeView.clientRect;
  r.inflate(1);
  M5.Lcd.drawRect(r.x -1, r.y, r.w +2, r.h, MenuItem::frameColor[1]);
  M5.Lcd.drawRect(r.x, r.y -1, r.w, r.h +2, MenuItem::frameColor[1]);
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

void CallBackStyle(MenuItem* sender)
{
  switch (sender->tag) {
  default: return;

  case 1001:
    treeView.fontColor[0]  = 0xFFF0;
    treeView.backColor[0]  = 0x0000;
    treeView.frameColor[0] = 0x0000;
    treeView.fontColor[1]  = 0x0000;
    treeView.backColor[1]  = 0xFFF0;
    treeView.frameColor[1] = 0xFFFF;
    treeView.backgroundColor = 0x0000;
    treeView.itemHeight = 12;
    treeView.setFreeFont(&TomThumb);
    M5ButtonDrawer::setTextFont(1);
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
    treeView.setTextFont(1);
    M5ButtonDrawer::setTextFont(1);
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
    treeView.itemHeight = 18;
    treeView.setTextFont(2);
    M5ButtonDrawer::setTextFont(2);
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
    treeView.itemHeight = 20;
    treeView.setFreeFont(&FreeMono9pt7b);
    M5ButtonDrawer::setFreeFont(&FreeMono9pt7b);
    M5ButtonDrawer::height = 20;
    M5ButtonDrawer::width = 106;
    break;

  case 1005:
    treeView.fontColor[0]  = 0xFFFF;
    treeView.backColor[0]  = 0x4302;
    treeView.frameColor[0] = 0x0208;
    treeView.fontColor[1]  = 0xFFFF;
    treeView.backColor[1]  = 0x6704;
    treeView.frameColor[1] = 0xFFFF;
    treeView.backgroundColor = 0x0000;
    treeView.itemHeight = 24;
    treeView.setFreeFont(&FreeSans9pt7b);
    M5ButtonDrawer::setFreeFont(&FreeSans9pt7b);
    M5ButtonDrawer::height = 20;
    M5ButtonDrawer::width = 100;
    break;

  case 1006:
    treeView.fontColor[0]  = 0xFFFF;
    treeView.backColor[0]  = 0x630C;
    treeView.frameColor[0] = 0x0208;
    treeView.fontColor[1]  = 0xFFFF;
    treeView.backColor[1]  = 0x421F;
    treeView.frameColor[1] = 0xFFFF;
    treeView.backgroundColor = 0x0000;
    treeView.itemHeight = 20;
    treeView.setTextFont(1);
    M5ButtonDrawer::setTextFont(1);
    M5ButtonDrawer::height = 16;
    M5ButtonDrawer::width = 64;
    break;
  }
  for (int i = 0; i < 2; ++i) {
    M5ButtonDrawer::fontColor[i] = treeView.fontColor[i];
    M5ButtonDrawer::backColor[i] = treeView.backColor[i];
    M5ButtonDrawer::frameColor[i] = treeView.frameColor[i];
    osk.fontColor[i] = treeView.fontColor[i];
    osk.backColor[i] = treeView.backColor[i];
    osk.frameColor[i]= treeView.frameColor[i];
  }
  M5.Lcd.fillRect(0, 218, M5.Lcd.width(), 22, 0);
}

void CallBackWiFi(MenuItem* sender)
{
  switch (sender->tag) {
  default:     break;
  case 2000:  WiFi.disconnect(true);  break;
  case 2001:  WiFi.mode(WIFI_OFF);    break;
  case 2002:  WiFi.mode(WIFI_STA);    break;
  case 2003:  WiFi.mode(WIFI_AP);     break;
  case 2004:  WiFi.mode(WIFI_AP_STA); break;
  }
}

void CallBackWiFiClient(MenuItem* sender)
{
  MenuItemWiFiClient* mi = static_cast<MenuItemWiFiClient*>(sender);
  if (!mi) return;

  if (mi->ssid == "") return;

  Preferences preferences;
  preferences.begin("wifi-config");
  preferences.putString("WIFI_SSID", mi->ssid);
  String wifi_passwd = preferences.getString("WIFI_PASSWD");

  if (mi->auth != WIFI_AUTH_OPEN) {
    osk.swapBtnBC = treeView.swapBtnBC;
    osk.setup(wifi_passwd);
    while (osk.loop()) { delay(1); }
    wifi_passwd = osk.getString();
    osk.close();
    WiFi.disconnect();
    WiFi.begin(mi->ssid.c_str(), wifi_passwd.c_str());
    preferences.putString("WIFI_PASSWD", wifi_passwd);
  } else {
    WiFi.disconnect();
    WiFi.begin(mi->ssid.c_str(), "");
    preferences.putString("WIFI_PASSWD", "");
  }
  preferences.end();
  while (M5.BtnA.isPressed()) M5.update();
}

void CallBackToggletest(MenuItem* sender) 
{
  MenuItemToggle* mi = static_cast<MenuItemToggle*>(sender);
  if (!mi) return;

  pinMode(mi->tag, OUTPUT);
  digitalWrite(mi->tag, mi->value);
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

void CallBackBtnSwap(MenuItem* sender)
{
  treeView.swapBtnBC = !treeView.swapBtnBC;
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
  treeView.useFACESEncoder= true;
  osk.useTextbox     = true;
  osk.useFACES       = true;
  osk.useCardKB      = true;
  osk.useJoyStick    = true;
  osk.usePLUSEncoder = true;
  osk.useFACESEncoder= true;
  osk.msecHold   = treeView.msecHold;
  osk.msecRepeat = treeView.msecRepeat;

  treeView.setItems(vmi
               { new MenuItem("Style Sample ", CallBackStyle, vmi
                 { new MenuItem("Style 1", 1001)
                 , new MenuItem("Style 2", 1002)
                 , new MenuItem("Style 3", 1003)
                 , new MenuItem("Style 4", 1004)
                 , new MenuItem("Style 5", 1005)
                 , new MenuItem("Style 6", 1006)
                 } )
               , new MenuItem("Toggle Sample ", vmi
                 { new MenuItemToggle("GPIO 3", false,  3, CallBackToggletest)
                 , new MenuItemToggle("GPIO 1", false,  1, CallBackToggletest)
                 , new MenuItemToggle("GPIO16", false, 16, CallBackToggletest)
                 , new MenuItemToggle("GPIO17", false, 17, CallBackToggletest)
                 } )
               , new MenuItem("Numeric Sample ", vmi
                 { new MenuItemNumeric("Brightness", 0, 255, 80, CallBackBrightness)
                 , new MenuItemNumeric("GPIO25 DAC", 0, 255, 0, 25, CallBackDACtest)
                 , new MenuItemNumeric("GPIO26 DAC", 0, 255, 0, 26, CallBackDACtest)
                 } )
               , new MenuItem("WiFi ", vmi
                 { new MenuItemWiFiClient("WiFi Client", CallBackWiFiClient)
                 , new MenuItem("WiFi mode", CallBackWiFi, vmi
                   { new MenuItem("WiFi disconnect(true)", 2000)
                   , new MenuItem("WiFi mode OFF", 2001)
                   , new MenuItem("WiFi mode STA", 2002)
                   , new MenuItem("WiFi mode AP" , 2003)
                   , new MenuItem("WiFi mode AP STA", 2004)
                 } )
               } )
               , new MenuItemSD("SD card", CallBackFS)
               , new MenuItemSPIFFS("SPIFFS", CallBackFS)
               , new MenuItem("Btn B/C swap", CallBackBtnSwap)
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
  drawFrame();
}

uint32_t loopcounter = 0;
void loop() {
  MenuItem* mi = treeView.update();

  if (!(loopcounter % 10))  header.draw();

  if (treeView.isRedraw()) {
    drawFrame();
  }
}