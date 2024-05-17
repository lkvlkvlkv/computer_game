# 電腦對局導論作業三

## 作者

:::info
410985013 資工四 羅凱威
:::

## HackMD 網址

<https://hackmd.io/@LKV/computer_game_hw3>

## 編譯方法

```bash
g++ .\main.cpp .\Game\game.cpp .\Search\search.cpp .\Player\player.cpp .\Display\display.cpp -o main
```

## 資料夾架構

- ./Display/display.h
  - 用來顯示盤面、存一些顯示相關的陣列
  - 解決 Game, Player 相互引用的問題，兩者可以引用 Display 來存取顯示相關的變數

- ./Game/game.h
  - 用來執行一場遊戲
    - `產生合法操作 (expand)`
    - `移動旗子 (move)`
    - `回復上一動 (undo)`
    - `判斷遊戲結束 (isEnd)`
    - `判斷遊戲贏家 (winner)`

- ./PLayer/player.h
  - 用來接收玩家的輸入
    - `獲取玩家操作 (getMove)`
    - `創建電腦或人類玩家的工廠 (createPlayer)`
    - 在電腦玩家創建時會詢問是否存檔，搜尋深度 (建議為 6~8)

- ./Search/search.h
  - 用來搜尋解法
  - 建構時，可以傳入 `搜尋深度 (depth)`，預設為深度 `8`
  - 其餘還有
    - `獲取電腦操作 (getMove)`
    - `判斷盤面好壞 (evaluate)`
    - `上次作業 α-β 剪枝搜尋 (alphaBetaPruning)`
    - `這次為了 debug 寫了跟簡報相同的 (AlphaBetaSearch)`
    - `一樣為了 debug 寫了跟簡報相同的 (NegaMax_VALUE)`
    - `這次作業要求，寫了跟簡報相同的 (NegaScoutSearch)`
    - `用來比較四個搜尋法的操作、分數的 (compare)`

- main.cpp
  - 程式進入點，會詢問玩家 1, 2 分別是否為電腦
  - 若玩家為電腦，會詢問是否存檔，搜尋深度
  - 執行一場遊戲，顯示結果後結束

## 執行畫面

- 執行後詢問玩家 1, 2 是否為電腦<br/>
<img src="https://hackmd.io/_uploads/Hy-1kmH70.png" alt="詢問玩家是否電腦" width=600>

---

- 顯示盤面，顯示合法操作，要求玩家輸入<br/>
<img src="https://hackmd.io/_uploads/rkGT9KJ-0.png" alt="要求玩家輸入" width=300>

---

- 電腦玩家會顯示計算時間，電腦選擇的移動<br/>
<img src="https://hackmd.io/_uploads/H1vA9YkWR.png" alt="顯示電腦移動" width=300>

---

- 終止盤面，顯示結果<br/>
<img src="https://hackmd.io/_uploads/BJzk2KJbR.png" alt="顯示結果" width=300>
