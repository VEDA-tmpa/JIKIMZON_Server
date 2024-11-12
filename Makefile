# 프로젝트 루트 디렉토리 변수 설정
PROJECT_ROOT := $(pwd)

# Include path에 PROJECT_ROOT 추가
INCLUDES := -I$(PROJECT_ROOT)/cctv

# 컴파일러와 플래그 설정
CXX := g++
CXXFLAGS := -std=c++17

# 소스 파일과 타겟 설정
SOURCES := main.cpp \
           cctv/Client.cpp

TARGET := main.out

# 빌드 규칙
all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $^ -o $@

# 클린 규칙
clean:
	rm -f $(TARGET)
