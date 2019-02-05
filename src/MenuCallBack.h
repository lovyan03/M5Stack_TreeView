#ifndef _MENUCALLBACK_H_
#define _MENUCALLBACK_H_

#include <M5Stack.h>
#include <M5ButtonDrawer.h>

class MenuItem;
//void (* std::function<void(MenuItem*)>)(MenuItem *); 

// メニュー選択時のコールバック
struct MenuCallBack {
  M5ButtonDrawer btnDrawer;
  virtual void operator()(MenuItem*) {
    M5.Lcd.fillScreen(MenuItem::backgroundColor);
    btnDrawer.setText("Back","","");
    if (setup()) {
      btnDrawer.draw(true);
      do {
        btnDrawer.draw();
      } while (loop());
      close();
      M5.Lcd.fillScreen(MenuItem::backgroundColor);
    }
  }
  virtual bool setup() { return true; };
  virtual bool loop()  { return false; };
  virtual void close() {};
  virtual ~MenuCallBack() {};
};

#endif