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

static void drawAuthIcon(const Rect16 r, uint16_t color) {
  M5.Lcd.fillRect(r.x, r.y+3, r.w  , 3, color);
  M5.Lcd.drawFastVLine(r.x+1, r.y+1, 2, color);
  M5.Lcd.drawFastVLine(r.x+4, r.y+1, 2, color);
  M5.Lcd.drawFastHLine(r.x+2, r.y  , 2, color);
}

static void drawWiFiIcon(const Rect16 r, uint8_t power, uint16_t color) {
  switch (power) {
  case 4:  M5.Lcd.drawFastHLine(r.x  , r.y  , 7, color);
  case 3:  M5.Lcd.drawFastHLine(r.x+1, r.y+2, 5, color);
  case 2:  M5.Lcd.drawFastHLine(r.x+2, r.y+4, 3, color);
  default: M5.Lcd.drawPixel(r.x+3, r.y+6, color);
  }
}

void MenuItemWiFiClient::onAfterDraw() {
  if (ssid.length() == 0) return;
  int y = rect.y + (rect.h - 6) / 2;

  uint16_t color (fontColor[this == focusItem ? 1 : 0]);

  if (WiFi.SSID() == ssid) {
    M5.Lcd.drawPixel(rect.x + 3, y + 4, color);
    M5.Lcd.drawLine(rect.x + 4, y + 5, rect.x + 8, y + 1, color);
  }

  if (auth != WIFI_AUTH_OPEN) {
    drawAuthIcon( Rect16 ( rect.right() - 18, y, 6, 6)
                , color);
  }
  drawWiFiIcon( Rect16 ( rect.right() - 10, y, 8, 8)
              , (rssi < -90 ? 1 : (rssi < -80 ? 2 : (rssi < -70 ? 3 : 4)))
              , color);
}
void MenuItemWiFiClient::scanWiFi()
{
  applyFont();
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
