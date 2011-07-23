# change this to point to your memory card or use a symlink or something (absolute path in unix style!)
export TARGET := $(CURDIR)/FeOS

SUBDIRS  := $(patsubst %/,%,$(dir $(wildcard */Makefile)))
APPS     := $(patsubst %/,%,$(dir $(wildcard apps/*/Makefile)))

.PHONY: clean install

all:
	@$(MAKE) --no-print-directory -C sdk || exit 1
	@$(MAKE) --no-print-directory -C kernel || exit 1
	@$(MAKE) --no-print-directory -C cmdprompt || exit 1
	@for i in $(APPS); do $(MAKE) --no-print-directory -C $$i || exit 1; done

clean:
	@$(MAKE) --no-print-directory -C sdk clean
	@$(MAKE) --no-print-directory -C kernel clean
	@$(MAKE) --no-print-directory -C cmdprompt clean
	@for i in $(APPS); do $(MAKE) --no-print-directory -C $$i clean || { exit 1;} done
	@rm -f $(TARGET)/data/FeOS/bin/*
	@rm -f $(TARGET)/data/FeOS/lib/*

install:
	@mkdir -p $(TARGET)/data/FeOS/bin || exit 1
	@mkdir -p $(TARGET)/data/FeOS/lib || exit 1
	@cp kernel/FeOS.nds $(TARGET)/FeOS.nds || exit 1
	@cp cmdprompt/cmd.fx2 $(TARGET)/data/FeOS/bin/cmd.fx2 || exit 1
	@for i in $(APPS); do $(MAKE) --no-print-directory -C $$i install; done
