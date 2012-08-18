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
        apps appsclean appsinstall\
        outputfolders

################################################################################
# generic rules
################################################################################
all: $(ALL)

clean: $(addsuffix clean,$(ALL))
	@rm -f $(FEOSDEST)/data/FeOS/bin/*
	@rm -f $(FEOSDEST)/data/FeOS/lib/*
	@rm -f $(FEOSDEST)/data/FeOS/arm7/*

install: $(addsuffix install,$(ALL))

outputfolders:
	@mkdir -p $(FEOSDEST)/data/FeOS/bin
	@mkdir -p $(FEOSDEST)/data/FeOS/lib
	@mkdir -p $(FEOSDEST)/data/FeOS/arm7

################################################################################
# 'all' rules
################################################################################
sdk:
	@$(MAKE) --no-print-directory -C sdk
	@$(MAKE) --no-print-directory -C arm7
	@$(MAKE) --no-print-directory -C kernel
	@$(MAKE) --no-print-directory -C kernel DEBUGVER=1

lib: sdk
	@$(MAKE) --no-print-directory -C sdk/userlib

apps: lib
	@$(MAKE) --no-print-directory -C apps

################################################################################
# 'clean' rules
################################################################################
sdkclean:
	@$(MAKE) --no-print-directory -C sdk clean
	@$(MAKE) --no-print-directory -C arm7 clean
	@$(MAKE) --no-print-directory -C kernel clean
	@$(MAKE) --no-print-directory -C kernel clean DEBUGVER=1

libclean:
	@$(MAKE) --no-print-directory -C sdk/userlib clean

appsclean:
	@$(MAKE) --no-print-directory -C apps clean

################################################################################
# 'install' rules
################################################################################
sdkinstall: sdk outputfolders
	@cp kernel/FeOS.nds $(FEOSDEST)/FeOS.nds
	@cp kernel/FeOSd.nds $(FEOSDEST)/FeOSd.nds
	@cp sdk/feoscxx.fx2 $(FEOSDEST)/data/FeOS/lib/feoscxx.fx2
	@[ -e $(AUTOEXEC) ] || touch $(AUTOEXEC)

libinstall: outputfolders
	@$(MAKE) --no-print-directory -C sdk/userlib install

appsinstall: libinstall
	@$(MAKE) --no-print-directory -C apps install
