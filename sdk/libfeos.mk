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
ifeq ($(DEBUGVER),)
TARGET        := feos
BUILD         := build
else
TARGET        := feosd
BUILD         := build_dbg
CONF_DEBUG    := 1
EXTRASRC      := source/debug
endif
SOURCES       := source source/FEOSBASE source/FEOSSTDIO source/FEOSPOSIXEMU source/FEOSMATH \
                 source/FEOSDSAPI source/FEOSDSSPR source/FEOSDSBG source/FEOSARM7 source/stdio \
                 $(EXTRASRC)
DATA          := data
INCLUDES      := include

CONF_CXXFLAGS = -fno-exceptions -fno-rtti

CONF_NOSTDLIB = 1
THIS_MAKEFILE := libfeos.mk

include $(FEOSMK)/staticlib.mk
