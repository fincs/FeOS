.SUFFIXES:

MODULE := $(shell basename $(CURDIR))

FUNCS := \
FeOS_GetKeysDown \
FeOS_GetKeysHeld \
FeOS_GetKeysUp \
FeOS_GetKeysDownRepeat \
FeOS_SetKeyRepeat \
FeOS_KeysUpdate \
FeOS_GetStylusPos \
FeOS_SetInterrupt \
FeOS_CheckPendingIRQs \
FeOS_WaitForIRQ \
FeOS_SetIRQWaitFunc \
FeOS_NextIRQ \
FeOS_IrqDisable \
FeOS_IrqEnable \
FeOS_ConsoleMode \
FeOS_DirectMode \
FeOS_TimerWrite \
FeOS_TimerTick \
FeOS_GetMode \
FeOS_ModeShim \
FeOS_VideoReset \
dmaCopyWords \
dmaCopyHalfWords \
dmaFillWords \
dmaFillHalfWords \
memUncached \
memCached \
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
setBrightness \
lcdSwap \
lcdMainOnTop \
lcdMainOnBottom \
powerOn \
powerOff

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
	@printf ".type %b STT_FUNC\n" $(@:.s=) >> $@
	@printf "%b:\n" $(@:.s=) >> $@
	@printf "\tldr pc, [pc, #-4]\n" >> $@
	@printf "__imp_%b:\n" $(@:.s=) >> $@
	@printf "\t.word 0" >> $@

clean:
	@rm -fr $(SFILES)
