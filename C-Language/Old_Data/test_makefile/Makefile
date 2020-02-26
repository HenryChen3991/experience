#2018/2/1
#Makefile
#normal use
#example:
#we have add.c sub.c mul.c then we wnat to produce add sub mul 
src = $(wildcard *.c)
obj = $(patsubst %.c,%.o,$(src))
exe = $(patsubst %.c,%,$(src))
cc = gcc

all:$(exe)

%:%.o
	@echo "gcc file.o -o file"
	$(cc) $^ -o $@
%.o:%.c
	@echo "gcc -c file.c -o file.o"
	@echo "1111"
	$(cc) -c $< -o $@
print_src:
	@echo $(src)
print_obj:
	@echo $(obj)
print_exe:
	@echo $(exe)
clean:
	@echo "clear obj files : $(obj)"
	@echo "clear exe files : $(exe)"
	rm -rf $(obj) $(exe)



