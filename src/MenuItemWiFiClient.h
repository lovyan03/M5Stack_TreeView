#ifndef _MENUITEMWIFICLIENT_H_
#define _MENUITEMWIFICLIENT_H_

#include <WiFi.h>
#include <MenuItem.h>
#include <Rect16.h>

class MenuItemWiFiClient : public MenuItem {
public:
  MenuItemWiFiClient(const String& title, TCallBackEnter cb = 0)
  : MenuItem(title, cb) {};

  virtual void onEnter();
  virtual void onExit();
  virtual void onAfterDraw();

  virtual int getRightPadding() const { return 18; }

  String ssid;
  int8_t rssi;
  wifi_auth_mode_t auth;

private:
  void scanWiFi();
};

#endif
