# help
CC = gcc
CFLAGS = -Os
LDFLAGS = -flto

ifeq ($(OS),Windows_NT)
	SHELL = cmd.exe
	RM = del /f 2>nul
	EXECUTABLE = CEromupdater.exe
else
	EXECUTABLE = CEromupdater
	RM = rm -f
endif

SOURCES = main.c

OBJECTS := $(SOURCES:.c=.o)

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

clean:
	$(RM) $(EXECUTABLE) $(OBJECTS)
