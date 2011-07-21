# change this to point to your memory card or use a symlink or something
TARGET  := FeOS

SUBDIRS := $(dir $(wildcard */Makefile))

.PHONY: sdk kernel cmdprompt clean clobber install

all: sdk kernel cmdprompt

sdk:
	@$(MAKE) -C sdk

kernel:
	@$(MAKE) -C kernel

cmdprompt:
	@$(MAKE) -C cmdprompt

clean:
	@$(MAKE) -C sdk clean
	@$(MAKE) -C kernel clean
	@$(MAKE) -C cmdprompt clean

clobber:
	@for i in $(SUBDIRS); do $(MAKE) -C $$i clean || { exit 1;} done

install: all
	@mkdir -p $(TARGET)/data/FeOS/bin || exit 1
	@mkdir -p $(TARGET)/data/FeOS/lib || exit 1
	@cp kernel/FeOS.nds $(TARGET)/FeOS.nds || exit 1
	@cp cmdprompt/cmd.fx2 $(TARGET)/data/FeOS/bin/cmd.fx2 || exit 1