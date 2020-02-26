#2018/2/2
#Makefile:
#	compile dir src file ,and then check whether dir obj and bin are exist or not
#	if not ,then mkdir obj or bin 

#declare variable
src= $(wildcard ./src/*.c)
obj= $(patsubst ./src/%.c, ./obj/%.o, $(src))
bin= $(patsubst ./src/%.c, ./bin/%, $(src))
c_Files = $(shell ls ./src/*.c)
VER=TB-251_v0.0.0.1
sp= $(subst _, ,$(VER))
VER_PRE= $(word 1,$(sp))
VER_LAST= $(word 2,$(sp))
aa=aa1
#compile variable
cc= gcc

#avoid auto delete .o file
.SUFFIXS: .c .o
.PRECIOUS: ./obj/%.o

#test_EX1
#the sets of dirs to do various things in
BUILDDIRS = $(DIRS:%=build-%)
INSTALLDIRS = $(DIRS:%=install-%)
CLEANDIRS = $(DIRS:%=clean-%)

all: test_EX1 check $(bin)
test_EX1:
	@echo "BUILDDIRS : $(BUILDDIRS)"
	@echo "INSTALLDIRS : $(INSTALLDIRS)"
	@echo "CLEANDIRS : $(CLEANDIRS)"

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
	$(cc) -c $< -o $@

#example test only build pointerFunction.c
#bulid_pointerFuction:./src/pointerFunction.c
#	$(cc) -c ./src/pointerFunction.c -o pointerFunction.o

bulid_pointerFuction:./src/pointerFunction.c
	

#print some useful info
print_src:
	@echo $(src)
print_obj:
	@echo $(obj)
print_bin:
	@echo $(bin)
print_cFiles:
	@echo $(c_Files)
	@echo $(aa)
VER:
	@echo $(VER)
	@echo $(VER_PRE)
	@echo $(VER_LAST)
clean:
	@echo "clean"
	rm -rf $(obj) $(bin)
