CFLAGS	+= -I../../include -D_FILE_OFFSET_BITS=64 -Wall -W -Wdeclaration-after-statement
LDFLAGS	+= -lpthread

OBJS =	../../lib/entropy.o	../../lib/entropy_poll.o	../../lib/ctr_drbg.o \
../../lib/sha512.o	../../lib/aes.o	../../lib/bignum.o	../../lib/ecp.o \
../../lib/ecp_curves.o	../../lib/util.o	../../lib/bd1.o \
../../lib/bd2.o	../../lib/mka.o	../../lib/gdh2.o

all: bd1_test.o	bd2_test.o	mka_test.o	gdh2_test.o

bd1_test.o: bd1_test.c $(OBJS)
	$(CC) $(CFLAGS) bd1_test.c $(LDFLAGS) -o bd1_test.o $(OBJS)

bd2_test.o: bd2_test.c $(OBJS)
	$(CC) $(CFLAGS) bd2_test.c $(LDFLAGS) -o bd2_test.o $(OBJS)

mka_test.o: mka_test.c $(OBJS)
	$(CC) $(CFLAGS) mka_test.c $(LDFLAGS) -o mka_test.o $(OBJS)

gdh2_test.o: gdh2_test.c $(OBJS)
	$(CC) $(CFLAGS) gdh2_test.c $(LDFLAGS) -o gdh2_test.o $(OBJS)

clean:
	rm *.o
