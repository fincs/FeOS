USERLIBS := $(patsubst %/,%,$(dir $(wildcard */Makefile)))
DEPENDS  := $(wildcard */deps.mk)

ifeq ($(FEOSDEST),)
export FEOSDEST = $(FEOSSDK)/../FeOS
endif

.PHONY: all clean install $(USERLIBS)

all: $(USERLIBS)

clean: TARGET := clean
clean: all

install: TARGET := install
install: all

$(USERLIBS):
	@$(MAKE) --no-print-directory -C $@ $(TARGET)

-include $(DEPENDS)
