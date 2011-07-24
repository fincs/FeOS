export SUBDIRS := $(patsubst %/,%,$(dir $(wildcard */Makefile)))

all:
	@for i in $(SUBDIRS); do $(MAKE) -C $$i; done

clean:
	@for i in $(SUBDIRS); do $(MAKE) -C $$i clean; done

ifneq ($(strip $(DEST)),)
install: all
	@for i in $(SUBDIRS); do cp $$i/$$i.fx2 $(DEST)/data/FeOS/bin/$$i.fx2; done
endif
