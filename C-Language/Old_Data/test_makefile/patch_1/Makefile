#2018/2/1
#Makefile-patch-1
#not flexiable ,because on line 13~16 are not flexiable

src = $(wildcard *.c)
obj = $(patsubst %.c,%.o,$(src))
exe = $(patsubst %.c,%,$(src))
cc = gcc

all:$(exe)
%.o:%.c
	$(cc) -c $< -o $@
pointerFunction:pointerFunction.o
	$(cc) $^ -o $@
Advanced_pointerFunction:Advanced_pointerFunction.o
	$(cc) $^ -o $@
src:
	@echo $(src)
obj:
	@echo $(obj)
exe:
	@echo $(exe)
clean:
	@echo "clear $(obj)"
	@echo "clear $(exe)"
	rm -rf $(obj) $(exe)
