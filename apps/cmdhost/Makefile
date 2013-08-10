#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------

ifeq ($(strip $(DEVKITPRO)),)
$(error "Please set DEVKITPRO in your environment. export DEVKITPRO=<path to>devkitPro")
endif

ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
endif

ifeq ($(strip $(FEOSSDK)),)
$(error "Please set FEOSSDK in your environment. export FEOSSDK=<path to>FeOS/sdk")
endif

FEOSMK = $(FEOSSDK)/mk

#---------------------------------------------------------------------------------
# TARGET is the name of the output
# BUILD is the directory where object files & intermediate files will be placed
# SOURCES is a list of directories containing source code
# DATA is a list of directories containing data files
# INCLUDES is a list of directories containing header files
#---------------------------------------------------------------------------------
TARGET        := $(shell basename $(CURDIR))
BUILD         := build
SOURCES       := source
DATA          := data
INCLUDES      := include
FILETYPES     := filetypes.cfg

CONF_DEFINES =
CONF_USERLIBS = coopgui
CONF_LIBS = -lcoopgui

include $(FEOSMK)/app.mk

install: all
	@mkdir -p $(FEOSDEST)/data/FeOS/bin
	@mkdir -p $(FEOSDEST)/data/FeOS/gui/ftypes
	@mkdir -p $(FEOSDEST)/data/FeOS/gui/assets
	@cp $(TARGET).fx2 $(FEOSDEST)/data/FeOS/bin/$(TARGET).fx2
	@cp $(FILETYPES) $(FEOSDEST)/data/FeOS/gui/ftypes/$(TARGET).cfg
	@grit apptile.png -ftr -fh! -gb -gB16 -gT! -gzl -p! -o $(FEOSDEST)/data/FeOS/gui/$(TARGET).grf
	@grit fileicon.png -ftr -fh! -gb -gB16 -gT7FFE -gzl -p! -o $(FEOSDEST)/data/FeOS/gui/ftypes/$(TARGET).grf
	@fmantool application.manifest $(FEOSDEST)/data/FeOS/gui/$(TARGET).app
