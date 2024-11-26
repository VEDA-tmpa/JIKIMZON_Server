# 컴파일러 설정
CXX = clang++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic

# pkg-config를 통해 플래그 자동 설정
PKG_CONFIG = pkg-config
PKG_CFLAGS = $(shell $(PKG_CONFIG) --cflags nlohmann_json)
PKG_LDFLAGS = $(shell $(PKG_CONFIG) --libs nlohmann_json)

# 타겟 이름
TARGET = json_example.out

# 소스 파일
SRCS = json_example.cpp

# Makefile 규칙
all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(PKG_CFLAGS) $(PKG_LDFLAGS) -o $@ $^

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
