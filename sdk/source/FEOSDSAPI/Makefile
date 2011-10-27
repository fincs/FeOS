.SUFFIXES:

MODULE := $(shell basename $(CURDIR))

FUNCS := \
FeOS_GetKeysDown \
FeOS_GetKeysHeld \
FeOS_GetKeysUp \
FeOS_GetStylusPos \
FeOS_SetInterrupt \
FeOS_CheckPendingIRQs \
FeOS_WaitForIRQ \
FeOS_IrqDisable \
FeOS_IrqEnable \
FeOS_ConsoleMode \
FeOS_DirectMode \
FeOS_GetMode \
dmaCopyWords \
dmaCopyHalfWords \
dmaFillWords \
dmaFillHalfWords \
vramSetPrimaryBanks \
vramSetBanks_EFG \
vramRestorePrimaryBanks \
vramRestoreBanks_EFG \
vramSetBankA \
vramSetBankB \
vramSetBankC \
vramSetBankD \
vramSetBankE \
vramSetBankF \
vramSetBankG \
vramSetBankH \
vramSetBankI \
videoSetMode \
videoSetModeSub \
videoGetMode \
videoGetModeSub \
video3DEnabled \
setBrightness

SFILES := $(addsuffix .s, $(FUNCS))

.PHONY: all clean

all: $(SFILES)

%.s:
	@echo $@
	@printf ".section .imp.%b, \"ax\", %%progbits\n" $(MODULE) > $@
	@printf ".global __imp_%b\n" $(@:.s=) >> $@
	@printf ".hidden __imp_%b\n" $(@:.s=) >> $@
	@printf ".global %b\n" $(@:.s=) >> $@
	@printf ".hidden %b\n" $(@:.s=) >> $@
	@printf "%b:\n" $(@:.s=) >> $@
	@printf "\tldr pc, [pc, #-4]\n" >> $@
	@printf "__imp_%b:\n" $(@:.s=) >> $@
	@printf "\t.word 0" >> $@

clean:
	@rm -fr $(SFILES)
