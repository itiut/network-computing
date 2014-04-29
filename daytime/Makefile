PROGRAM := daytime
OBJECTS := daytime.o

CC := gcc
CFLAGS := -Wall -O2

.SUFFIXES: .c .o

.c.o:
	$(CC) $(CFLAGS) -c $<

.PHONY: all clean

all: $(PROGRAM) $(OBJECTS)

$(PROGRAM): $(OBJECTS)
	$(CC) -o $(PROGRAM) $^

clean:
	rm -f $(PROGRAM) $(OBJECTS)
