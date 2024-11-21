# 프로젝트 루트 디렉토리 변수 설정
PROJECT_ROOT := $(shell cd .. && pwd)

# Include path와 프로젝트 루트 설정
INCLUDES := -I$(PROJECT_ROOT) -DPROJECT_ROOT=\"$(PROJECT_ROOT)\"

# 컴파일러와 플래그 설정
CXX := g++
CXXFLAGS := -std=c++17 $(shell pkg-config --cflags openssl) $(INCLUDES)
LDFLAGS := $(shell pkg-config --libs openssl)

# 소스 파일 및 타겟 설정
SOURCES := $(PROJECT_ROOT)/common/log/Logger.cpp \
           $(PROJECT_ROOT)/common/cipher/ChaCha20.cpp \
           $(PROJECT_ROOT)/viewer/main.cpp \
           $(PROJECT_ROOT)/viewer/Server.cpp \
           $(PROJECT_ROOT)/test/fixture/Fixture.cpp

TARGET := main.out

# 빌드 규칙
all: $(TARGET)

$(TARGET):
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(SOURCES) -o $@

# 클린 규칙
clean:
	rm -f $(TARGET)
