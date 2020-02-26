# Generic Makefile for C/C++ Program
# Author: 
# Description: 
# This is an easily customizable makefile template. The purpose is to
# provide an instant building environment for C/C++ programs.
#
# It searches all the C/C++ source files in the specified directories,
# makes dependencies, compiles and links to form an executable.
#
# Besides its default ability to build C/C++ programs which use only
# standard C/C++ libraries, you can customize the Makefile to build
# those using other libraries. Once done, without any changes you can
# then build programs using the same or less libraries, even if source
# files are renamed, added or removed. Therefore, it is particularly
# convenient to use it to build codes for experimental or study use.
#
# GNU make is expected to use the Makefile. Other versions of makes
#

.PHONY : all clean

# Curstomer build output file directory and filename
EXES =  Ad
DIR_EXES = 
DIR_OBJS = 
DIR_DEPS =
DIR_INCS = 

LINK_LIBS = 
LIBS_TYPE = dynamic  
#LIBS_TYPE = static
DIR_LIBS = 
LIBS = 

# Top directory  Makefile
CURDIR = $(shell pwd)

# The C program compiler
COPTION = -O2 
MACRO = -DDEBUGALL 
CFLAGS += -g -werror $(MACRO) $(COPTION)
CC = gcc
AR = ar
ARFLAGES = crv

# default execute output directory
ifeq ($(DIR_EXES),)
DIR_EXES = $(TOPDIR)/build/out
endif

# defaulet libaray creat directory
ifeq ($(DIR_LIBS),)
DIR_LIBS = $(TOPDIR)/build/libs
endif

# directory
DIRS = $(DIR_OBJS) $(DIR_DEPS) $(DIR_EXES) $(DIR_LIBS)

# include directory 
ifneq ($(DIR_INCS),"")
DIR_INCS := $(strip $(DIR_INCS))
DIR_INCS := $(addprefix -I,$(DIR_INCS))
endif

# build execute file 
ifneq ($(EXES),)
EXES := $(addprefix $(DIR_EXES)/,$(EXES))
RMS += $(EXES)
DIR_LIBS := $(strip $(DIR_LIBS))
DIR_LIBS := $(addprefix -L,$(DIR_LIBS))
endif

# build libaray file 
ifneq ($(LIBS),"")
LIBS := $(addprefix $(DIR_LIBS)/,$(LIBS))
RMS += $(LIBS)
endif

# default source code file directory
ifeq ($(DIR_SRCS),)
DIR_SRCS = .
endif 

# scan source code 
SRCS = $(wildcard $(DIR_SRCS)/*.c)
OBJS = $(patsubst %.c, %.o,$(notdir $(SRCS)))
OBJS := $(addprefix $(DIR_OBJS)/,$(OBJS))
RMS += $(OBJS) $(DIR_OBJS)

# dependant file 
DEPS = $(patsubst %.c, %.dep,$(notdir $(SRCS)))
DEPS := $(addprefix $(DIR_DEPS)/,$(DEPS))
RMS += $(DEPS) $(DIR_DEPS)

# build execute file 
ifneq ($(EXES),"")
all : $(EXES)
endif

# build library 
ifneq ($(LIBS),"")
all : $(LIBS)
endif

# link libs name 
ifneq ($(LINK_LIBS),"")
LINK_LIBS := $(strip $(LINK_LIBS))
LINK_LIBS := $(addprefix -l,$(LINK_LIBS))
endif

# include dependent files 
ifneq ($(MAKECMDGOALS), clean)
-include $(DEPS)
endif

$(DIRS):
	mkdir -p $@

# creat execute file
$(EXES) : $(DIR_OBJS) $(OBJS) $(DIR_EXES)
	$(CC) $(DIR_INCS) $(CFLAGES) -o $@ $(OBJS) $(DIR_LIBS) $(LINK_LIBS)

# creat libaray file
$(LIBS) : $(DIR_LIBS) $(DIR_OBJS) $(OBJS)
# library type is static
ifeq ($(LIB_TYPE),static)
	$(AR) $(ARFLAGS) $@ $(OBJS)
endif

# library type is dynamic
ifeq ($(LIB_TYPE),dynamic)
	$(CC) -shared -o $@ $(OBJS)
endif

# creat object file 
$(DIR_OBJS)/%.o : $(DIR_SRCS)/%.c
	@echo "source files:" $<
	@echo "object files:" $@
ifeq ($(LIB_TYPE),static)
	$(CC) $(DIR_INCS) $(CFLAGES) -o $@ -c $<
else
	$(CC) $(DIR_INCS) $(CFLAGES) -fPIC -o $@ -c $<
endif

# creat depandant file
$(DIR_DEPS)/%.dep : $(DIR_SRCS)/%.c $(DIR_DEPS)
	@echo "creating depend file ..." $@
	@set -e;\
	$(CC) $(DIR_INCS) -MM $< > $@.tmp;\
	sed 's,\($*\)\.o[ :]*,$(DIR_OBJS)/\1.o $@ : ,g' < $@.tmp > $@

clean:
	rm -rf $(RMS)
