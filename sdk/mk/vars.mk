#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------

FEOSBIN = $(FEOSSDK)/bin
FEOSUSERLIB = $(FEOSSDK)/userlib

ifeq ($(FEOSDEST),)
ifneq ($(wildcard $(FEOSSDK)/libfeos.mk),)
export FEOSDEST = $(FEOSSDK)/../FeOS
else
export FEOSDEST = $(FEOSSDK)/install-root
endif
endif

#---------------------------------------------------------------------------------
# the prefix on the compiler executables
#---------------------------------------------------------------------------------
PREFIX         := arm-none-eabi-

export CC      := $(PREFIX)gcc
export CXX     := $(PREFIX)g++
export AS      := $(PREFIX)as
export AR      := $(PREFIX)ar
export OBJCOPY := $(PREFIX)objcopy
export STRIP   := $(PREFIX)strip
export NM      := $(PREFIX)nm
