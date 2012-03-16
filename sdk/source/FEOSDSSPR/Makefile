.SUFFIXES:

MODULE := $(shell basename $(CURDIR))

FUNCS := \
FeOS_GetMainOAM \
FeOS_GetSubOAM \
FeOS_GetOAMMemory \
oamInit \
oamUpdate \
oamDisable \
oamEnable \
oamGetGfxPtr \
oamAllocateGfx \
oamFreeGfx \
oamSetMosaic \
oamSetMosaicSub \
oamSet \
oamClear \
oamClearSprite \
oamRotateScale \
oamAffineTransformation \
oamGfxPtrToOffset

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
