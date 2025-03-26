# EnvironmentSwitcher - Clean output to /building/
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

.PHONY: all clean

all: $(OUTDIR)/$(TARGET).rpx

$(BUILD):
	mkdir -p $@

$(OUTDIR):
	mkdir -p $@

$(BUILD)/%.o: $(SOURCE)/%.c | $(BUILD)
	$(CC) $(CFLAGS) -c -o $@ $<

$(OUTDIR)/$(TARGET).rpx: $(OBJS) | $(OUTDIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
	cp $(METADIR)/meta.xml $(OUTDIR)/ || true
	cp icon.png $(OUTDIR)/ || true

clean:
	rm -rf $(BUILD) $(OUTDIR) *.rpx
