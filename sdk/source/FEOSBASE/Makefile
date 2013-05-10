.SUFFIXES:

MODULE := $(shell basename $(CURDIR))

FUNCS := \
FeOS_LoadModule \
FeOS_FindSymbol \
FeOS_FreeModule \
FeOS_LockModule \
FeOS_UnlockModule \
FeOS_ModuleGetExtraSize \
FeOS_ModuleExtraRead \
FeOS_ModuleExtraSeek \
FeOS_ModuleExtraTell \
FeOS_EnumModules \
FeOS_GetModule \
FeOS_GetModuleName \
FeOS_Execute \
DC_FlushRange \
DC_FlushAll \
IC_InvalidateRange \
IC_InvalidateAll \
FeOS_DrainWriteBuffer \
FeOS_WaitForMemAddr \
FeOS_SetSuspendMode \
FeOS_PushExitFunc \
FeOS_PopExitFunc \
FeOS_GetErrnoPtr \
FeOS_GetModuleExidxTbl \
FeOS_ModuleFromAddress \
FeOS_ExecStatusCreate \
FeOS_ExecStatusAddRef \
FeOS_ExecStatusRelease \
FeOS_SetCurExecStatus \
FeOS_GetCurExecStatus \
FeOS_SetAutoUpdate \
FeOS_GetAutoUpdate \
FeOS_GetDiskStats \
FeOS_GetDiskStats64 \
FeOS_GetMemStats \
FeOS_GetTickCount \
FeOS_GetVersion \
FeOS_GetSystemInfo \
__aeabi_idiv \
__aeabi_idivmod \
__aeabi_uidiv \
__aeabi_uidivmod \
__aeabi_ldivmod \
__aeabi_uldivmod \
setjmp \
longjmp \
atoi \
strtol \
strtoul \
rand \
srand \
malloc \
memalign \
calloc \
free \
realloc \
exit \
bsearch \
qsort \
abs \
div \
system \
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
	@printf ".type %b STT_FUNC\n" $(@:.s=) >> $@
	@printf "%b:\n" $(@:.s=) >> $@
	@printf "\tldr pc, [pc, #-4]\n" >> $@
	@printf "__imp_%b:\n" $(@:.s=) >> $@
	@printf "\t.word 0" >> $@

clean:
	@rm -fr $(SFILES)
