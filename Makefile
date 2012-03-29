SUBDIRS  := $(patsubst %/,%,$(dir $(wildcard */Makefile)))
APPS     := $(patsubst %/,%,$(dir $(wildcard apps/*/Makefile)))

ifeq ($(FEOSDEST),)
export FEOSDEST = $(FEOSSDK)/../FeOS
endif

AUTOEXEC := $(FEOSDEST)/data/FeOS/autoexec.cmd

ifeq ($(FEOSBUILD_ALL),)
ALL := sdk lib apps
else
ALL := $(FEOSBUILD_ALL)
endif

.PHONY: all  clean     install\
        sdk  sdkclean  sdkinstall\
        lib  libclean  libinstall\
        apps appsclean appsinstall

################################################################################
# generic rules
################################################################################
all: $(ALL)
	@mkdir -p $(FEOSDEST)/data/FeOS/bin  || exit 1
	@mkdir -p $(FEOSDEST)/data/FeOS/lib  || exit 1
	@mkdir -p $(FEOSDEST)/data/FeOS/arm7 || exit 1

clean: $(addsuffix clean,$(ALL))
	@rm -f $(FEOSDEST)/data/FeOS/bin/*
	@rm -f $(FEOSDEST)/data/FeOS/lib/*
	@rm -f $(FEOSDEST)/data/FeOS/arm7/*

install: all $(addsuffix install,$(ALL))

################################################################################
# 'all' rules
################################################################################
sdk:
	@$(MAKE) --no-print-directory -C sdk || exit 1
	@$(MAKE) --no-print-directory -C arm7 || exit 1
	@$(MAKE) --no-print-directory -C kernel || exit 1

lib:
	@$(MAKE) --no-print-directory -C sdk/userlib || exit 1

apps:
	@for i in $(APPS); do $(MAKE) --no-print-directory -C $$i || exit 1; done

################################################################################
# 'clean' rules
################################################################################
sdkclean:
	@$(MAKE) --no-print-directory -C sdk clean
	@$(MAKE) --no-print-directory -C arm7 clean
	@$(MAKE) --no-print-directory -C kernel clean

libclean:
	@$(MAKE) --no-print-directory -C sdk/userlib clean || exit 1

appsclean:
	@for i in $(APPS); do $(MAKE) --no-print-directory -C $$i clean || { exit 1;} done

################################################################################
# 'install' rules
################################################################################
sdkinstall: sdk
	@cp kernel/FeOS.nds $(FEOSDEST)/FeOS.nds || exit 1
	@cp sdk/feoscxx.fx2 $(FEOSDEST)/data/FeOS/lib/feoscxx.fx2 || exit 1
	@[ -e $(AUTOEXEC) ] || touch $(AUTOEXEC)

libinstall: lib
	@mkdir -p $(FEOSDEST)/data/FeOS/bin  || exit 1
	@mkdir -p $(FEOSDEST)/data/FeOS/lib  || exit 1
	@mkdir -p $(FEOSDEST)/data/FeOS/arm7 || exit 1
	@$(MAKE) --no-print-directory -C sdk/userlib install || exit 1

appsinstall: apps
	@for i in $(APPS); do $(MAKE) --no-print-directory -C $$i install; done
