ARMGNU = arm-none-eabi

COPS = -I../../include -ggdb -Wall -nostdlib -nostartfiles -ffreestanding

OBJS = ../../lib/entropy.gcc.thumb.o	../../lib/entropy_poll.gcc.thumb.o	\
../../lib/ctr_drbg.gcc.thumb.o	../../lib/sha512.gcc.thumb.o	../../lib/aes.gcc.thumb.o	\
../../lib/bignum.gcc.thumb.o	../../lib/ecp.gcc.thumb.o \
../../lib/ecp_curves.gcc.thumb.o	../../lib/platform.gcc.thumb.o	\
../../lib/memory_buffer_alloc.gcc.thumb.o	../../lib/stm32f4d_helper.gcc.thumb.o \
../../lib/util.gcc.thumb.o	../../lib/KnxTelegram.gcc.thumb.o \
../../lib/KnxTpUart.gcc.thumb.o ../../lib/bd1.gcc.thumb.o	\
../../lib/bd2.gcc.thumb.o	../../lib/mka.gcc.thumb.o	../../lib/gdh2.gcc.thumb.o

LIBGCC = /home/dario/gcc-arm-none-eabi-4_8-2014q2/lib/gcc/arm-none-eabi/4.8.4/armv7-m/libgcc.a

STLINK = /home/dario/stlink

all: mka_test.gcc.thumb.bin	bd1_test.gcc.thumb.bin	bd2_test_root.gcc.thumb.bin \
bd2_test_node.gcc.thumb.bin	gdh2_test1.gcc.thumb.bin	gdh2_test2.gcc.thumb.bin \


vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o

mka_test.gcc.thumb.o: mka_test.c $(OBJS)
	$(ARMGNU)-gcc $(COPS) -mthumb -c mka_test.c -o mka_test.gcc.thumb.o
mka_test.gcc.thumb.bin : memmap vectors.o mka_test.gcc.thumb.o $(OBJS)
	$(ARMGNU)-ld -o mka_test.gcc.thumb.elf -T memmap vectors.o mka_test.gcc.thumb.o  $(OBJS) $(LIBGCC)
	$(ARMGNU)-objcopy mka_test.gcc.thumb.elf mka_test.gcc.thumb.bin -O binary

bd1_test.gcc.thumb.o: bd1_test.c $(OBJS)
	$(ARMGNU)-gcc $(COPS) -mthumb -c bd1_test.c -o bd1_test.gcc.thumb.o
bd1_test.gcc.thumb.bin : memmap vectors.o bd1_test.gcc.thumb.o $(OBJS)
	$(ARMGNU)-ld -o bd1_test.gcc.thumb.elf -T memmap vectors.o bd1_test.gcc.thumb.o  $(OBJS) $(LIBGCC)
	$(ARMGNU)-objcopy bd1_test.gcc.thumb.elf bd1_test.gcc.thumb.bin -O binary

bd2_test_root.gcc.thumb.o: bd2_test_root.c $(OBJS)
	$(ARMGNU)-gcc $(COPS) -mthumb -c bd2_test_root.c -o bd2_test_root.gcc.thumb.o
bd2_test_root.gcc.thumb.bin : memmap vectors.o bd2_test_root.gcc.thumb.o $(OBJS)
	$(ARMGNU)-ld -o bd2_test_root.gcc.thumb.elf -T memmap vectors.o bd2_test_root.gcc.thumb.o  $(OBJS) $(LIBGCC)
	$(ARMGNU)-objcopy bd2_test_root.gcc.thumb.elf bd2_test_root.gcc.thumb.bin -O binary

bd2_test_node.gcc.thumb.o: bd2_test_node.c $(OBJS)
	$(ARMGNU)-gcc $(COPS) -mthumb -c bd2_test_node.c -o bd2_test_node.gcc.thumb.o
bd2_test_node.gcc.thumb.bin : memmap vectors.o bd2_test_node.gcc.thumb.o $(OBJS)
	$(ARMGNU)-ld -o bd2_test_node.gcc.thumb.elf -T memmap vectors.o bd2_test_node.gcc.thumb.o  $(OBJS) $(LIBGCC)
	$(ARMGNU)-objcopy bd2_test_node.gcc.thumb.elf bd2_test_node.gcc.thumb.bin -O binary

gdh2_test1.gcc.thumb.o: gdh2_test1.c $(OBJS)
	$(ARMGNU)-gcc $(COPS) -mthumb -c gdh2_test1.c -o gdh2_test1.gcc.thumb.o
gdh2_test1.gcc.thumb.bin : memmap vectors.o gdh2_test1.gcc.thumb.o $(OBJS)
	$(ARMGNU)-ld -o gdh2_test1.gcc.thumb.elf -T memmap vectors.o gdh2_test1.gcc.thumb.o  $(OBJS) $(LIBGCC)
	$(ARMGNU)-objcopy gdh2_test1.gcc.thumb.elf gdh2_test1.gcc.thumb.bin -O binary

gdh2_test2.gcc.thumb.o: gdh2_test2.c $(OBJS)
	$(ARMGNU)-gcc $(COPS) -mthumb -c gdh2_test2.c -o gdh2_test2.gcc.thumb.o
gdh2_test2.gcc.thumb.bin : memmap vectors.o gdh2_test2.gcc.thumb.o $(OBJS)
	$(ARMGNU)-ld -o gdh2_test2.gcc.thumb.elf -T memmap vectors.o gdh2_test2.gcc.thumb.o  $(OBJS) $(LIBGCC)
	$(ARMGNU)-objcopy gdh2_test2.gcc.thumb.elf gdh2_test2.gcc.thumb.bin -O binary

burn_bd1: bd1_test.gcc.thumb.bin
	$(STLINK)/st-flash write bd1_test.gcc.thumb.bin 0x8000000
burn_bd2_root: bd2_test_root.gcc.thumb.bin
	$(STLINK)/st-flash write bd2_test_root.gcc.thumb.bin 0x8000000
burn_bd2_node: bd2_test_node.gcc.thumb.bin
	$(STLINK)/st-flash write bd2_test_node.gcc.thumb.bin 0x8000000
burn_gdh2_test1: gdh2_test1.gcc.thumb.bin
	$(STLINK)/st-flash write gdh2_test1.gcc.thumb.bin 0x8000000
burn_gdh2_test2: gdh2_test2.gcc.thumb.bin
	$(STLINK)/st-flash write gdh2_test2.gcc.thumb.bin 0x8000000
burn_mka: mka_test.gcc.thumb.bin
	$(STLINK)/st-flash write mka_test.gcc.thumb.bin 0x8000000

size:
	size *.elf

clean:
	rm *.o
	rm *.elf
	rm *.bin
