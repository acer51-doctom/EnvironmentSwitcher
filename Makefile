TARGET     := environmentswitcher
BUILD      := build
SOURCE     := source
INCLUDE    := include
META       := meta

PREFIX     := /opt/devkitpro/devkitPPC/bin/powerpc-eabi-
CC         := $(PREFIX)gcc

CFLAGS     := -Wall -O2 -mcpu=750 -meabi -mhard-float
CFLAGS     += -I$(INCLUDE) -I/opt/devkitpro/wut/include

LDFLAGS    := -L/opt/devkitpro/wut/lib -lwut

SRCS       := $(wildcard $(SOURCE)/*.c)
OBJS       := $(patsubst $(SOURCE)/%.c,$(BUILD)/%.o,$(SRCS))

.PHONY: all clean

all: $(BUILD) $(TARGET).rpx

$(BUILD):
	mkdir -p $@

$(BUILD)/%.o: $(SOURCE)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(TARGET).rpx: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
	mkdir -p $(TARGET)/meta
	cp -r $(META)/* $(TARGET)/meta/

clean:
	rm -rf $(BUILD) *.rpx $(TARGET)