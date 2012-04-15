#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------

FEOSBIN = $(FEOSSDK)/bin

ifeq ($(FEOSDEST),)
export FEOSDEST = $(FEOSSDK)/../FeOS
endif

#---------------------------------------------------------------------------------
# the prefix on the compiler executables
#---------------------------------------------------------------------------------
PREFIX         := arm-eabi-

export CC      := $(PREFIX)gcc
export CXX     := $(PREFIX)g++
export AS      := $(PREFIX)as
export AR      := $(PREFIX)ar
export OBJCOPY := $(PREFIX)objcopy
export STRIP   := $(PREFIX)strip

#---------------------------------------------------------------------------------
# options for code generation
#---------------------------------------------------------------------------------
DEFINES   := -DFEOS $(CONF_DEFINES)

ifeq ($(CONF_ARM7),)
ARCH       := -mcpu=arm946e-s -mtune=arm946e-s
DEFINES    += -DARM9
INCLUDECXX := -I$(FEOSSDK)/include/cxx
LDEXTRA    := --use-blx,
else
ARCH      := -mcpu=arm7tdmi -mtune=arm7tdmi
DEFINES   += -DARM7
LIBFEOSSUF := 7
endif

ifeq ($(CONF_GCSECTIONS),1)
GCSECTIONS := --gc-sections,
endif

ARMARCH   := -marm
THUMBARCH := -mthumb
ifeq ($(strip $(DEFARCH)),)
DEFARCH   := $(THUMBARCH)
endif

export CONF_DEBUG
ifeq ($(CONF_DEBUG),1)
ifeq ($(CONF_ARM7),1)
$(error "Debug build not available on ARM7, sorry")
endif
CONF_NODEBUGINFO :=
CFLAGSBASE := -O0
DEFINES    += -DDEBUG
LIBFEOSSUF := d
else
CFLAGSBASE := -O2
DEFINES    += -DNDEBUG
endif

# The default is to produce debugging info on ARM9 (but not on ARM7)
ifneq ($(CONF_ARM7),1)
export EMITDEBUGINFO := 1
else
export EMITDEBUGINFO := 0
endif

ifeq ($(CONF_NODEBUGINFO),1)
export EMITDEBUGINFO := 0
endif

ifeq ($(EMITDEBUGINFO),1)
DEBUGFLAG   := -g
CEXTRAFLAGS := -funwind-tables
else
DEBUGFLAG   := -g0
endif

ifeq ($(CONF_USECXXLIB),1)
ifeq ($(CONF_ARM7),1)
$(error "Debug build not available on ARM7, sorry")
endif
export CXXLIB := -lfeoscxx
endif

CFLAGS   := $(ARCH) $(DEBUGFLAG) $(CFLAGSBASE) $(CEXTRAFLAGS) -fomit-frame-pointer -Wall -save-temps -fvisibility=hidden\
            -mthumb-interwork -mword-relocations $(INCLUDE) $(DEFINES) -nostdinc $(CONF_CFLAGS)

CXXFLAGS := $(CFLAGS) $(INCLUDECXX) -fno-rtti -nostdinc++ -fvisibility-inlines-hidden $(CONF_CXXFLAGS)

ifneq ($(CONF_ARM7),)
CXXFLAGS += -fno-exceptions
endif

ASFLAGS  := $(DEBUGFLAG) $(ARCH) $(DEFINES) $(INCLUDE)
LDFLAGS  := -nostartfiles -nostdlib -T $(FEOSBIN)/fxe2.ld $(ARCH) -Wl,-d,-q,$(GCSECTIONS)$(LDEXTRA)-Map,$(TARGET).map

ifeq ($(CONF_NOSTDLIB),)
LIBS     := -lfeos$(LIBFEOSSUF) $(CXXLIB)
endif
LIBS     := $(CONF_LIBS) $(LIBS)

#---------------------------------------------------------------------------------
# list of directories containing libraries, this must be the top level containing
# include and lib
#---------------------------------------------------------------------------------
ifneq ($(CONF_NOSTDLIB),1)
LIBDIRS := $(FEOSSDK)
ifneq ($(CONF_ARM7),)
LIBDIRS += $(DEVKITPRO)/libnds
endif
endif
LIBDIRS += $(foreach lib,$(CONF_USERLIBS),$(FEOSSDK)/userlib/$(lib))
LIBDIRS += $(CONF_LIBDIRS)

#---------------------------------------------------------------------------------
# no real need to edit anything past this point unless you need to add additional
# rules for different file extensions
#---------------------------------------------------------------------------------
ifneq ($(BUILD),$(notdir $(CURDIR)))
#---------------------------------------------------------------------------------

export PATH := $(FEOSBIN):$(DEVKITARM)/bin:$(PATH)

ifneq ($(strip $(CONF_TARGET)),staticlib)
export OUTPUT := $(CURDIR)/$(TARGET)
else
export OUTPUT := $(CURDIR)/lib/lib$(TARGET)
endif

export VPATH := $(foreach dir,$(SOURCES),$(CURDIR)/$(dir)) \
                $(foreach dir,$(DATA),$(CURDIR)/$(dir))

export DEPSDIR := $(CURDIR)/$(BUILD)

CFILES   := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
CPPFILES := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))
SFILES   := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.s)))
BINFILES := $(foreach dir,$(DATA),$(notdir $(wildcard $(dir)/*.*)))

#---------------------------------------------------------------------------------
# use CXX for linking C++ projects, CC for standard C
#---------------------------------------------------------------------------------

ifeq ($(strip $(CPPFILES)),)
	export LD := $(CC)
else
	export LD := $(CXX)
ifeq ($(CONF_ARM7),)
ifneq ($(CONF_NOCXXLIB),1)
	export CXXLIB := -lfeoscxx
endif
endif
endif

export OFILES   := $(addsuffix .o,$(BINFILES)) \
                   $(CPPFILES:.cpp=.o) $(CFILES:.c=.o) $(SFILES:.s=.o)

export INCLUDE  := $(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) \
                   $(foreach dir,$(LIBDIRS),-I$(dir)/include) \
                   -I$(CURDIR)/$(BUILD)

export LIBPATHS := $(foreach dir,$(LIBDIRS),-L$(dir)/lib)

ifeq ($(strip $(THIS_MAKEFILE)),)
	THIS_MAKEFILE := Makefile
endif

.PHONY: $(BUILD) clean all

#---------------------------------------------------------------------------------
all: $(BUILD)

$(BUILD):
	@[ -d $@ ] || mkdir -p $@
ifeq ($(strip $(CONF_TARGET)),staticlib)
	@mkdir -p lib
endif
	@make --no-print-directory -C $(BUILD) -f $(CURDIR)/$(THIS_MAKEFILE)
ifneq ($(strip $(CONF_TARGET)),staticlib)
	@arm-eabi-nm -CSn $(OUTPUT).elf > $(BUILD)/$(TARGET).alt.map
else
	@arm-eabi-nm -CSn $(OUTPUT).a > $(BUILD)/$(TARGET).alt.map
endif
ifeq ($(strip $(CONF_TARGET)),dynlib)
	@if [ -e $(TARGET).imp.a ]; then \
		mkdir -p lib; \
		cp $(TARGET).imp.a lib/lib$(TARGET).a; \
	fi
endif

#---------------------------------------------------------------------------------
clean:
ifneq ($(strip $(CONF_TARGET)),staticlib)
	@rm -fr $(BUILD) $(TARGET).elf $(TARGET).elf.dbg $(TARGET).fx2 $(TARGET).imp.a lib/lib$(TARGET).a
else
	@rm -fr $(BUILD) lib/lib$(TARGET).a
endif

#---------------------------------------------------------------------------------
else

DEPENDS := $(OFILES:.o=.d)

#---------------------------------------------------------------------------------
# main target(s)
#---------------------------------------------------------------------------------

ifneq ($(strip $(CONF_TARGET)),staticlib)

$(OUTPUT).fx2: $(OUTPUT).elf
$(OUTPUT).elf: $(OFILES)

#---------------------------------------------------------------------------------
%.fx2:
	@fxe2tool $(OUTPUT)
	@echo Built: $(notdir $@)

#---------------------------------------------------------------------------------
%.elf:
	@echo
	@echo Linking...
	@$(LD) $(LDFLAGS) $(OFILES) $(LIBPATHS) $(LIBS) -o $@
ifeq ($(EMITDEBUGINFO),1)
	@$(OBJCOPY) --only-keep-debug $@ $@.dbg
	@$(STRIP) -g $@
endif

else

$(OUTPUT).a: $(OFILES)

#---------------------------------------------------------------------------------
%.a:
	@echo
	@echo Linking...
	@rm -f $@
	@$(AR) -rc $@ $^
	@echo Built: $(notdir $@)

endif

#---------------------------------------------------------------------------------
%.arm.o: %.arm.c
	@echo $(notdir $<)
	@$(CC) -MMD -MP -MF $(DEPSDIR)/$*.d $(ARMARCH) $(CFLAGS) -c $< -o $@

#---------------------------------------------------------------------------------
%.arm.o: %.arm.cpp
	@echo $(notdir $<)
	@$(CXX) -MMD -MP -MF $(DEPSDIR)/$*.d $(ARMARCH) $(CXXFLAGS) -c $< -o $@

#---------------------------------------------------------------------------------
%.thumb.o: %.thumb.c
	@echo $(notdir $<)
	@$(CC) -MMD -MP -MF $(DEPSDIR)/$*.d $(THUMBARCH) $(CFLAGS) -c $< -o $@

#---------------------------------------------------------------------------------
%.thumb.o: %.thumb.cpp
	@echo $(notdir $<)
	@$(CXX) -MMD -MP -MF $(DEPSDIR)/$*.d $(THUMBARCH) $(CXXFLAGS) -c $< -o $@

#---------------------------------------------------------------------------------
%.o: %.c
	@echo $(notdir $<)
	@$(CC) -MMD -MP -MF $(DEPSDIR)/$*.d $(DEFARCH) $(CFLAGS) -c $< -o $@

#---------------------------------------------------------------------------------
%.o: %.cpp
	@echo $(notdir $<)
	@$(CXX) -MMD -MP -MF $(DEPSDIR)/$*.d $(DEFARCH) $(CXXFLAGS) -c $< -o $@

#---------------------------------------------------------------------------------
%.o: %.s
	@echo $(notdir $<)
	@$(CC) -MMD -MP -MF $(DEPSDIR)/$*.d -x assembler-with-cpp $(ASFLAGS) -c $< -o $@

#---------------------------------------------------------------------------------
# canned command sequence for binary data
#---------------------------------------------------------------------------------
define bin2o
	bin2s $< | $(AS) -o $(@)
	echo "extern const u8" `(echo $(<F) | sed -e 's/^\([0-9]\)/_\1/' | tr . _)`"_end[];" > `(echo $(<F) | tr . _)`.h
	echo "extern const u8" `(echo $(<F) | sed -e 's/^\([0-9]\)/_\1/' | tr . _)`"[];" >> `(echo $(<F) | tr . _)`.h
	echo "extern const u32" `(echo $(<F) | sed -e 's/^\([0-9]\)/_\1/' | tr . _)`_size";" >> `(echo $(<F) | tr . _)`.h
endef

-include $(DEPENDS)

#---------------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------------
