# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++17 -Wall -pthread

# Target executable
TARGET = stogui

# Source files
SRCS = main.cpp server.cpp

# Object files (replace .cpp with .o for each source file)
OBJS = $(SRCS:.cpp=.o)

# Default target
all: $(TARGET)

# Link object files to create the executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Compile source files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up object files and executable
clean:
	rm -f $(OBJS) $(TARGET)

