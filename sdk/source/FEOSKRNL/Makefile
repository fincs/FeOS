.SUFFIXES:

MODULE := $(shell basename $(CURDIR))

FUNCS := \
DC_DrainWriteBuffer \
DC_FlushAll \
DC_FlushRange \
IC_InvalidateAll \
IC_InvalidateRange \
IoGetStderr \
IoGetStdin \
IoGetStdout \
IoOpenStream \
IoSetStderr \
IoSetStdin \
IoSetStdout \
KeDiv3232 \
KeDiv6432 \
KeDiv6464 \
KeGetDiskStats \
KeGetErrnoPtr \
KeGetMemStats \
KeGetOSVersion \
KeGetSysInfo \
KeGetTickCount \
KeMod3232 \
KeMod6432 \
KeMod6464 \
KeSetExcptHandler \
KeSqrt32 \
KeSqrt64 \
KeWaitForMemAddr \
LdrEnumModules \
LdrExecuteArgv \
LdrFindSymbol \
LdrFreeModule \
LdrGetExidxTbl \
LdrGetModule \
LdrGetModuleName \
LdrLoadModule \
LdrLockModule \
LdrResGetSize \
LdrResRead \
LdrResSeek \
LdrResTell \
LdrResolveAddr \
LdrUnlockModule \
PsCreateFromArgv \
PsCreateFromCmdLine \
ThrCreate \
ThrDetach \
ThrExit \
ThrFree \
ThrGetExitCode \
ThrGetSelf \
ThrIsActive \
ThrJoin \
ThrRunInContext \
ThrSetPriority \
ThrTlsAlloc \
ThrTlsFree \
ThrTlsGetValue \
ThrTlsSetValue \
ThrYield \
__aeabi_idiv \
__aeabi_idivmod \
__aeabi_ldivmod \
__aeabi_uidiv \
__aeabi_uidivmod \
__aeabi_uldivmod \
abs \
acosLerp \
asctime \
asinLerp \
atoi \
bsearch \
calloc \
chdir \
clearerr \
close \
closedir \
cosLerp \
ctime \
div \
exit \
fclose \
fdopen \
feof \
ferror \
fflush \
fgetc \
fgets \
fileno \
fopen \
fputc \
fputs \
fread \
free \
freopen \
fseek \
fstat \
ftell \
ftruncate \
fwrite \
getcwd \
getdelim \
gmtime \
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
localtime \
longjmp \
lseek \
malloc \
memalign \
memchr \
memcmp \
memcpy \
memmove \
memset \
mkdir \
mktime \
open \
opendir \
qsort \
rand \
read \
readdir \
realloc \
remove \
rename \
rewinddir \
seekdir \
setjmp \
setvbuf \
sinLerp \
srand \
stat \
strcat \
strchr \
strcmp \
strcpy \
strcspn \
strerror \
strftime \
stricmp \
strlen \
strncat \
strncmp \
strncpy \
strnicmp \
strpbrk \
strrchr \
strspn \
strstr \
strtok \
strtol \
strtoul \
system \
tanLerp \
telldir \
time \
tolower \
toupper \
ungetc \
vfprintf \
vfscanf \
vsnprintf \
vsprintf \
vsscanf \
write

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
