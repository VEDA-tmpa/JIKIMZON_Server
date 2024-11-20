# 컴파일러와 플래그
CXX = g++
CXXFLAGS = -std=c++11 $(shell pkg-config --cflags openssl)
LDFLAGS = $(shell pkg-config --libs openssl)

# 대상 파일
TARGET = a.out
SRC = chacha20_example.cpp

# 기본 빌드 대상
all: $(TARGET)

# 프로그램 빌드 규칙
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

# 정리 규칙
clean:
	rm -f $(TARGET)

# 편리한 디버그 규칙
debug: CXXFLAGS += -g
debug: clean $(TARGET)
