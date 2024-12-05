# 프로젝트 루트 디렉토리 변수 설정
PROJECT_ROOT := $(shell cd ../../.. && pwd)

# Include path에 PROJECT_ROOT 추가
INCLUDES := -I$(PROJECT_ROOT) -DPROJECT_ROOT=\"$(PROJECT_ROOT)\"

# 컴파일러와 플래그 설정
CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra $(shell pkg-config --cflags openssl)
LDFLAGS := $(shell pkg-config --libs openssl)

# 서버와 클라이언트 소스 파일 경로 설정
SERVER_SOURCES := $(PROJECT_ROOT)/test/cipher_net/step2-1/server.cpp \
				  $(PROJECT_ROOT)/common/cipher/ChaCha20.cpp \
				  $(PROJECT_ROOT)/common/log/Logger.cpp
CLIENT_SOURCES := $(PROJECT_ROOT)/test/cipher_net/step2-1/client.cpp \
				  $(PROJECT_ROOT)/common/cipher/ChaCha20.cpp \
				  $(PROJECT_ROOT)/common/log/Logger.cpp


# 실행 파일 이름 설정
SERVER_TARGET := server.out
CLIENT_TARGET := client.out

# 기본 타겟
all: $(SERVER_TARGET) $(CLIENT_TARGET)

# 서버 실행 파일 생성 규칙
$(SERVER_TARGET): $(SERVER_SOURCES)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $^ -o $@ $(LDFLAGS)

# 클라이언트 실행 파일 생성 규칙
$(CLIENT_TARGET): $(CLIENT_SOURCES)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $^ -o $@ $(LDFLAGS)

# 클린 규칙
clean:
	rm -f $(SERVER_TARGET) $(CLIENT_TARGET)

# 실행 규칙
run_server: $(SERVER_TARGET)
	./$(SERVER_TARGET)
#	./$(SERVER_TARGET) <port>

run_client: $(CLIENT_TARGET)
	./$(CLIENT_TARGET)
#	./$(CLIENT_TARGET) <server_ip> <port>
