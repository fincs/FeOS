
include $(FEOSMK)/vars.mk

export PATH := $(FEOSBIN):$(DEVKITARM)/bin:$(PATH)

include $(FEOSMK)/package.mk
