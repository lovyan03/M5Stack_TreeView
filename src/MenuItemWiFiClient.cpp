#include <WiFi.h>
#include <MenuItemWiFiClient.h>
#include <Rect16.h>

void MenuItemWiFiClient::onEnter() {
  if (ssid == "") {
    scanWiFi();
  }

  MenuItem::onEnter();
}

void MenuItemWiFiClient::onExit() {
  WiFi.scanDelete();
  deleteItems();

  MenuItem::onExit();
}

void MenuItemWiFiClient::onAfterDraw() {
  if (ssid.length() == 0) return;
  Rect16 r( rect.x + 4
          , rect.y + (rect.h - 6) / 2
          , 6
          , 6);
  if (auth != WIFI_AUTH_OPEN) {
    uint16_t color = fontColor[this == focusItem ? 1 : 0];
    M5.Lcd.fillRect(r.x, r.y+3, r.w  , 3, color);
    M5.Lcd.drawFastVLine(r.x+1, r.y+1, 2, color);
    M5.Lcd.drawFastVLine(r.x+4, r.y+1, 2, color);
    M5.Lcd.drawFastHLine(r.x+2, r.y  , 2, color);
  }
  M5.Lcd.drawRightString(String(rssi, DEC) + "dBm", rect.right() - 10, rect.y + (rect.h - M5.Lcd.fontHeight(font)) / 2, font);
}
void MenuItemWiFiClient::scanWiFi()
{
  trimFillRect( rect.x + 1, rect.y + 1, rect.w - 2, rect.h - 2, backColor[1]);
  drawTitle(true, "WiFi Scanning...");
  if (WiFi.status() != WL_CONNECTED) WiFi.disconnect();
  int count = WiFi.scanNetworks();

  deleteItems();
  for ( int i = 0; i < count; ++i) {
    wifi_auth_mode_t atmp = WiFi.encryptionType(i);
    MenuItemWiFiClient* mi = new MenuItemWiFiClient(WiFi.SSID(i));
    mi->ssid = WiFi.SSID(i);
    mi->rssi = WiFi.RSSI(i);
    mi->auth = atmp;
    addItem(mi);
  }
}
