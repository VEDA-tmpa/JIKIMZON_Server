# 프로젝트 루트 디렉토리 변수 설정
PROJECT_ROOT := $(shell cd ../.. && pwd)

# Include path에 PROJECT_ROOT 추가
INCLUDES := -I$(PROJECT_ROOT) -DPROJECT_ROOT=\"$(PROJECT_ROOT)\"

# 컴파일러와 플래그 설정
CXX := g++
CXXFLAGS := -std=c++17 $(shell pkg-config --cflags openssl)
LDFLAGS = $(shell pkg-config --libs openssl)

# 소스 파일과 타겟 설정
SOURCES := $(PROJECT_ROOT)/common/log/Logger.cpp \
		   $(PROJECT_ROOT)/common/tcp/server/BaseServer.cpp \
		   $(PROJECT_ROOT)/common/frame/Frame.cpp \
		   $(PROJECT_ROOT)/common/frame/Header.cpp \
		   $(PROJECT_ROOT)/common/frame/Body.cpp \
		   $(PROJECT_ROOT)/common/cipher/ChaCha20.cpp \
		   $(PROJECT_ROOT)/common/cipher/ChaCha20CipherAdapter.cpp \
		   $(PROJECT_ROOT)/common/cipher/TlsServerHandler.cpp \
		   $(PROJECT_ROOT)/viewer/video/main.cpp \
           $(PROJECT_ROOT)/viewer/video/VideoServer.cpp \
           $(PROJECT_ROOT)/test/fixture/Fixture.cpp 

TARGET := main.out

# 빌드 규칙
all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $^ -o $@ $(LDFLAGS)

# 클린 규칙
clean:
	rm -f $(TARGET)
