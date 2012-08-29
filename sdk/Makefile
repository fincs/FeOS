.SUFFIXES:

TOOLS := bin/fxe2tool_src \
         bin/fartool_src \
         bin/fpkgtool_src \
         bin/fmantool_src \
FAKES := source/FEOSBASE \
         source/FEOSSTDIO \
         source/FEOSPOSIXEMU \
         source/FEOSMATH \
         source/FEOSDSAPI \
         source/FEOSDSSPR \
         source/FEOSDSBG \
         source/FEOSARM7 \
         source7/FEOS7

.PHONY: all clean $(TOOLS) $(FAKES)

all: $(TOOLS) $(FAKES)
	@$(MAKE) -f libfeos.mk
	@$(MAKE) -f libfeos.mk DEBUGVER=1
	@$(MAKE) -f libfeos7.mk
	@$(MAKE) -f libfeoscxx.mk

clean:
	@for i in $(TOOLS); do $(MAKE) -C $$i clean; done
	@for i in $(FAKES); do $(MAKE) -C $$i clean; done
	@$(MAKE) -f libfeos.mk clean
	@$(MAKE) -f libfeos.mk DEBUGVER=1 clean
	@$(MAKE) -f libfeos7.mk clean
	@$(MAKE) -f libfeoscxx.mk clean

$(TOOLS) $(FAKES):
	@$(MAKE) -C $@
