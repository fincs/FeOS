.SUFFIXES:

MODULE := $(shell basename $(CURDIR))

FUNCS := \
swiWaitForVBlank \
irqSet \
irqClear \
irqEnable \
irqDisable \
writePowerManagement \
timerStart \
timerPause \
timerStop \
timerElapsed \
fifoSendAddress \
fifoSendValue32 \
fifoSendDatamsg \
fifoSetAddressHandler \
fifoSetValue32Handler \
fifoSetDatamsgHandler \
fifoCheckAddress \
fifoCheckValue32 \
fifoCheckDatamsg \
fifoGetAddress \
fifoGetValue32 \
fifoGetDatamsg \
swiDelay \
swiIntrWait \
memcpy \
memcmp \
memset \
strcpy \
strcmp \
strncpy \
strncmp \
malloc \
free \
__aeabi_idiv \
__aeabi_idivmod \
__aeabi_uidiv \
__aeabi_uidivmod \
__aeabi_ldivmod \
__aeabi_uldivmod \
__aeabi_lmul \
coopFifoSetDatamsgHandler \
coopFifoSetValue32Handler \
coopFifoSetAddressHandler \
coopIrqSet \
coopIrqCheck \
coopIrqNext \
coopTimerStart

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
	@printf "\tldr r12, [pc]\n" >> $@
	@printf "\tbx  r12\n" >> $@
	@printf "__imp_%b:\n" $(@:.s=) >> $@
	@printf "\t.word 0" >> $@

clean:
	@rm -fr $(SFILES)
