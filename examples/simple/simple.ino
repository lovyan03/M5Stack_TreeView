#include <vector>
#include <M5Stack.h>
#include <M5TreeView.h>

M5TreeView treeView;

typedef std::vector<MenuItem*> vmi;

void setup() {
  M5.begin();
  Wire.begin();

  treeView.useFACES       = true;
  treeView.useCardKB      = true;
  treeView.useJoyStick    = true;
  treeView.usePLUSEncoder = true;
  treeView.useFACESEncoder= true;
  treeView.clientRect.x = 2;
  treeView.clientRect.y = 10;
  treeView.clientRect.w = 316;
  treeView.clientRect.h = 216;

  treeView.setItems(vmi
               { new MenuItem("main 1", 1, vmi
                 { new MenuItem("sub 1-1", 11, vmi
                   { new MenuItem("sub 1-1-1", 111)
                   } )
                 } )
               , new MenuItem("main 2", vmi
                 { new MenuItem("sub 2-1", vmi
                   { new MenuItem("sub 2-1-1", 211)
                   , new MenuItem("sub 2-1-2", 212)
                   } )
                 , new MenuItem("sub 2-2", vmi
                   { new MenuItem("sub 2-2-1", 221)
                   , new MenuItem("sub 2-2-2", 222)
                   } )
                 } )
               , new MenuItem("main 3", vmi
                 { new MenuItem("sub 3-1", vmi
                   { new MenuItem("sub 3-1-1", vmi
                     { new MenuItem("sub 3-1-1-1", 3111)
                     , new MenuItem("sub 3-1-1-2", 3112)
                     , new MenuItem("sub 3-1-1-3", 3113)
                   } )
                   , new MenuItem("sub 3-1-2", vmi
                     { new MenuItem("sub 3-1-2-1", 3121)
                     , new MenuItem("sub 3-1-2-2", 3122)
                     , new MenuItem("sub 3-1-2-3", 3123)
                   } )
                   , new MenuItem("sub 3-1-3", vmi
                     { new MenuItem("sub 3-1-3-1", 3131)
                     , new MenuItem("sub 3-1-3-2", 3132)
                     , new MenuItem("sub 3-1-3-3", 3133)
                     } )
                   } )
                 , new MenuItem("sub 3-2", vmi
                   { new MenuItem("sub 3-2-1", vmi
                     { new MenuItem("sub 3-2-1-1", 3211)
                     , new MenuItem("sub 3-2-1-2", 3212)
                     , new MenuItem("sub 3-2-1-3", 3213)
                   } )
                   , new MenuItem("sub 3-2-2", 322)
                   , new MenuItem("sub 3-2-3", 323)
                   } )
                 , new MenuItem("sub 3-3", vmi
                   { new MenuItem("sub 3-3-1", vmi
                     { new MenuItem("sub 3-3-1-1", 3311)
                     , new MenuItem("sub 3-3-1-2", 3312)
                     , new MenuItem("sub 3-3-1-3", 3313)
                   } )
                   , new MenuItem("sub 3-3-2", 332)
                   , new MenuItem("sub 3-3-3", vmi
                     { new MenuItem("sub 3-3-3-1", 3331)
                     , new MenuItem("sub 3-3-3-2", 3332)
                     , new MenuItem("sub 3-3-3-3", 3333)
                     } )
                   } )
                 } )
               }
             );

  treeView.begin();
}

void loop() {
  MenuItem* mi = treeView.update();
  if (mi != NULL) {
    M5.Lcd.fillRect(0,0,320,8,0);
    M5.Lcd.setTextColor(0xffff,0);
    M5.Lcd.setTextSize(1);
    M5.Lcd.drawString("menu:" + mi->title + " / tag:" + mi->tag, 15, 0, 1);
  }
}