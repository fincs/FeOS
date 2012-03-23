.SUFFIXES:

MODULE := $(shell basename $(CURDIR))

FUNCS := \
bgSetRotate \
bgRotate \
bgSet \
bgSetRotateScale \
bgSetScale \
bgInit \
bgInitSub \
bgUpdate \
bgSetControlBits \
bgClearControlBits \
bgSetPriority \
bgSetMapBase \
bgSetTileBase \
bgSetScrollf \
bgMosaicEnable \
bgMosaicDisable \
bgSetMosaic \
bgSetMosaicSub \
bgGetMapPtr \
bgGetGfxPtr \
bgGetPriority \
bgGetMapBase \
bgGetTileBase \
bgScrollf \
bgShow \
bgHide \
bgSetCenterf \
bgSetAffineMatrixScroll

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
