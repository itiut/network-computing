TARGETS := httpd_fork httpd_select
TARGET_SOURCES := $(addsuffix .c, $(TARGETS))
TARGET_OBJECTS := $(TARGET_SOURCES:.c=.o)
SOURCES := $(filter-out $(TARGET_SOURCES), $(wildcard *.c))
OBJECTS := $(SOURCES:.c=.o)

CC := gcc
CFLAGS := -Wall -O2 -std=c99

.SUFFIXES: .c .o

.c.o:
	$(CC) $(CFLAGS) -c $<

.PHONY: all clean

all: $(TARGETS) $(TARGET_OBJECTS) $(OBJECTS)

$(TARGETS): $(TARGET_OBJECTS) $(OBJECTS)
	$(CC) -o $@ $@.o $(OBJECTS)

clean:
	rm -f $(TARGETS) $(TARGET_OBJECTS) $(OBJECTS)
