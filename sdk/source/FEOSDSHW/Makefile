.SUFFIXES:

MODULE := $(shell basename $(CURDIR))

FUNCS := \
DSConsoleMode \
DSDirectMode \
DSFreeARM7 \
DSGetMainOAM \
DSGetMode \
DSGetOAMMemory \
DSGetSubOAM \
DSLoadARM7 \
DSModeShim \
DSProcessIRQs \
DSSetIRQWaitFunc \
DSSetSuspendMode \
DSTimerTick \
DSTimerWrite \
DSVideoReset \
DSWaitForIRQ \
DSWaitForNextIRQ \
bgClearControlBits \
bgExtPaletteDisable \
bgExtPaletteDisableSub \
bgExtPaletteEnable \
bgExtPaletteEnableSub \
bgGetGfxPtr \
bgGetMapBase \
bgGetMapPtr \
bgGetPriority \
bgGetTileBase \
bgHide \
bgInit \
bgInitSub \
bgMosaicDisable \
bgMosaicEnable \
bgRotate \
bgScrollf \
bgSet \
bgSetAffineMatrixScroll \
bgSetCenterf \
bgSetControlBits \
bgSetMapBase \
bgSetMosaic \
bgSetMosaicSub \
bgSetPriority \
bgSetRotate \
bgSetRotateScale \
bgSetScale \
bgSetScrollf \
bgSetTileBase \
bgShow \
bgUpdate \
dmaCopyHalfWords \
dmaCopyWords \
dmaFillHalfWords \
dmaFillWords \
fifoCheckAddress \
fifoCheckDatamsg \
fifoCheckDatamsgLength \
fifoCheckValue32 \
fifoGetAddress \
fifoGetDatamsg \
fifoGetRetValue32 \
fifoGetValue32 \
fifoSendAddress \
fifoSendDatamsg \
fifoSendValue32 \
fifoSetAddressHandler \
fifoSetDatamsgHandler \
fifoSetValue32Handler \
irqDisable \
irqEnable \
irqSet \
keysDown \
keysDownRepeat \
keysHeld \
keysSetRepeat \
keysUp \
lcdMainOnBottom \
lcdMainOnTop \
lcdSwap \
memCached \
memUncached \
oamAffineTransformation \
oamAllocateGfx \
oamClear \
oamClearSprite \
oamDisable \
oamEnable \
oamFreeGfx \
oamGetGfxPtr \
oamGfxPtrToOffset \
oamInit \
oamRotateScale \
oamSet \
oamSetMosaic \
oamSetMosaicSub \
oamUpdate \
powerOff \
powerOn \
scanKeys \
setBrightness \
touchRead \
video3DEnabled \
videoGetMode \
videoGetModeSub \
videoSetMode \
videoSetModeSub \
vramRestoreBanks_EFG \
vramRestorePrimaryBanks \
vramSetBankA \
vramSetBankB \
vramSetBankC \
vramSetBankD \
vramSetBankE \
vramSetBankF \
vramSetBankG \
vramSetBankH \
vramSetBankI \
vramSetBanks_EFG \
vramSetPrimaryBanks

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
