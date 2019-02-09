#include <vector>
#include <M5Stack.h>
#include <M5TreeView.h>
#include <M5OnScreenKeyboard.h>
#include <MenuItemSD.h>
#include <MenuItemSPIFFS.h>
#include <MenuItemWiFiClient.h>
#include <Preferences.h>

#include "MenuItemSDUpdater.h"
#include "HeaderSample.h"
#include "CBFTPserver.h"

M5TreeView treeView;
M5OnScreenKeyboard osk;
HeaderSample header;

typedef std::vector<MenuItem*> vmi;

void setup() {
  M5.begin();
  Wire.begin();

  Preferences preferences;
  preferences.begin("wifi-config");
  WiFi.begin(preferences.getString("WIFI_SSID").c_str(), preferences.getString("WIFI_PASSWD").c_str());
  preferences.end();

  treeView.clientRect.x = 2;
  treeView.clientRect.y = 16;
  treeView.clientRect.w = 196;
  treeView.clientRect.h = 200;
  treeView.itemWidth = 186;
  treeView.itemHeight = 18;
  treeView.font = 1;

  treeView.useFACES = true;
  treeView.useJoyStick = true;
  treeView.usePLUSEncoder = true;
  osk.useTextbox     = true;
  osk.useFACES       = true;
  osk.useCardKB      = true;
  osk.useJoyStick    = true;
  osk.usePLUSEncoder = true;

  treeView.setItems(vmi
               { new MenuItemWiFiClient("WiFi Client", CallBackWiFiClient)
               , new MenuItemSDUpdater("SD Updater")
               , new MenuItemSD("SD card")
               , new MenuItemSPIFFS("SPIFFS")
               , new MenuItem("FTP server", CBFTPserver())
               });
  treeView.begin();
}



bool redraw = true;
void loop() {

  if (redraw) {
    Rect16 r = treeView.clientRect;
    r.inflate(1);
    M5.Lcd.drawRect(r.x -1, r.y, r.w +2, r.h, MenuItem::frameColor[1]);
    M5.Lcd.drawRect(r.x, r.y -1, r.w, r.h +2, MenuItem::frameColor[1]);
    redraw = false;
    treeView.update(true);
  } else {
    treeView.update();
  }
  header.draw();
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
    osk.setup(wifi_passwd);
    while (osk.loop()) { delay(1); }
    wifi_passwd = osk.getString();
    osk.close();
    redraw = true;
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

