# Shell Script

* EX1
	* 從600組證書(600-cert)當中過濾出哪50組證書是尚未被使用過的
	* log.txt已經被使用的名單
	* 利用grep技巧，檢查檔案中是否有指定的字串
		*  grep -q $search_string $file
		*  返回值:
			*   0 : 找到該字串
			*   1 : 找不到該字串
	* ![imageEX1_01](./figures/EX1_01.png)