.SUFFIXES:

MODULE := $(shell basename $(CURDIR))

FUNCS := \
FeOS_GetStdin \
FeOS_GetStdout \
FeOS_GetStderr \
FeOS_SetStdin \
FeOS_SetStdout \
FeOS_SetStderr \
FeOS_OpenStream \
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
setvbuf \
vfprintf \
vsprintf \
vsnprintf \
vfscanf \
vsscanf \
fgetc \
fputc \
fgets \
fputs \
ungetc \
remove \
rename \
getdelim

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
