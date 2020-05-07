OBJ = ckl_ht.o ckl_hashtable.o ckl_util.o
APP = ckl_ht

all: $(APP)
$(APP): $(OBJ) ckl_debug.h
	$(CC) -o $@ $(OBJ) -lpthread

%.o: %.c
	$(CC) $< -c
clean:
	rm -f *.o $(APP)

