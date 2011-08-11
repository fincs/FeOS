.SUFFIXES:

MODULE := $(shell basename $(CURDIR))

FUNCS := \
FeOS_LoadModule \
FeOS_FindSymbol \
FeOS_FreeModule \
FeOS_Execute \
FeOS_WaitForVBlank \
FeOS_DebugPrint \
FeOS_PushExitFunc \
FeOS_PopExitFunc \
FeOS_GetErrnoPtr \
FeOS_GetModuleExidxTbl \
__aeabi_idiv \
__aeabi_idivmod \
__aeabi_uidiv \
__aeabi_uidivmod \
__aeabi_ldivmod \
__aeabi_uldivmod \
atoi \
strtol \
strtoul \
rand \
srand \
malloc \
calloc \
free \
realloc \
exit \
bsearch \
qsort \
abs \
div \
strlen \
strcpy \
strcat \
strcmp \
stricmp \
strncpy \
strncat \
strncmp \
strnicmp \
memcpy \
memmove \
memset \
memcmp \
memchr \
strchr \
strcspn \
strpbrk \
strrchr \
strspn \
strstr \
strtok \
strerror \
isalnum \
isalpha \
iscntrl \
isdigit \
isgraph \
islower \
isprint \
ispunct \
isspace \
isupper \
isxdigit \
tolower \
toupper \
mktime \
time \
asctime \
ctime \
gmtime \
localtime \
strftime

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
