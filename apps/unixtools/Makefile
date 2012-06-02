export SUBDIRS := $(patsubst %/,%,$(dir $(wildcard */Makefile)))

all:
	@for i in $(SUBDIRS); do $(MAKE) -C $$i; done

clean:
	@for i in $(SUBDIRS); do $(MAKE) -C $$i clean; done

install:
	@for i in $(SUBDIRS); do $(MAKE) -C $$i install; done
