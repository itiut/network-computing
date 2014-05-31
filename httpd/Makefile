PROGRAM := httpd_fork
OBJECTS := httpd_fork.o safe_functions.o server.o

CC := gcc
CFLAGS := -Wall -O2 -std=c99

.SUFFIXES: .c .o

.c.o:
	$(CC) $(CFLAGS) -c $<

.PHONY: all clean

all: $(PROGRAM) $(OBJECTS)

$(PROGRAM): $(OBJECTS)
	$(CC) -o $(PROGRAM) $^

clean:
	rm -f $(PROGRAM) $(OBJECTS)
