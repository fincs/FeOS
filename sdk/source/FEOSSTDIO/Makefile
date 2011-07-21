.SUFFIXES:

MODULE := $(shell basename $(CURDIR))

FUNCS := \
FeOS_GetStdin \
FeOS_GetStdout \
FeOS_GetStderr \
fopen \
freopen \
fclose \
fwrite \
fread \
feof \
fseek \
ftell \
fflush \
ferror \
clearerr \
vfprintf \
vsprintf \
vfscanf \
vsscanf \
fgetc \
fputc \
fgets \
fputs \
remove \
rename \

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
