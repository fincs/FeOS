.SUFFIXES:

.PHONY: all clean

all:
	@make -C bin/fxe2tool_src
	@make -C source/FEOSBASE
	@make -C source/FEOSSTDIO
	@make -C source/FEOSPOSIXEMU
	@make -C source/FEOSMATH
	@make -C source/FEOSDSAPI
	@make -C source/FEOSDSSPR
	@make -C source/FEOSDSBG
	@make -C source/FEOSARM7
	@make -C source7/FEOS7
	@make -f libfeos.mk
	@make -f libfeos.mk DEBUGVER=1
	@make -f libfeos7.mk
	@make -f libfeoscxx.mk

clean:
	@make -C bin/fxe2tool_src clean
	@make -C source/FEOSBASE clean
	@make -C source/FEOSSTDIO clean
	@make -C source/FEOSPOSIXEMU clean
	@make -C source/FEOSMATH
	@make -C source/FEOSDSAPI clean
	@make -C source/FEOSDSSPR clean
	@make -C source/FEOSDSBG clean
	@make -C source/FEOSARM7 clean
	@make -C source7/FEOS7 clean
	@make -f libfeos.mk clean
	@make -f libfeos.mk DEBUGVER=1 clean
	@make -f libfeos7.mk clean
	@make -f libfeoscxx.mk clean
