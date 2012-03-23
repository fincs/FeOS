.SUFFIXES:

MODULE := $(shell basename $(CURDIR))

FUNCS := \
sinLerp \
cosLerp \
tanLerp \
asinLerp \
acosLerp \
FeOS_div3232 \
FeOS_mod3232 \
FeOS_div6432 \
FeOS_mod6432 \
FeOS_div6464 \
FeOS_mod6464 \
FeOS_sqrt32 \
FeOS_sqrt64

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
