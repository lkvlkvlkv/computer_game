# 電腦對局導論作業二

## 作者

:::info
410985013 資工四 羅凱威
:::

## HackMD 網址

<https://hackmd.io/@LKV/computer_game_hw2>

## 編譯方法

- 我沒有特別把 `search.hpp`, `game.hpp`, `player.hpp` 寫進去就可以編了

```bash
g++ -std=c++17 main.cpp -o main
```

- 如果不行的話就把 hpp 也加進去

```bash
g++ -std=c++17 main.cpp search.hpp game.hpp player.hpp -o main
```

## 資料夾架構

- game.hpp
  - 用來執行一場遊戲
  - 建構時，可以傳入 `盤面大小 (n, m)`, `是否自行輸入盤面(by_input)`
  - 其餘還有
    - `產生隨機盤面 (randomBoard)`
    - `印出盤面 (printBoard)`
    - `產生合法操作 (expand)`
    - `移動旗子 (move)`
    - `回復上一動 (undo)`
    - `判斷遊戲結束 (isEnd)`
    - `判斷遊戲贏家 (winner)`
- player.hpp
  - 用來接收玩家的輸入
    - `顯示合法操作 (showMove)`
    - `獲取玩家操作 (playerMove)`
    - `獲取電腦操作 (computerMove)`
- search.hpp
  - 用來搜尋解法
  - 建構時，可以傳入 `搜尋深度 (depth)`，預設為深度 `8`
  - 其餘還有
    - `獲取電腦操作 (getMove)`
    - `判斷盤面好壞 (evaluate)`
    - `α-β 剪枝搜尋 (alphaBetaPruning)`
- main.cpp
  - 程式進入點，會詢問玩家 1, 2 分別是否為電腦
  - 執行一場遊戲，顯示結果後結束

## 執行畫面

- 執行後詢問玩家 1, 2 是否為電腦<br/>
<img src="https://hackmd.io/_uploads/rJVjcYyWR.png" alt="詢問玩家是否電腦" width=300>

---

- 顯示盤面，顯示合法操作，要求玩家輸入<br/>
<img src="https://hackmd.io/_uploads/rkGT9KJ-0.png" alt="要求玩家輸入" width=300>

---

- 電腦玩家會顯示計算時間，電腦選擇的移動<br/>
<img src="https://hackmd.io/_uploads/H1vA9YkWR.png" alt="顯示電腦移動" width=300>

---

- 終止盤面，顯示結果<br/>
<img src="https://hackmd.io/_uploads/BJzk2KJbR.png" alt="顯示結果" width=300>

## 執行效率

- test.cpp
  - 執行 10 場遊戲，每一步操作都會 call 1~9 步各算一次紀錄時間，並寫入 log.txt
  - 經由 statistics.py 來統計各個步數的資料
  - 總共算了 491 turns，下面的時間單位都是 ms<br/>
<img src="https://hackmd.io/_uploads/Hya5FikbC.png" alt="統計資料" width=300>
  - 將步數和時間 plot 出來<br/>
<img src="https://hackmd.io/_uploads/SJ8ots1ZA.png" alt="步數時間關係圖" width=350>
- 總之我最後讓電腦算深度 8，即雙方對奕各四步，時間上比較可以接受
