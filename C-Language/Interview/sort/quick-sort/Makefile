#ME_9_3_OBJS = me_9_3_13.o
#ME_9_12_OBJS = me_9_12_2.o
#ME_65305_OBJS = me_65305.o

#OBJ = omciparser.o omci.o omci_init.o omci_util.o $(ME_9_12_OBJS) $(ME_9_3_OBJS) $(ME_65305_OBJS)
#APP = omciparser
#LIBS = -lgtk_string

APP = quick-sort
OBJ = quick-sort.o
LIBS = -lgtk_string

CURR_DIR := $(shell pwd)
GTK_STRING_LIB_PATH := $(CURR_DIR)/../../../GTK_UTIL/GTK_STRING_LIB

ALLOWED_INCLUDE_PATHS := -I. \
			 -I$(CURR_DIR)/../../../DEBUG \
			 -I$(GTK_STRING_LIB_PATH) \

LIBS_PATH := -L.\
			 -L$(GTK_STRING_LIB_PATH) \

all: libs_build $(APP)

$(APP): $(OBJ)
	$(CC) -o $@ $(OBJ) -lpthread -Wl,-rpath,$(LIBS_PATH) $(LIBS_PATH) $(LIBS) -save-temps

%.o: %.c
	$(CC) $< -c $(ALLOWED_INCLUDE_PATHS)

libs_build:
	make -C $(GTK_STRING_LIB_PATH)

clean:
	rm -f *.o $(APP)
	cd $(GTK_STRING_LIB_PATH)
	make -C $(GTK_STRING_LIB_PATH) clean
	
