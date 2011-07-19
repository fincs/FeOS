.SUFFIXES:

MODULE := $(shell basename $(CURDIR))

FUNCS := \
FeOS_FindSymbol \
atoi \
rand \
srand \
malloc \
calloc \
free \
relloc \
bsearch \
qsort \
abs \
div \
strlen \
strcpy \
strcat \
strcmp \
strncpy \
strncat \
strncmp \
memcpy \
memmove \
memset \
memcmp \
printf \
sprintf \

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
	@printf "\tldr r12, [pc]\n" >> $@
	@printf "\tbx r12\n" >> $@
	@printf "__imp_%b:\n" $(@:.s=) >> $@
	@printf "\t.word 0" >> $@

clean:
	@rm -fr $(SFILES)
