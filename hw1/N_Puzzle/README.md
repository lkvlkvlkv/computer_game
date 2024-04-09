# 電腦對局導論作業一

## 作者

:::info
410985013 資工四 羅凱威
:::

## HackMD 網址

https://hackmd.io/@LKV/computer_game_hw1

## 前言

這是我拿之前[人工智慧導論程式作業一](https://hackmd.io/@LKV/Artificial_Inteligence_Program1)改的，之前為了練習物件導向，而且需要支援多種搜尋法，所以架構比較 OOP ，同時有些設計犧牲了一點效能，這次就將搜尋的部分改回比較有效率的方法，並且增加了隨機盤面的功能。

## 編譯方法

- 我沒有特別把 `generator.hpp`, `puzzle.hpp`, `node.hpp`, `search.hpp` 寫進去就可以編了

```bash
g++ -std=c++17 test1.cpp -o test1
```

```bash
g++ -std=c++17 test1000.cpp -o test1000
```

- 如果不行的話就把 hpp 也加進去

```bash
g++ -std=c++17 test1.cpp generator.hpp puzzle.hpp node.hpp search.hpp -o test1
```

```bash
g++ -std=c++17 test1000.cpp generator.hpp puzzle.hpp node.hpp search.hpp -o test1000
```

## 資料夾結構

- input
  - 會存放隨機生成的盤面
    - 格式為 `input/<random_seed>_<size>x<size>/random_board_<turn>.in`
    - 如果種子碼為 123，盤面大小為 3x3，第 1 個盤面，則檔名為 `input/123_3x3/random_board_1.in`
- output
  - 會存放解出來的盤面
    - 格式為 `output/<random_seed>_<size>x<size>/random_board_<turn>.out`
    - 如果種子碼為 123，盤面大小為 3x3，第 1 個盤面，則檔名為 `output/123_3x3/random_board_1.out`
- statistics
  - 用來統計 output 的結果
- test1
  - 用來測試一個盤面
- test1000
  - 用來測試 1000 個盤面
- generator.hpp
  - 用來生成隨機盤面
  - 建構時，可以傳入`盤面大小 (size)`, `種子碼 (seed) or 是否使用隨機種子碼 (use_random_seed)`
    - 生成盤面時，可以傳入 `print = true | false` 來決定是否印出盤面
- node.hpp
  - 紀錄盤面資訊，大致包含 (但不一定有用到)
  - `盤面 (state)`
    - `上個移動方向 (move)`
    - `審決值 (heuristic)`
    - `深度 (depth)`
    - `空格位置 (zero_index)`
- puzzle.hpp
  - 用來執行一場遊戲，大致包含
  - `讀取輸入 (get_input)`
  - `判斷輸入是否有誤 (verify_input)`
  - `判斷盤面是否有解 (is_solvable)`
  - `審決函式 (manhattan_distance)`
  - `生成下一個節點 (expand)`
- search.hpp
  - 用來搜尋最佳解，使用 iterative deepening search
  - 大致包含
    - `搜尋 (iterative_deepening_search_v2)`
    - `給出搜尋結果 (show_result)`

## test1000

### 功能

- 生成 n 個隨機盤面，並將輸入存在輸入資料夾
- 讀取資料夾中的 n 個盤面，得出解後將結果存在輸出資料夾
- 不會將解法盤面變化印出來 (怕太亂)，要去輸出檔內看

### 初始輸入

![image](https://hackmd.io/_uploads/S1OWcc066.png)

- 輸入盤面大小 (size)、要測試的盤面個數 (turns)
- 輸出 generator 的 seed 是多少，可供復現
- 會將 input 和 output 分別生在該兩個資料夾內

----

### 盤面不可解輸出

![image](https://hackmd.io/_uploads/HJ-O5qAT6.png)

- 輸出不可解
- 會將輸出存在 .out 檔中

----

### 盤面可解輸出

![image](https://hackmd.io/_uploads/SJX5q5RTa.png)

- 輸出路徑
- 最大節點數
- 所有探索過的節點數
- 時間
- 步數
- 詳細盤面變化會存在.out檔中，不然輸出太多了

----

![image](https://hackmd.io/_uploads/H1U_j5Aa6.png)

- .out檔中的盤面變化

----

### 跑完 n 個後的輸出

![image](https://hackmd.io/_uploads/Sy1WhcCpa.png)

- 總共時間
- 平均時間
- 有多少可解
- 多少不可解

----

## test1

### 功能

- 隨機生成一個盤面並解決
- 不會將輸入輸出存在資料夾內
- 會將解法盤面變化印出來

### 初始輸入

![image](https://hackmd.io/_uploads/BkVX-o0Tp.png)

- 輸入盤面大小 (size)、要測試的盤面個數 (turns)
- 輸出 generator 的 seed 是多少，可供復現

----

### 可解輸出 (同上，但會印出盤面變化)

![image](https://hackmd.io/_uploads/HyJLbiRaT.png)

----

## 搜尋法

使用 iterative deepening search，只要盤面可解，就一定可以解出來。
隨機到 4\*4 的最高步數為 66，花了 2888649 ms，大約一小時。

## 隨機盤面時間分析

### 3\*3

![image](https://hackmd.io/_uploads/Sy1WhcCpa.png)

隨機 1000 個盤面，其中有 497 個可解，503 個不可解。
解 497 個共花了 590 ms，平均一個花了 1.18712 ms。

----

### 4\*4

有花了快一天跑過一遍，但沒有截到圖，所以寫了 python 去 output 裡面統計資料

![image](https://hackmd.io/_uploads/HJb_mTAaT.png)

隨機 1000 個，其中有 508 個可解，492 不可解
解 508 共花了 73987599 ms = 73987 s = 20 hr
平均一個盤面花了145秒，大約兩分鐘

----

![image](https://hackmd.io/_uploads/rkJj46RTa.png)

其中的步數平均落在 53 上下，
55 步內幾乎都可以在一分鐘內解完，
有些步數比較少，時間花比較多的情況，
我想可能與搜尋順序相關，我的順序是右左下上，
或者是該狀況的剪枝剛好較多，我是用深度+曼哈頓距離>深度限制來進行剪枝的。

----

![image](https://hackmd.io/_uploads/SyDQVa0Ta.png)

- 步數與平均時間的圖表

----

## 隨機盤面節點分析

![image](https://hackmd.io/_uploads/BkzHQn0pT.png)

- 我原本用 int 存，所以看起來在 step >= 56 後有許多 state 數 overflow了

----

![image](https://hackmd.io/_uploads/SJshm6CTa.png)

- 把負的拿掉之後長這樣，但 step 大於 56 後，實際平均值應該更大

----

![image](https://hackmd.io/_uploads/rJvlS20a6.png)

- 大致呈指數成長

----
