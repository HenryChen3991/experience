src= $(wildcard ./src/*.c)
obj= $(patsubst ./src/%.c, ./obj/%.o, $(src))
bin= $(patsubst ./src/%.c, ./bin/%, $(src))

cc= gcc
all:$(bin)
./bin/%:./obj/%.o
	$(cc) $^ -o $@

./obj/%.o:./src/%.c
	$(cc) -c $< -o $@

print_src:
	@echo $(src)
print_obj:
	@echo $(obj)
print_bin:
	@echo $(bin)
clean:
	@echo "clean"
	rm -rf $(obj) $(bin)
