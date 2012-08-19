APPS     := $(patsubst %/,%,$(dir $(wildcard */Makefile)))
DEPENDS  := $(wildcard */deps.mk)

ifeq ($(FEOSDEST),)
export FEOSDEST = $(FEOSSDK)/../FeOS
endif

.PHONY: all clean install $(APPS)

all: $(APPS)

clean: TARGET := clean
clean: all

install: TARGET := install
install: all

$(APPS):
	@$(MAKE) --no-print-directory -C $@ $(TARGET)

-include $(DEPENDS)
