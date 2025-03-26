# Makefile for EnvironmentSwitcher (WUT)
TARGET     := environmentswitcher
BUILD      := build
SOURCE     := source
INCLUDE    := include
DATA       := data
META       := meta
APP_TITLE  := EnvironmentSwitcher

PREFIX     := $(DEVKITPPC)/bin/powerpc-eabi-
CC         := $(PREFIX)gcc

CFLAGS     := -Wall -O2 -mcpu=750 -meabi -mhard-float
CFLAGS     += $(shell wut-config --cflags)
LDFLAGS    := $(shell wut-config --libs)

SRCS       := $(wildcard $(SOURCE)/*.c)
OBJS       := $(patsubst $(SOURCE)/%.c,$(BUILD)/%.o,$(SRCS))

all: $(BUILD) $(TARGET).rpx

$(BUILD):
	mkdir -p $@

$(BUILD)/%.o: $(SOURCE)/%.c
	$(CC) $(CFLAGS) -I$(INCLUDE) -c -o $@ $<

$(TARGET).rpx: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
	cp -r $(META) $(@:.rpx=)/meta

clean:
	rm -rf $(BUILD) *.rpx
