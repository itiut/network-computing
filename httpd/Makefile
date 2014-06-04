TARGETS := httpd_fork httpd_select httpd_thread
TARGET_SOURCES := $(addsuffix .c, $(TARGETS))
TARGET_OBJECTS := $(TARGET_SOURCES:.c=.o)
SOURCES := $(filter-out $(TARGET_SOURCES), $(wildcard *.c))
OBJECTS := $(SOURCES:.c=.o)
TAR_DIR := httpd
TAR := /tmp/$(TAR_DIR).tar.gz

CC := gcc
CFLAGS := -Wall -O2 -std=c99 -lpthread

.SUFFIXES: .c .o

.c.o:
	$(CC) -c $< $(CFLAGS)

.PHONY: all clean

all: $(TARGETS) $(TARGET_OBJECTS) $(OBJECTS)

$(TARGETS): $(TARGET_OBJECTS) $(OBJECTS)
	$(CC) -o $@ $@.o $(OBJECTS) $(CFLAGS)

clean:
	rm -f $(TARGETS) $(TARGET_OBJECTS) $(OBJECTS)

tar:
	make clean
	rm -f $(TAR)
	cd ../;	tar czf $(TAR) $(TAR_DIR)
	@echo "$(TAR) craeted."
