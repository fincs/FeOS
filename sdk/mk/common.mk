#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------

FEOSBIN = $(FEOSSDK)/bin

#---------------------------------------------------------------------------------
# the prefix on the compiler executables
#---------------------------------------------------------------------------------
PREFIX         := arm-eabi-

export CC      := $(PREFIX)gcc
export CXX     := $(PREFIX)g++
export AS      := $(PREFIX)as
export AR      := $(PREFIX)ar
export OBJCOPY := $(PREFIX)objcopy

#---------------------------------------------------------------------------------
# options for code generation
#---------------------------------------------------------------------------------
DEFINES   := -DFEOS $(CONF_DEFINES)
ARCH      := -mcpu=arm946e-s -mtune=arm946e-s
ARMARCH   := -marm
THUMBARCH := -mthumb
ifeq ($(strip $(DEFARCH)),)
DEFARCH   := $(THUMBARCH)
endif

INCLUDECXX := -I$(FEOSSDK)/include/cxx

CFLAGS   := -g0 -Wall -O2 -save-temps -fvisibility=hidden\
            $(ARCH) -fomit-frame-pointer -ffast-math\
            -mthumb-interwork $(INCLUDE) $(DEFINES) -nostdinc $(CONF_CFLAGS)

CXXFLAGS := $(CFLAGS) $(INCLUDECXX) -nostdinc++ -fvisibility-inlines-hidden $(CONF_CXXFLAGS)

ASFLAGS  := -g0 $(ARCH) $(DEFINES)
LDFLAGS  := -nostartfiles -nostdlib -T $(FEOSBIN)/fxe2.ld -g $(ARCH) -Wl,-d,-q,-Map,$(TARGET).map

ifneq ($(CONF_NOSTDLIB),1)
LIBS     := $(CXXLIB) -lfeos
endif
LIBS     := $(CONF_LIBS) $(LIBS)

#---------------------------------------------------------------------------------
# list of directories containing libraries, this must be the top level containing
# include and lib
#---------------------------------------------------------------------------------
ifneq ($(CONF_NOSTDLIB),1)
LIBDIRS := $(FEOSSDK)
endif
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
ifneq ($(CONF_NOCXXLIB),1)
	export CXXLIB := -lfeoscxx
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
	@rm -fr $(BUILD) $(TARGET).elf $(TARGET).fx2 $(TARGET).imp.a lib/lib$(TARGET).a
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
