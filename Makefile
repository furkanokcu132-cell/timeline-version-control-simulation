# --- COMP201 BTA Simulator Makefile ---

CC = gcc
CFLAGS = -Wall -Wextra -g -std=c99
TARGET = bta_simulator
SRCS = main.c timeline.c
OBJS = $(SRCS:.c=.o)

# Default target when you just type 'make'
all: $(TARGET) 1 2

# Link the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Compile source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# --- SIMULATION TARGETS ---

# Run Simulation 1
1: $(TARGET)
	@echo "\n========================================"
	@echo "      RUNNING SIMULATION 1 (BASIC)      "
	@echo "========================================"
	./$(TARGET) sim1.txt

# Run Simulation 2
2: $(TARGET)
	@echo "\n========================================"
	@echo "    RUNNING SIMULATION 2 (ADVANCED)     "
	@echo "========================================"
	./$(TARGET) sim2.txt

# --- VALGRIND TARGETS (Crucial for Memory Check) ---

# Here you can define Valgrind commands for easy usage.

# Clean compiled files
clean:
	rm -f $(OBJS) $(TARGET)

# Declare non-file targets
.PHONY: all clean 1 2 # valgrind1 valgrind2