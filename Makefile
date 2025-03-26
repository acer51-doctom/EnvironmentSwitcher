# EnvironmentSwitcher - WHB-enabled Wii U homebrew
TARGET     := environmentswitcher
BUILD      := build
SOURCE     := source
INCLUDE    := include
OUTDIR     := building
METADIR    := meta

PREFIX     := /opt/devkitpro/devkitPPC/bin/powerpc-eabi-
CC         := $(PREFIX)gcc

CFLAGS     := -Wall -O2 -mcpu=750 -meabi -mhard-float
CFLAGS     += -I$(INCLUDE) -I/opt/devkitpro/wut/include

LDFLAGS    := -L/opt/devkitpro/wut/lib -lwut

SRCS       := $(wildcard $(SOURCE)/*.c)
OBJS       := $(patsubst $(SOURCE)/%.c,$(BUILD)/%.o,$(SRCS))

.PHONY: all clean install

all: $(BUILD) $(OUTDIR) $(OUTDIR)/$(TARGET).rpx install

$(BUILD):
	mkdir -p $@

$(OUTDIR):
	mkdir -p $@

$(BUILD)/%.o: $(SOURCE)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(OUTDIR)/$(TARGET).rpx: $(OBJS)
