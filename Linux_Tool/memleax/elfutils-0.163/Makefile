#***********************************************************************
#
#  Copyright (c) 2012  Broadcom Corporation
#  All Rights Reserved
#
#***********************************************************************/

all dynamic install: conditional_build

#
# Set our CommEngine directory (by splitting the pwd into two words
# at /userspace and taking the first word only).
# Then include the common defines under CommEngine.
# You do not need to modify this part.
#
CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /userspace, /userspace,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))

include $(BUILD_DIR)/make.common

UNTAR_COMPLETE_FILE = .untar_complete
CONFIG_COMPLETE_FILE = config_complete

APP = elfutils-0.163
LIBASM := libasm
LIBDW := libdw
LIBELF := libelf

ifneq ($(strip $(BUILD_MPTCP)),)

$(APP)/$(UNTAR_COMPLETE_FILE): $(APP).tar.bz2 Makefile
	rm -rf $(APP)
	(tar xvf $(APP).tar.bz2 2> /dev/null || true);
	patch -p1 -b -s -d $(APP)/libelf < test.patch;
	@echo "$(APP) is untarred"
	touch $(APP)/$(UNTAR_COMPLETE_FILE)

$(APP)/$(CONFIG_COMPLETE_FILE): $(APP)/$(UNTAR_COMPLETE_FILE) 
	(cd $(APP); ./configure \
		--host=arm-linux-gnueabihf \
		--prefix=$(BCM_FSBUILD_DIR)/public/elfutils-0.163 \
	    CFLAGS=-s\ -Os\ -Wno-strict-aliasing || exit 1);
	@echo "$(APP) is configured"
	touch $(APP)/$(CONFIG_COMPLETE_FILE)

conditional_build: $(APP)/$(CONFIG_COMPLETE_FILE) sanity_check
	$(MAKE) -C $(APP)
	mkdir -p $(BCM_FSBUILD_DIR)/public/$(APP)/lib/
	$(MAKE) -C $(APP) install
	cp -f $(BCM_FSBUILD_DIR)/public/$(APP)/lib/$(LIBASM)*so* $(INSTALL_DIR)/lib/
	cp -f $(BCM_FSBUILD_DIR)/public/$(APP)/lib/$(LIBDW)*so* $(INSTALL_DIR)/lib/
	cp -f $(BCM_FSBUILD_DIR)/public/$(APP)/lib/$(LIBELF)*so* $(INSTALL_DIR)/lib/
	cp -r $(BCM_FSBUILD_DIR)/public/$(APP)/lib/elfutils/ $(INSTALL_DIR)/lib/elfutils/
	#cp -f $(BCM_FSBUILD_DIR)/public/$(APP)/lib/$(LIB)* $(INSTALL_DIR)/lib/
	echo "Done building $(APP)"
else
conditional_build: sanity_check
	@echo "skipping $(APP) (not configured)"
endif

# NOTE: make clean from within app does not do a proper job, so wiping out
# entire directory to ensure consistency.
clean: 
	rm -rf $(BCM_FSBUILD_DIR)/public/$(APP)
	rm -rf $(INSTALL_DIR)/lib/$(LIBASM)*so*
	rm -rf $(INSTALL_DIR)/lib/$(LIBDW)*so*
	rm -rf $(INSTALL_DIR)/lib/$(LIBELF)*so*
	rm -rf $(INSTALL_DIR)/lib/elfutils/
	@if [ -e $(APP)/Makefile ]; then \
		$(MAKE) -C $(APP) uninstall; \
		$(MAKE) -C $(APP) distclean; \
	fi
	rm -rf $(APP)

# The next line is a hint to our release scripts
# GLOBAL_RELEASE_SCRIPT_CALL_DISTCLEAN
distclean: clean

shell:
	bash

