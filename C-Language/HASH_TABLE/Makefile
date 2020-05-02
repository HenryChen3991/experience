OBJ = ckl_ht.o ckl_hashtable.o
APP = ckl_ht

all: $(APP)
$(APP): $(OBJ)
	$(CC) -o $@ $(OBJ) -lpthread

%.o: %.c
	$(CC) $< -c
%.c: %.h
	
clean:
	rm -f *.o $(APP)

