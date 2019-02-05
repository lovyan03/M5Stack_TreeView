#ifndef _MENUITEMWIFICLIENT_H_
#define _MENUITEMWIFICLIENT_H_

#include <WiFi.h>
#include <MenuItem.h>
#include <Rect16.h>
#include <M5OnScreenKeyboard.h>

class MenuItemWiFiClient : public MenuItem {
public:
  MenuItemWiFiClient(const String& title, std::function<void(MenuItem*)>cb = 0)
  : MenuItem(title, cb) {};

  virtual void onEnter();
  virtual void onExit();
  virtual void onAfterDraw();

  String ssid;
  int8_t rssi;
  wifi_auth_mode_t auth;

private:
  void scanWiFi();
};

#endif
