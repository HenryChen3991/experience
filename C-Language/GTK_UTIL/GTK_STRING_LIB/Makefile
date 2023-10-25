LIB := libgtk_string.a
OBJ = gtk_string.o
SRCS = gtk_stringlib.c

CURR_DIR := $(shell pwd)
ALLOWED_INCLUDE_PATHS := -I. \
						 -I$(CURR_DIR)/../../DEBUG \

TR="AA11"
TRR=`echo ${TR} |  tr A-Z a-z`
TRa=`${$(TR),,}`
.PHONY: all

all: $(LIB)
	echo "TRR : ${TRR}"
	echo "TRa : ${TRa}"

$(LIB): $(OBJ)
	ar -rcs $(LIB) $(OBJ)
	
%.o: %.c
	$(CC) $< -c $(ALLOWED_INCLUDE_PATHS)
	
clean:
	rm -f *.o $(LIB)
