# 프로젝트 루트 디렉토리 변수 설정
PROJECT_ROOT := $(shell cd ../../.. && pwd)

# Include path에 PROJECT_ROOT 추가
INCLUDES := -I$(PROJECT_ROOT) -DPROJECT_ROOT=\"$(PROJECT_ROOT)\"

# 컴파일러와 플래그 설정
CXX := g++
CXXFLAGS := -std=c++17 $(shell pkg-config --cflags nlohmann_json)
LDFLAGS = $(shell pkg-config --libs nlohmann_json)

# 소스 파일과 타겟 설정
SOURCES := $(PROJECT_ROOT)/common/log/Logger.cpp \
		   $(PROJECT_ROOT)/common/objectDetection/Object.cpp \
		   $(PROJECT_ROOT)/common/objectDetection/Frame.cpp \
		   $(PROJECT_ROOT)/test/objectDetection/Json/main.cpp 

TARGET := main.out

# 빌드 규칙
all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $^ -o $@ $(LDFLAGS)

clean:
	rm -f $(TARGET)
