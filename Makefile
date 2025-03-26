# EnvironmentSwitcher - WHB-enabled Wii U homebrew
TARGET     := environmentswitcher
BUILD      := build
SOURCE     := source
INCLUDE    := include
APPDIR     := wiiu/apps/$(TARGET)

PREFIX     := /opt/devkitpro/devkitPPC/bin/powerpc-eabi-
CC         := $(PREFIX)gcc

CFLAGS     := -Wall -O2 -mcpu=750 -meabi -mhard-float
CFLAGS     += -I$(INCLUDE) -I/opt/devkitpro/wut/include

LDFLAGS    := -L/opt/devkitpro/wut/lib -lwut

SRCS       := $(wildcard $(SOURCE)/*.c)
OBJS       := $(patsubst $(SOURCE)/%.c,$(BUILD)/%.o,$(SRCS))

.PHONY: all clean install

all: $(BUILD) $(TARGET).rpx install

$(BUILD):
	mkdir -p $@

$(BUILD)/%.o: $(SOURCE)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(TARGET).rpx: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

install:
	mkdir -p $(APPDIR)
	cp $(TARGET).rpx $(APPDIR)/
	cp meta.xml $(APPDIR)/ || true
	cp icon.png $(APPDIR)/ || true

clean:
	rm -rf $(BUILD) *.rpx $(APPDIR)
