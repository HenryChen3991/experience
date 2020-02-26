all: main

main:main.o util.o
	gcc -o $@ $^

main.o:main.c util.h
	gcc -c $< -o $@
util.o:util.c util.h
	gcc -c $< -o $@

clean:
	rm -rf main.o main util.o
