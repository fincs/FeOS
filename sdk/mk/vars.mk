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
# devkitARM configuration
#---------------------------------------------------------------------------------
include $(DEVKITARM)/base_tools
