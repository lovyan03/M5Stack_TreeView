#ifndef _MENUCALLBACK_H_
#define _MENUCALLBACK_H_

#include <M5Stack.h>
#include <M5TreeView.h>
#include <M5ButtonDrawer.h>

class MenuItem;

// メニュー選択時のコールバック
struct MenuCallBack {
  M5ButtonDrawer btnDrawer;
  virtual void operator()(MenuItem* mi) {
    M5TreeView* tv((M5TreeView*)(mi->topItem()));
    M5TreeView::eCmd cmd;
    M5.Lcd.fillScreen(0);
    btnDrawer.setText("Back","","");
    if (setup()) {
      byte i = -1;
      do {
        cmd = tv->checkInput();
        btnDrawer.draw(0 == (++i & 0x0F));
      } while (cmd != M5TreeView::eCmd::BACK && loop());
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
