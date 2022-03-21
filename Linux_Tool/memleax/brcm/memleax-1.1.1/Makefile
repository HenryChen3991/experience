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

APP = memleax-1.1.1

ifneq ($(strip $(BUILD_MPTCP)),)
gtk_untar: $(APP)/$(UNTAR_COMPLETE_FILE)

$(APP)/$(UNTAR_COMPLETE_FILE): $(APP).tar.gz Makefile
	rm -rf $(APP)
	(tar xkf $(APP).tar.gz 2> /dev/null || true);
	patch -p1 -b -N -s -d$(APP) < $(APP)-0001.patch;

	@echo "$(APP) is untarred"
	touch $(APP)/$(UNTAR_COMPLETE_FILE)
	
$(APP)/$(CONFIG_COMPLETE_FILE): $(APP)/$(UNTAR_COMPLETE_FILE) 
	(cd $(APP); ./configure \
		--prefix=$(BCM_FSBUILD_DIR)/public/$(APP) \
		--LLFLAG="-L$(BCM_FSBUILD_DIR)/public/libunwind-1.6.2/lib/" \
		--LLFLAG="-L$(BCM_FSBUILD_DIR)/public/elfutils-0.163/lib/" \
		--CFLAGS="-DMLX_WITH_LIBDW" \
		--CFLAGS="-I$(BCM_FSBUILD_DIR)/public/elfutils-0.163/include/elfutils/" \
		--CFLAGS="-I$(BCM_FSBUILD_DIR)/public/elfutils-0.163/include" \
		--CFLAGS="-I$(BCM_FSBUILD_DIR)/public/elfutils-0.163" \
		--CFLAGS="-I$(BCM_FSBUILD_DIR)/public/libunwind-1.6.2/" \
		--CFLAGS="-I$(BCM_FSBUILD_DIR)/public/libunwind-1.6.2/include/" \
		--CFLAGS="-s" \
		--CFLAGS="-Os" \
		--CFLAGS="-Wno-strict-aliasing" || exit 1);
	@echo "$(APP) is configured"
	touch $(APP)/$(CONFIG_COMPLETE_FILE)

conditional_build: $(APP)/$(CONFIG_COMPLETE_FILE) sanity_check
	$(MAKE) -C $(APP)
	$(MAKE) -C $(APP) install
	cp $(BCM_FSBUILD_DIR)/public/$(APP)/bin/* $(INSTALL_DIR)/bin/
	echo "Done building $(APP)"
else
conditional_build: sanity_check
	@echo "skipping $(APP) (not configured)"
endif

# NOTE: make clean from within app does not do a proper job, so wiping out
# entire directory to ensure consistency.
clean:
	rm -rf $(BCM_FSBUILD_DIR)/public/$(APP)
	rm -f $(INSTALL_DIR)/bin/memleax
	rm -rf $(APP)

# The next line is a hint to our release scripts
# GLOBAL_RELEASE_SCRIPT_CALL_DISTCLEAN
distclean: clean

shell:
	bash


