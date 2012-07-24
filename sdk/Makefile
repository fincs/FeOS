.SUFFIXES:

.PHONY: all clean

all:
	@$(MAKE) -C bin/fxe2tool_src
	@$(MAKE) -C bin/fartool_src
	@$(MAKE) -C source/FEOSBASE
	@$(MAKE) -C source/FEOSSTDIO
	@$(MAKE) -C source/FEOSPOSIXEMU
	@$(MAKE) -C source/FEOSMATH
	@$(MAKE) -C source/FEOSDSAPI
	@$(MAKE) -C source/FEOSDSSPR
	@$(MAKE) -C source/FEOSDSBG
	@$(MAKE) -C source/FEOSARM7
	@$(MAKE) -C source7/FEOS7
	@$(MAKE) -f libfeos.mk
	@$(MAKE) -f libfeos.mk DEBUGVER=1
	@$(MAKE) -f libfeos7.mk
	@$(MAKE) -f libfeoscxx.mk

clean:
	@$(MAKE) -C bin/fxe2tool_src clean
	@$(MAKE) -C bin/fartool_src clean
	@$(MAKE) -C source/FEOSBASE clean
	@$(MAKE) -C source/FEOSSTDIO clean
	@$(MAKE) -C source/FEOSPOSIXEMU clean
	@$(MAKE) -C source/FEOSMATH clean
	@$(MAKE) -C source/FEOSDSAPI clean
	@$(MAKE) -C source/FEOSDSSPR clean
	@$(MAKE) -C source/FEOSDSBG clean
	@$(MAKE) -C source/FEOSARM7 clean
	@$(MAKE) -C source7/FEOS7 clean
	@$(MAKE) -f libfeos.mk clean
	@$(MAKE) -f libfeos.mk DEBUGVER=1 clean
	@$(MAKE) -f libfeos7.mk clean
	@$(MAKE) -f libfeoscxx.mk clean
