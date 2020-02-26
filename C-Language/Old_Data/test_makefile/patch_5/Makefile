#2018/2/2
#Makefile:
#	compile dir src file ,and then check whether dir obj and bin are exist or not
#	if not ,then mkdir obj or bin 

#declare variable
src:= $(wildcard ./src/*.c)
obj:= $(patsubst ./src/%.c, ./obj/%.o, $(src))
bin:= $(patsubst ./src/%.c, ./bin/%, $(src))
c_Files:= $(shell ls ./src/*.c)
$(shell if [ ! -d "obj" ];then\
	    mkdir obj;\
		mkdir aa;\
	fi
	@if [ ! -d "bin" ];then\
	    mkdir bin;\
	fi
	@if [ ! -d "src" ];then\
	    mkdir src;\
            cp *.c ./src;\
	    rm -rf *.c;\
	fi)
#compile variable
cc= gcc

#avoid auto delete .o file
.SUFFIXS: .c .o
.PRECIOUS: ./obj/%.o

all:  

check:
	@if [ ! -d "obj" ];then\
	    mkdir obj;\
	fi
	@if [ ! -d "bin" ];then\
	    mkdir bin;\
	fi
	@if [ ! -d "src" ];then\
	    mkdir src;\
            cp *.c ./src;\
	    rm -rf *.c;\
	fi
p_src: src= $(wildcard ./src/*.c)

p_obj: obj= $(patsubst ./src/%.c, ./obj/%.o, $(src))

p_bin: bin= $(patsubst ./src/%.c, ./bin/%, $(src))

./bin/%:./obj/%.o
	$(cc) $^ -o $@
./obj/%.o:./src/%.c
	$(cc) -c $< -o $@

#example test only build pointerFunction.c
bulid_pointerFuction:./src/pointerFunction.c
	$(cc) -c ./src/pointerFunction.c -o pointerFunction.o

#print some useful info
print_src:
	@echo $(src)
	@echo "bin = $(bin)"
print_obj:
	@echo $(obj)
print_bin:
	@echo $(bin)
print_cFiles:
	@echo $(c_Files)
clean:
	@echo "clean"
	rm -rf $(obj) $(bin)
