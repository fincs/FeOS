.SUFFIXES:

.PHONY: all clean

all:
	@make -C bin/fxe2tool_src
	@make -C source/FEOSBASE
	@make -C source/FEOSSTDIO
	@make -C source/FEOSPOSIXEMU
	@make -f libfeos.mk

clean:
	@make -C bin/fxe2tool_src clean
	@make -C source/FEOSBASE clean
	@make -C source/FEOSSTDIO clean
	@make -C source/FEOSPOSIXEMU clean
	@make -f libfeos.mk clean
