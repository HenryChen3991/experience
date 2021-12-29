LIB := libgtk_string.a
OBJ = gtk_string.o
SRCS = gtk_stringlib.c

.PHONY: all

all: $(LIB)

$(LIB): $(OBJ)
	ar -rcs $(LIB) $(OBJ)
	
%.o: %.c
	$(CC) $< -c $(ALLOWED_INCLUDE_PATHS)
	
clean:
	rm -f *.o $(LIB)