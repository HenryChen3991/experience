OBJ = omci2pcap.o
APP = omci2pcap
LIBS = -lgtk_string

CURR_DIR := $(shell pwd)
GTK_STRING_LIB_PATH := $(CURR_DIR)/../../GTK_UTIL/GTK_STRING_LIB

ALLOWED_INCLUDE_PATHS := -I. \
						 -I$(CURR_DIR)/../../DEBUG \
						 -I$(GTK_STRING_LIB_PATH) \

LIBS_PATH := -L.\
			 -L$(GTK_STRING_LIB_PATH) \

all: libs_build $(APP)

$(APP): $(OBJ)
	$(CC) -o $@ $(OBJ) -lpthread -Wl,-rpath,$(LIBS_PATH) $(LIBS_PATH) $(LIBS)

%.o: %.c
	$(CC) $< -c $(ALLOWED_INCLUDE_PATHS)

libs_build:
	make -C $(GTK_STRING_LIB_PATH)

clean:
	rm -f *.o $(APP)
	cd $(GTK_STRING_LIB_PATH)
	make -C $(GTK_STRING_LIB_PATH) clean
	