all:omci2pcap

CURR_DIR := $(shell pwd)
ALLOWED_INCLUDE_PATHS := -I. \
						 -I$(CURR_DIR)/../../DEBUG \

omci2pcap:omci2pcap.c
	gcc omci2pcap.c -o omci2pcap $(ALLOWED_INCLUDE_PATHS)

clean:
	rm omci2pcap