# Linux Awk Command
##### 演示資料(data.txt)

        ID  name    gender  age  email          phone
        1   Bob     male    28   abc@qq.com     18023394012
        2   Alice   female  24   def@gmail.com  18084925203
        3   Tony    male    21   aaa@163.com    17048792503
        4   Kevin   male    21   bbb@189.com    17023929033
        5   Alex    male    18   ccc@xyz.com    18185904230
        6   Andy    female  22   ddd@139.com    18923902352
        7   Jerry   female  25   exdsa@189.com  18785234906
        8   Peter   male    20   bax@qq.com     17729348758
        9   Steven  female  23   bc@sohu.com    15947893212
        10  Bruce   female  27   bcbd@139.com   13942943905

#### 讀取文件的幾種方式

	1.按字符數量讀取: 
		每一次可以讀取一個字符，或者多個字符，直到把文件讀取完
	2.按照分隔符號進行讀取:
		一直讀取直到遇到了分隔符號才停止，下次繼續從分隔符號的位置向後讀取，直到讀完整個文件
	3.按行讀取:
		每次讀取一行，直到讀完整個文件
			是按照分隔符號進行讀取的一種特殊情況，**將分隔符號指定為患行符號(\n)**
	4.一次性讀取整個文件
	5.按照字符數量讀取:
		一次讀取指定數量的字符數，直到把文件讀完

#### 透過"Read"指令來讀取文件
##### 1. 按字符数量读取

```bash
# 正确
while read -N 3 data;do
  echo "$data"
done <data.txt


# 错误
while read -N 3 data < data.txt;do
  echo "$data"
done

```
##### 2. 按分隔符读取
```bash
while read -d "m" data ;do
  echo "$data"
done <a.txt
```
##### 3. 按行读取
```bash
while read line;do
  echo "$line"
done <a.txt
```
##### 3. 一次性读整个文件
- 按照字符数量读取，且指定的字符数要大于文件的总大小
- 按分隔符读取，且指定的分隔符是文件中不存在的字符，这样的话会一直读取，因为找不到分隔符而读完整个文件

```bash
# 指定超出文件大小的字符数量
read -N 1000000 data <a.txt
echo "$data"

# 指定文件中不存在的字符作为分隔符
read -d "_" data <a.txt
echo "$data"
```



