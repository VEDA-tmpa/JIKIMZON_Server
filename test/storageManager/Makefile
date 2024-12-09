# 프로젝트 루트 디렉토리 변수 설정
PROJECT_ROOT := $(shell cd ../.. && pwd)

# Include 경로 설정
INCLUDES := -I$(PROJECT_ROOT) -DPROJECT_ROOT=\"$(PROJECT_ROOT)\"

# 컴파일러 설정
CC = g++
CFLAGS = -Wall -std=c++17

# 소스 파일 설정
SRCS := $(PROJECT_ROOT)/common/frame/Frame.cpp \
		$(PROJECT_ROOT)/common/frame/Header.cpp \
		$(PROJECT_ROOT)/common/frame/Body.cpp \
		$(PROJECT_ROOT)/common/log/Logger.cpp \
		$(PROJECT_ROOT)/storage/StorageFile.cpp \
		$(PROJECT_ROOT)/storage/StorageManager.cpp \
		$(PROJECT_ROOT)/test/storageMAnager/main.cpp 

# 실행 파일 이름
TARGET = a.out

# 기본 실행
all: $(TARGET)

# 타겟 빌드
$(TARGET):
	$(CC) $(CFLAGS) $(INCLUDES) $(SRCS) -o $(TARGET)

# 실행 명령
run: $(TARGET)
	./$(TARGET)

# 클린업
clean:
	rm -f $(TARGET)
