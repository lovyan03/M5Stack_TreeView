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
  static int16_t font;
  static bool useFACES;
  static bool useCardKB;
  static bool useJoyStick;
  static bool usePLUSEncoder;

  MenuItem* const parentItem() const { return _parentItem; }
  MenuItem* const topItem() { return _parentItem ? _parentItem->topItem() : this; }

public:
  MenuItem() : MenuItem("", NULL, NULL) {}
  MenuItem(const String& title, const std::vector<MenuItem*> &sub) : MenuItem(title, NULL, sub) {}
  MenuItem(const String& title, std::function<void(MenuItem*)> cb) : MenuItem(title, NULL, cb) {}
  MenuItem(const String& title, std::function<void(MenuItem*)> cb, const std::vector<MenuItem*> &sub) : MenuItem(title, NULL, cb, sub) {}
  MenuItem(const String& title, int tg    , const std::vector<MenuItem*> &sub) : MenuItem(title, tg, NULL, sub ) {}
  MenuItem(const String& title, int tg = 0, std::function<void(MenuItem*)> cb = NULL);
  MenuItem(const String& title, int tg    , std::function<void(MenuItem*)> cb, const std::vector<MenuItem*> &sub);
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
  void setFocusItem(MenuItem* newmi);
  MenuItem* draw(bool force = true, const Rect16* forceRect = 0, const Rect16* prevforceRect = 0, MenuItem* nextmi = NULL);
  MenuItem* erase(bool force = true, MenuItem* nextmi = NULL);

  virtual void onEnter();
  virtual void onExit() { }
  virtual void onFocus() { }
  virtual void onDefocus() { }
  virtual void onAfterDraw();

  int16_t updateDest();
  virtual int16_t updateDestRect(MenuItem* mi, int16_t x = 0, int16_t y = 0);

  std::vector<MenuItem*> Items;
  String title;
  std::function<void(MenuItem*)> callback = 0;
  Rect16 destRect;    // destinationPoint
  Rect16 rect;        // displayPoint
  int tag = 0;
  bool visible;
  bool _hiding = false;
protected:
  static MenuItem* focusItem;
  static M5ButtonDrawer _btnDrawer;
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
