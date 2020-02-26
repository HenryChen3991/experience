在Linux下面的C語言可以透過getopt去parse從command line傳過來的參數

使用方式基本上就是重複讀取getopt回傳option字元直到回傳-1。

請注意：這只是蓋略說明，很多細節仍然需要參考manpage，例如哪些是Gnu的特別功能還有一些細節的參數操作等。

進一步細節如下：

參數以及變數說明

參數
	int argc: main的argument count
	
	char **argv: main的argument vector

	const char optstring: 你的程式要支援的option及格式

		option以-開頭，後面為單一個字元，可接參數
		
		格式			
			單一字元
			單一字元後接:
				該option 支援參數
			單一字元後接::
				該option 可接參數也可不接
				ex: option -a, 參數arg1，下命令會是./cmd -aarg1
	範例: 支援-a, -b, -c 參數, -d 參數，-e optional 參數
		"abc:d:e::"
		
變數 (僅列舉)
	optopt
		如果遇到不支援的option或是不合語法的option操作，getopt會回傳?，然後把出問題的option放在optopt這個變數內
	optind
		option index，表示getopt下一個要處理的參數。當getopt回傳-1後記得比較optind和argc看看還有沒有非option的參數要處理
		範例 ./cmd -a -b 123 456，跑完getopt後還有123 456要處理。



		
Code

include <unistd.h>  /* getopt */
#include <stdio.h>

int main(int argc, char **argv)
{
    int cmd_opt = 0;

    fprintf(stderr, "argc:%d\n", argc);
    while(1) {
        fprintf(stderr, "proces index:%d\n", optind);
        cmd_opt = getopt(argc, argv, "abc:d:e::");

        /* End condition always first */
        if (cmd_opt == -1) {
            break;
        }

        /* Print option when it is valid */
        if (cmd_opt != '?') {
            fprintf(stderr, "option:-%c\n", cmd_opt);
        }

        /* Lets parse */
        switch (cmd_opt) {
            /* No args */
            case 'a':
            case 'b':
                break;

            /* Single arg */
            case 'c':
            case 'd':
                fprintf(stderr, "option arg:%s\n", optarg);
                break;

            /* Optional args */
            case 'e':
                if (optarg) {
                    fprintf(stderr, "option arg:%s\n", optarg);
                }
                break;


            /* Error handle: Mainly missing arg or illegal option */
            case '?':
                fprintf(stderr, "Illegal option:-%c\n", isprint(optopt)?optopt:'#');
                break;
            default:
                fprintf(stderr, "Not supported option\n");
                break;
        }
    }

    /* Do we have args? */
    if (argc > optind) {
        int i = 0;
        for (i = optind; i < argc; i++) {
            fprintf(stderr, "argv[%d] = %s\n", i, argv[i]);
        }
    }

    return 0;
}


Result:

$ ./test_getopt -a -emy_args -c dddd  123 456 7890 -b -f -d
argc:11
proces index:1
option:-a
proces index:2
option:-e
option arg:my_args
proces index:3
option:-c
option arg:dddd
proces index:5
option:-b
proces index:9
./test_getopt: invalid option -- 'f'
Illegal option:-f
proces index:10
./test_getopt: option requires an argument -- 'd'
Illegal option:-d
proces index:11
argv[8] = 123
argv[9] = 456
argv[10] = 7890

