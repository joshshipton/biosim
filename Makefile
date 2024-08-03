# Makefile

# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -g

# Source files
SOURCES = main.c neural_network.c genetic.c

# Object files
OBJECTS = $(SOURCES:.c=.o)

# Executable name
EXECUTABLE = biosim

# Default target
all: $(EXECUTABLE)

# Link the target executable
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(EXECUTABLE) $(OBJECTS) -lm

# Compile source files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up
clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

# Phony targets
.PHONY: all clean
