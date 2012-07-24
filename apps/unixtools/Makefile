SUBDIRS := $(patsubst %/,%,$(dir $(wildcard */Makefile)))
.PHONY: $(SUBDIRS)

all: $(SUBDIRS)

clean:
	@for i in $(SUBDIRS); do $(MAKE) -C $$i clean; done

install: all
	@for i in $(SUBDIRS); do $(MAKE) -C $$i install; done

$(SUBDIRS):
	$(MAKE) -C $@
