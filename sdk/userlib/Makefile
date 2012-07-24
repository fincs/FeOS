USERLIBS := $(patsubst %/,%,$(dir $(wildcard */Makefile)))
DEPENDS  := $(wildcard */deps.mk)

ifeq ($(FEOSDEST),)
export FEOSDEST = $(FEOSSDK)/../FeOS
endif

.PHONY: all clean install $(USERLIBS)

all: $(USERLIBS)

clean:
	@for i in $(USERLIBS); do $(MAKE) --no-print-directory -C $$i clean || exit 1; done

install: all
	@for i in $(USERLIBS); do $(MAKE) --no-print-directory -C $$i install || exit 1; done

$(USERLIBS):
	@$(MAKE) --no-print-directory -C $@ || exit 1

-include $(DEPENDS)
