#2018/2/2
#Makefile:
#	compile dir src file ,and then check whether dir obj and bin are exist or not
#	if not ,then mkdir obj or bin 

#declare variable
MFlag= y
src= $(wildcard ./src/*.c)
obj= $(patsubst ./src/%.c, ./obj/%.o, $(src))
bin= $(patsubst ./src/%.c, ./bin/%, $(src))
c_Files = $(shell ls ./src/*.c)
#compile variable
cc= gcc

#avoid auto delete .o file
.SUFFIXS: .c .o
.PRECIOUS: ./obj/%.o

all:check $(bin)
check:
	@if [ ! -d "obj" ];then\
	    mkdir obj;\
	fi
	@if [ ! -d "bin" ];then\
	    mkdir bin;\
	fi
./bin/%:./obj/%.o
	$(cc) $^ -o $@ 
./obj/%.o:./src/%.c
	$(cc) -DMFlag -c $< -o $@

#example test only build pointerFunction.c
bulid_pointerFuction:./src/pointerFunction.c
	$(cc) -c ./src/pointerFunction.c -o pointerFunction.o

#print some useful info
print_src:
	@echo $(src)
print_obj:
	@echo $(obj)
print_bin:
	@echo $(bin)
print_cFiles:
	@echo $(c_Files)
clean:
	@echo "clean"
	rm -rf ./bin/*
	rm -rf $(obj) $(bin)
