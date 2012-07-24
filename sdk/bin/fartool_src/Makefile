# This Makefile was blatantly ripped off from the devkitPro project

.SUFFIXES:

TARGET  := fartool
BUILD   := build
SOURCES := source

export CC    := gcc
export CXX   := g++
export STRIP := strip

CFLAGS   := -g0 -Wall -O2
CXXFLAGS := $(CFLAGS) -fno-rtti -fno-exceptions

LDFLAGS := -g0

UNAME := $(shell uname -s)

ifneq (,$(findstring MINGW,$(UNAME)))
	EXEEXT := .exe
	LDFLAGS += -static-libgcc -static-libstdc++
endif

ifneq (,$(findstring Darwin,$(UNAME)))
	SDK         := /Developer/SDKs/MacOSX10.4u.sdk
	OSXCFLAGS   := -mmacosx-version-min=10.4 -isysroot $(SDK) -arch i386 -arch ppc
	OSXCXXFLAGS := -fvisibility=hidden -mmacosx-version-min=10.4 -isysroot $(SDK) -arch i386 -arch ppc
	OSXLDFLAGS  := -mmacosx-version-min=10.4 -Wl,-syslibroot,$(SDK) -arch i386 -arch ppc
endif

ifneq ($(BUILD),$(notdir $(CURDIR)))

export OUTPUT := $(CURDIR)/../$(TARGET)$(EXEEXT)
export DEPSDIR := $(CURDIR)/$(BUILD)
export VPATH := $(foreach dir,$(SOURCES),$(CURDIR)/$(dir))

CFILES := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
CPPFILES := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))

ifeq ($(strip $(CPPFILES)),)
	export LD := $(CC)
else
	export LD := $(CXX)
endif

export OFILES := $(CFILES:.c=.o) $(CPPFILES:.cpp=.o)

.PHONY: $(BUILD) clean

$(BUILD):
	@[ -d $@ ] || mkdir -p $@
	@$(MAKE) --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile

clean:
	@rm -fr $(BUILD) $(OUTPUT)

else

$(OUTPUT): $(OFILES)

-include $(DEPSDIR)/*.d

$(OUTPUT):
	@echo Linking...
	@$(LD) $(OSXLDFLAGS) $(LDFLAGS) $(OFILES) -o $@
	@$(STRIP) $@

%.o: %.c
	@echo $(notdir $<)
	@$(CC) -E -MMD -MF $(DEPSDIR)/$(*).d $(CFLAGS) $< > /dev/null
	@$(CC) $(OSXCFLAGS) $(CFLAGS) -o $@ -c $<

%.o: %.cpp
	@echo $(notdir $<)
	@$(CXX) -E -MMD -MF $(DEPSDIR)/$(*).d $(CXXFLAGS) $< > /dev/null
	@$(CXX) $(OSXCXXFLAGS) $(CXXFLAGS) -o $@ -c $<

endif
