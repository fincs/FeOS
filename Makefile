# change this to point to your memory card or use a symlink or something (absolute path in unix style!)
export DEST := $(CURDIR)/FeOS

SUBDIRS  := $(patsubst %/,%,$(dir $(wildcard */Makefile)))
APPS     := $(patsubst %/,%,$(dir $(wildcard apps/*/Makefile)))
USERLIBS := $(patsubst %/,%,$(dir $(wildcard sdk/userlib/*/Makefile)))

.PHONY: all clean install liball libclean libinstall

all:
	@$(MAKE) --no-print-directory -C sdk || exit 1
	@$(MAKE) --no-print-directory -C arm7 || exit 1
	@$(MAKE) --no-print-directory -C kernel || exit 1
	@$(MAKE) --no-print-directory -C cmdprompt || exit 1
	@for i in $(APPS); do $(MAKE) --no-print-directory -C $$i || exit 1; done

liball:
	@for i in $(USERLIBS); do $(MAKE) --no-print-directory -C $$i || exit 1; done

clean:
	@$(MAKE) --no-print-directory -C sdk clean
	@$(MAKE) --no-print-directory -C arm7 clean
	@$(MAKE) --no-print-directory -C kernel clean
	@$(MAKE) --no-print-directory -C cmdprompt clean
	@for i in $(APPS); do $(MAKE) --no-print-directory -C $$i clean || { exit 1;} done
	@rm -f $(DEST)/data/FeOS/bin/*
	@rm -f $(DEST)/data/FeOS/lib/*
	@rm -f $(DEST)/data/FeOS/arm7/*

libclean:
	@for i in $(USERLIBS); do $(MAKE) --no-print-directory -C $$i clean || { exit 1;} done

install: all
	@mkdir -p $(DEST)/data/FeOS/bin || exit 1
	@mkdir -p $(DEST)/data/FeOS/lib || exit 1
	@cp kernel/FeOS.nds $(DEST)/FeOS.nds || exit 1
	@cp sdk/feoscxx.fx2 $(DEST)/data/FeOS/lib/feoscxx.fx2 || exit 1
	@cp sdk/feosstl.fx2 $(DEST)/data/FeOS/lib/feosstl.fx2 || exit 1
	@cp cmdprompt/cmd.fx2 $(DEST)/data/FeOS/bin/cmd.fx2 || exit 1
	@for i in $(APPS); do $(MAKE) --no-print-directory -C $$i install; done

libinstall: liball
	@for i in $(USERLIBS); do $(MAKE) --no-print-directory -C $$i install; done
