PROJECT_ROOT := $(shell cd ../../.. && pwd)

# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 $(shell pkg-config --cflags openssl) -I$(PROJECT_ROOT)
LDFLAGS = $(shell pkg-config --libs openssl)

# Target binary
TARGET = a.out

# Source files
SRCS := $(PROJECT_ROOT)/test/tutorial/cipher/chacha20_example.cpp \
        $(PROJECT_ROOT)/common/cipher/Cipher.cpp

# Default target
all: $(TARGET)

# Build target
$(TARGET):
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET) $(LDFLAGS)

# Clean build artifacts
clean:
	rm -f $(TARGET)

# Run the program
run: $(TARGET)
	./$(TARGET)

# Debug target to print project paths
debug:
	@echo "Project root: $(PROJECT_ROOT)"
	@echo "Source files: $(SRCS)"
