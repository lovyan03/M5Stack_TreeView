M5Stack TreeView menu UI library.
===

TreeView menu user interface library for M5Stack.  

M5Stack用 ツリービュー形式のメニューUIライブラリ  

## Description

カスタマイズして独自のメニュー項目を作成できます。  

Support FACES Keyboard and GameBoy unit.  
Support PLUS Encoder unit.  
Support JoyStick unit.  
Support CardKB unit.  

M5Stackの各種ユニットでの操作にも対応。  

![treeview](https://user-images.githubusercontent.com/42724151/52274970-90550b00-2991-11e9-9e6a-dc62c61252a6.png)

 `BtnA click` : フォーカスを親へ移動  Move focus to parent item.  
 `BtnA hold`  : フォーカスを前へ移動  Move focus to the previous item.  
 `BtnB click` : 選択  Select focus item.  
 `BtnC click/hold` : フォーカスを次へ移動  Move focus to the next item.  
  
## Requirement
動作には M5OnScreenKeyboard が必要です。  
M5OnScreenKeyboard is required for operation.  
(https://github.com/lovyan03/M5Stack_OnScreenKeyboard)


## Usage

```
#include <M5Stack.h>
#include <M5TreeView.h>
M5TreeView tv;
void setup() {
  M5.begin();
  Wire.begin();
// setItemsでツリー構造の定義を作成します
// 個々のアイテムは new MenuItemで作成し、
// 引数で表示名のほかに、識別用タグ値、サブツリー、コールバック処理などを渡せます）
  tv.setItems(std::vector<MenuItem*>
               { new MenuItem( "main 1")
               , new MenuItem( "main 2", 2) // 識別用タグ値
               , new MenuItem( "main 3", func) // コールバック処理
               , new MenuItem( "main 4"
                             , std::vector<MenuItem*> // サブメニュー
                               { new MenuItem("sub1")
                               , new MenuItem("sub2") 
                               }
                             )
               }
             );
  tv.begin();
}

void loop() {
// tv.updateを繰り返し呼ぶことで動作します。
// update の引数に true を渡すと強制再描画します。
// update の戻り値には選択されたアイテムが返されます。
  tv.update();
}

void func(MenuItem* mi) {
// コールバック関数やファンクタが設定されていると、アイテム選択時に呼び出されます。
// （親アイテムに設定されている場合にも呼び出されます。）
// 引数には選択されたアイテムのポインタが渡されます。
}
```

## Licence

[MIT](https://github.com/lovyan03/M5Stack_TreeView/blob/master/LICENSE)  

## Author

[lovyan03](https://twitter.com/lovyan03)  
