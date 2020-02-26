ARMGNU = arm-none-eabi
COPS = -I../include -ggdb -Wall -O2 -nostdlib -nostartfiles -ffreestanding
LIBGCC = /home/dario/gcc-arm-none-eabi-4_8-2014q2/lib/gcc/arm-none-eabi/4.8.4/armv7-m/libgcc.a

CFLAGS  += -I../include -c -D_FILE_OFFSET_BITS=64 -Wall -W -Wdeclaration-after-statement

OBJS =	entropy.o	entropy_poll.o	ctr_drbg.o	sha512.o	aes.o	bignum.o	ecp.o	ecp_curves.o

stm32: entropy.gcc.thumb.o	entropy_poll.gcc.thumb.o	ctr_drbg.gcc.thumb.o \
sha512.gcc.thumb.o	aes.gcc.thumb.o	bignum.gcc.thumb.o	ecp.gcc.thumb.o \
ecp_curves.gcc.thumb.o	platform.gcc.thumb.o	memory_buffer_alloc.gcc.thumb.o \
stm32f4d_helper.gcc.thumb.o	util.gcc.thumb.o bd1.gcc.thumb.o \
bd2.gcc.thumb.o	mka.gcc.thumb.o		gdh2.gcc.thumb.o \
KnxTelegram.gcc.thumb.o	KnxTpUart.gcc.thumb.o

entropy_poll.gcc.thumb.o: entropy_poll.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c entropy_poll.c -o entropy_poll.gcc.thumb.o
entropy.gcc.thumb.o: entropy.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c entropy.c -o entropy.gcc.thumb.o
sha512.gcc.thumb.o: sha512.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c sha512.c -o sha512.gcc.thumb.o
aes.gcc.thumb.o: aes.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c aes.c -o aes.gcc.thumb.o
ctr_drbg.gcc.thumb.o: ctr_drbg.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c ctr_drbg.c -o ctr_drbg.gcc.thumb.o
bignum.gcc.thumb.o: bignum.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c bignum.c -o bignum.gcc.thumb.o
ecp.gcc.thumb.o: ecp.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c ecp.c -o ecp.gcc.thumb.o
ecp_curves.gcc.thumb.o: ecp_curves.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c ecp_curves.c -o ecp_curves.gcc.thumb.o
platform.gcc.thumb.o: platform.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c platform.c -o platform.gcc.thumb.o
memory_buffer_alloc.gcc.thumb.o: memory_buffer_alloc.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c memory_buffer_alloc.c -o memory_buffer_alloc.gcc.thumb.o
stm32f4d_helper.gcc.thumb.o: stm32f4d_helper.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c stm32f4d_helper.c -o stm32f4d_helper.gcc.thumb.o
util.gcc.thumb.o: util.c $(OBJS2)
	$(ARMGNU)-gcc $(COPS) -mthumb -c util.c -o util.gcc.thumb.o
bd1.gcc.thumb.o: bd1.c $(OBJS2)
	$(ARMGNU)-gcc $(COPS) -mthumb -c bd1.c -o bd1.gcc.thumb.o
bd2.gcc.thumb.o: bd2.c $(OBJS2)
	$(ARMGNU)-gcc $(COPS) -mthumb -c bd2.c -o bd2.gcc.thumb.o
mka.gcc.thumb.o: mka.c $(OBJS2)
	$(ARMGNU)-gcc $(COPS) -mthumb -c mka.c -o mka.gcc.thumb.o
gdh2.gcc.thumb.o: gdh2.c $(OBJS2)
	$(ARMGNU)-gcc $(COPS) -mthumb -c gdh2.c -o gdh2.gcc.thumb.o
KnxTelegram.gcc.thumb.o: KnxTelegram.c 
	$(ARMGNU)-gcc $(COPS) -mthumb -c KnxTelegram.c -o KnxTelegram.gcc.thumb.o
KnxTpUart.gcc.thumb.o: KnxTpUart.c stm32f4d_helper.gcc.thumb.o
	$(ARMGNU)-gcc $(COPS) -mthumb -c KnxTpUart.c -o KnxTpUart.gcc.thumb.o


local: aes.o	bignum.o	ctr_drbg.o	ecp.o	ecp_curves.o \
entropy.o	entropy_poll.o	sha512.o	util.o	bd1.o	bd2.o	mka.o	gdh2.o

aes.o: aes.c
	$(CC) $(CFLAGS) aes.c $(LDFLAGS) -o aes.o
bignum.o: bignum.c
	$(CC) $(CFLAGS)  bignum.c $(LDFLAGS) -o bignum.o
ctr_drbg.o: ctr_drbg.c
	$(CC) $(CFLAGS) ctr_drbg.c $(LDFLAGS) -o ctr_drbg.o
ecp.o: ecp.c
	$(CC) $(CFLAGS) ecp.c $(LDFLAGS) -o ecp.o
ecp_curves.o: ecp_curves.c
	$(CC) $(CFLAGS) ecp_curves.c $(LDFLAGS) -o ecp_curves.o
entropy.o: entropy.c
	$(CC) $(CFLAGS) entropy.c $(LDFLAGS) -o entropy.o
entropy_poll.o: entropy_poll.c
	$(CC) $(CFLAGS) entropy_poll.c $(LDFLAGS) -o entropy_poll.o
sha512.o: sha512.c
	$(CC) $(CFLAGS) sha512.c $(LDFLAGS) -o sha512.o
util.o: util.c
	$(CC) $(CFLAGS) util.c $(LDFLAGS) -o util.o
bd1.o: bd1.c
	$(CC) $(CFLAGS) bd1.c $(LDFLAGS) -o bd1.o
bd2.o: bd2.c
	$(CC) $(CFLAGS) bd2.c $(LDFLAGS) -o bd2.o
mka.o: mka.c
	$(CC) $(CFLAGS) mka.c $(LDFLAGS) -o mka.o
gdh2.o: gdh2.c
	$(CC) $(CFLAGS) gdh2.c $(LDFLAGS) -o gdh2.o

clean: 
	rm *.o
