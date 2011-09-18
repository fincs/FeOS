.SUFFIXES:

MODULE := $(shell basename $(CURDIR))

FUNCS := \
FeOS_FifoSendAddress \
FeOS_FifoSendValue32 \
FeOS_FifoSendDatamsg \
FeOS_FifoCheckAddress \
FeOS_FifoCheckValue32 \
FeOS_FifoCheckDatamsg \
FeOS_FifoGetAddress \
FeOS_FifoGetValue32 \
FeOS_FifoGetDatamsg

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
