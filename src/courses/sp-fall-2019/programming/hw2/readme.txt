Execution:
    (1) Compilation: $ make
    (2) Run: $ ./bidding_system [host_num] [player_num]

    * NOTE: Please ensure that there are no any .FIFO files in working directory, or it will exit and print error message

Description:
    (1) bidding_system.c:
        * algorithm to enumerate all subset of size 8 on the player set: 
            一開始，我將 int set[8] 初始化成 {1, 2, 3, 4, 5, 6, 7, 8} 並在每次需要assign新的coompetition給root_host時，
            函數 next_subset() 會將 set[] 改成字典序比原本的subset大的最小subset。具體實作方式爲從 i = 7 開始往前掃，如果
            把 set[i] + 1 不超過 player_num，且 set[i] + 1 ... player_num 足夠填完 set[i...7]，就令 set[i] = set[i] + 1
            ，並令set[i+1] = set[i] + 1, set[i+2] = set[i+1] + 1...以此類推。如此以來，我就能得到字典序比原本的subset大的最小subset。
            若找不到這樣的subset，也就是當前的subset已經是字典序最大的subset了，next_subset()回傳-1,我就知道已經枚舉完 C(player_num, 8)
            種不同的subset了。

        * create FIFO:
            我在把每一個FIFO成功open之後，便立刻unlink掉他們，確保即使程式crash了，也不會遺留這些檔案。

        * avoid deadlock when opening FIFO:
            因爲在blocking mode下open FIFO時，如果對面沒有reader/writer，便會被block住，因此，我在執行open() + exec()後才能去open這些
            FIFO，如此以來，如果其中一邊被block住了，系統便會context switch到另一個process，此時因爲他已經能看到reader/writer，所以就不會
            被block住。最後在切換會本來被block住的process，就能使parent和child都順利開啓FIFO。

        * output rank of each player:
            因爲在排序後我們仍然要知道每一個player本來的index，所以我對pointer排序，這樣就可以透過pointer的value得知原本在array中的位置。

    (2) host.c:
        * pipe
            我在host建pipe的策略是對於每一個不同的child、write/read都各自建立一個獨立的pipe，我認爲這樣可以減少發生race condition的可能性。
        
        * function sublist(int list[], int L, int R, char buf[]):
            將list[]中 [L, R) 的數字轉換爲字串寫到buffer裏，方便我建構給child的訊息。

    (3) player.c:
        * 由於對於player來說，所有讀寫都是在standard input/output完成的，因此只需要簡單的scanf/printf就能達成我們的要求
