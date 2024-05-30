# 電腦對局導論期末作業

## 作者

410985013 資工四 羅凱威

email: <a23202418@gmail.com>

## HackMD 網址

<https://hackmd.io/@LKV/computer_game_final_project>

## 編譯方法

```bash
g++ main.cpp MyAI.cpp .\Game\game.cpp .\Display\display.cpp .\Search\search.cpp -o main
```

## 資料夾架構

- 基本上就是 hw3，拔掉 Player 這一層，直接 call search。
- 修改遊戲規則，讓 Game.expand 可以吃同顏色的旗子

- ./Display/display.h
  - 用來顯示盤面、存一些全局變數的陣列

- ./Game/game.h
  - 用來執行一場遊戲
    - `根據骰子產生合法操作 (expand)`
    - `根據數字產生合法操作 (expandNumber)`
    - `移動旗子 (move)`
    - `回復上一動 (undo)`
    - `判斷遊戲結束 (isEnd)`
    - `判斷遊戲贏家 (winner)`

- ./Search/search.h
  - 用來搜尋解法
  - 建構時，可以傳入 `搜尋深度 (depth)`，預設為深度 `8`
  - 其餘還有
    - `獲取操作 (getMove)`
    - `判斷盤面好壞 (evaluate)`
    - `AlphaBetaSearch`
    - `NegaMax_VALUE`
    - `NegaScoutSearch`
    - `用來比較各個搜尋法的操作、分數的 (compare)`

## 問題

### 一定能聯絡到你的方式

email: a23202418@gmail.com

### 採用那些技術和演算法

最後是用 negaMaxSearch

### 測試數據

- 比賽的時候 90 場贏了 72 場，大概 8 成勝率
  (不排除我沒對到的剩下兩個人 counter 我)
- 勝率比較高的原因我猜測可能是
  1. 大家審局函式邏輯不會差太多，同時我剛好搜得深了一兩步
  2. 或是我對於機率搜尋的正確性比較高

#### 比賽記分板

![image](https://hackmd.io/_uploads/H1Re8iHN0.png)

#### 測試賽前我在跑 7 層的數據，跑到一半被中斷了

![畫面截圖](https://hackmd.io/_uploads/r1WGooHVR.png)

### 你的愛因斯坦棋程式特別之處

#### 基本上沒有什麼特別調整的

就是可以跑六層，大概花費兩三秒。

#### 我有想過有沒有要為了報告的這一項去特別跑七層

因為跑七層大概二十秒，是可接受的時間，另外老師說過雙方對局的遊戲，搜尋偶數比較合理，估計大家都會是偶數深度，若我是奇數深度，就會比較特別。

但我自己打隨機電腦 100 場也大概 6 層贏 7 成，7 層 贏 6 成，所以就還是選擇搜 6 層

#### 對於機率我的處理

若有確定骰到的數字，會傳給 negaMax 該骰到的數字，否則給予 -1，
當傳入的數字為 -1 時，代表有 1/6 的機率骰到各個數字，
Negamax 就會去計算骰到各個數字的結果，再把結果 / 6 傳回來。

![image](https://hackmd.io/_uploads/ryYdIsSV0.png)

#### 這樣實作的缺點 => branching factor 最大為 10

在 Worst-case 的情況下，

假設 2,3,4,5 掛掉了，
骰到 1 會計算走 1，
骰到 2,3,4,5 會計算走 1, 6，
骰到 6 會計算走 6，
等於走 1,6 分別各算了 5 次，

若有將選擇移動哪個旗子的結果存起來可以縮小不少 braching factor，
目前這樣最大的 branching factor 為 10，若有存下來最多為 6，
說不定這樣就可以跟 hw3 一樣搜到 8 層也只需要 8 秒左右。
(理想上我也覺得搜到 8 層會強很多，因為開局的時候任意旗子走四步都可以到終點，整個遊戲樹的發展在初期就可以大致看清)

但最後覺得練習賽的成績已經還不錯了 (加上最近被其他課業追殺)，
就沒有時間繼續做更改，TODO 就還留在程式碼裡面。
