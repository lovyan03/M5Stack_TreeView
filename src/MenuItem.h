#ifndef _MENUITEM_H_
#define _MENUITEM_H_

#include <M5Stack.h>
#include <vector>
#include <functional>

#include <M5ButtonDrawer.h>
#include "Rect16.h"

class MenuItem {
protected:
public:
  using TCallBackEnter = std::function<void(MenuItem*)>;

  static Rect16 clientRect;
  static uint16_t itemHeight;
  static uint16_t itemWidth;
  static int8_t treeOffset;
  static uint16_t frameColor[2];
  static uint16_t backColor[2];
  static uint16_t fontColor[2];
  static uint16_t backgroundColor;
  static uint32_t msecHold;
  static uint32_t msecRepeat;
  static int16_t titleOffset;
  static bool useFACES;
  static bool useCardKB;
  static bool useJoyStick;
  static bool usePLUSEncoder;
  static bool useFACESEncoder;
  static bool useLowClockDelay;
  static bool swapBtnBC;

  static void setTextFont(int f) { gfxFont = NULL; font = f; }
  static void setFreeFont(const GFXfont* f) { gfxFont = f; font = 1; }
  static void setFocusItem(MenuItem* newmi);
  static MenuItem* getFocusItem();

  MenuItem* parentItem() const { return _parentItem; }
  MenuItem* topItem() { return _parentItem ? _parentItem->topItem() : this; }

  MenuItem() : MenuItem("", 0, NULL) {}
  MenuItem(const String& title, const std::vector<MenuItem*> &sub) : MenuItem(title, 0, sub) {}
  MenuItem(const String& title, TCallBackEnter cb) : MenuItem(title, 0, cb) {}
  MenuItem(const String& title, TCallBackEnter cb, const std::vector<MenuItem*> &sub) : MenuItem(title, 0, cb, sub) {}
  MenuItem(const String& title, int tg    , const std::vector<MenuItem*> &sub) : MenuItem(title, tg, NULL, sub ) {}
  MenuItem(const String& title, int tg = 0, TCallBackEnter cb = NULL);
  MenuItem(const String& title, int tg    , TCallBackEnter cb, const std::vector<MenuItem*> &sub);
  virtual ~MenuItem() {};

  void addItem(MenuItem* item);
  void addItems(const std::vector<MenuItem*> &sub);
  void setItems(const std::vector<MenuItem*> &sub);
  void deleteItems();

  void drawTitle(bool selected);
  void drawTitle(bool selected, const String& text);
  void drawText(String text, int16_t x, int16_t y);
  void focusBack();
  void focusNext();
  void focusPrev();
  bool focusEnter();
  MenuItem* draw(bool force = true, const Rect16* forceRect = 0, const Rect16* prevforceRect = 0, MenuItem* nextmi = NULL);
  MenuItem* erase(bool force = true, MenuItem* nextmi = NULL);

  virtual void onEnter();
  virtual void onExit() { }
  virtual void onFocus() { }
  virtual void onFocusLoop() { }
  virtual void onDefocus() { }
  virtual void onAfterDraw();

  // title right padding
  virtual int getRightPadding() const { return 0; }

  int16_t updateDest();
  int16_t updateDestRect(int16_t x = 0, int16_t y = 0);

  std::vector<MenuItem*> Items;
  String title;
  TCallBackEnter callback = 0;
  int tag = 0;
  Rect16 destRect;    // destinationPoint
  Rect16 rect;        // displayPoint
  bool visible;
  bool _hiding = false;
protected:
  static int16_t font;
  static const GFXfont* gfxFont;
  static MenuItem* focusItem;
  static M5ButtonDrawer _btnDrawer;
  static int16_t fontHeight();
  static void applyFont();
  static void trimFillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
  static bool compareIgnoreCase(MenuItem* lhs, MenuItem* rhs);

  bool move(bool force = false);
  void scrollDestRectY(int16_t add_y);
  void scrollSubitemArea(MenuItem* mi);
  void scrollTarget(MenuItem* targetItem);
  void updateButtons();
  bool _moving;
  MenuItem* _parentItem = NULL;
  Rect16 _oldRect;    // previousDisplayPoint
};

#endif
