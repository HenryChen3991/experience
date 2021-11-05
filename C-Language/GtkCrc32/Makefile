
EXE = gtkcrc32
OBJS = GtkCrc32.o
OBJS += crc32.o

all: $(EXE)

clean: 
	rm -rf *.[od]
#
# Set our CommEngine directory (by splitting the pwd into two words
# at /userspace and taking the first word only).
# Then include the common defines under CommEngine.
#
CURR_DIR :=$(shell pwd)

CFLAGS += -I./
CFLAGS += -Wall
CFLAGS += -s
CFLAGS += -O2

LDFLAGS += -L.
LDFLAGS += -lm


#
# Implicit rule will make the .c into a .o
# Implicit rule is $(CC) -c $(CPPFLAGS) $(CFLAGS)
# See Section 10.2 of Gnu Make manual
#
$(EXE): $(OBJS)
	$(CC) -o $(EXE) $(CFLAGS) $(OBJS) $(LDFLAGS)


#
# Include the rule for making dependency files.
# The '-' in front of the second include suppresses
# error messages when make cannot find the .d files.
# It will just regenerate them.
# See Section 4.14 of Gnu Make.
#

-include $(OBJS:.o=.d)

