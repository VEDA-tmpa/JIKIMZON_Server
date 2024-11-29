# 프로젝트 루트 디렉토리 변수 설정
PROJECT_ROOT := $(shell cd ../../.. && pwd)

# Include 경로 설정
INCLUDES := -I$(PROJECT_ROOT) -DPROJECT_ROOT=\"$(PROJECT_ROOT)\"

# 컴파일러 및 플래그 설정
CXX := g++
CXXFLAGS := -std=c++17

# 소스 파일 설정
SOURCES := $(PROJECT_ROOT)/common/frame/Frame.cpp \
		   $(PROJECT_ROOT)/common/frame/Header.cpp \
		   $(PROJECT_ROOT)/common/frame/Body.cpp \
           $(PROJECT_ROOT)/test/tutorial/serialize/main.cpp

# 타겟 파일 설정
TARGET := a.out

# 기본 빌드 규칙
all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $^ -o $@

# Clean 규칙
clean:
	rm -f $(TARGET)

# Debug 빌드 규칙
debug: CXXFLAGS += -g
debug: clean all
