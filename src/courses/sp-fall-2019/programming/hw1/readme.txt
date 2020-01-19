我將這次作業分成兩個階段解決：

（1） IO Multiplexing
    * 卡關1：未將server的fd考慮進IO Multiplexing，因此我的程式會卡在accept這個函數，而無法去處理其他要求
    * 卡關2：沒注意到在call select的時候，timeout可能會被改變 -> solution: 復制一份tmp_timeout
    * 優化1：原本我對3~maxfd進行select，後來改成只檢查到當下開啓的最大fd
    * 優化2：將server及每一個connection都設成nonblocking mode，雖然在select的控制下理應不會被block，但因爲教授在上課時說建議兩者都做，所以我仍然將它們設爲nonblocking
    * 優化3: 我本來每次select都只取一個最小可讀的fd做處理，其他留到下次select，雖然在query不頻繁的情況沒有問題，但當concurrency增加，這樣理論上很沒有效率，因此我便改爲
            一次性對所有可讀的fd做處理

    # 函數 void add_read_set(int fd): 將fd加到master_set
    # 函數 fd_set do_select(): 回傳select處理過的fd_set
    
（2） Requests Handling
    * 卡關1：未注意到同一個process是無法利用fcntl得知自己之前是否有對一個byte上lock -> 解決辦法：利用一個array去記憶現在一個id是否有被鎖住
    * 對於寫入動作兩個階段的處理：我使用一個array (wait_for_cmd) 記錄現在一個connection是在哪一個階段。由於當我select到一個不是server的fd時，我都會先把該fd從master_set中移除，
      因此若是第一階段的write query，我會在處理結束後再次將它加到master_set
    * 我利用 F_SETLK 這個 fcntl command 來使檢查有沒有lock和上lock是一個atomic operation，如此一來就可以避免在這兩者之間發生context switch的情況

    # 函數 void query_for_read(request *req): 處理read query
    # 函數 query_for_write(request *req): 處理write_query，如果該connection是第二階段，則呼叫do_action後return
    # 函數 do_action(request *req): 處理第二階段的write_query，包含對account_list的各種寫入操作

