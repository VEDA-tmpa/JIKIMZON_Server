# 컴파일러와 플래그 설정
CXX = g++
CXXFLAGS = -std=c++17 -I./ -DPROJECT_ROOT=\"./\" -Wall -Wextra

# 링크할 라이브러리
LDFLAGS = -lstdc++fs

# 바이너리 이름
TARGET = StorageTest

# 소스 파일 리스트
SRC = main.cpp \
      storage/StorageManager.cpp \
      common/frame/Frame.cpp \
      common/frame/Body.cpp \
      common/frame/Header.cpp \
      common/log/Logger.cpp

# 오브젝트 파일 리스트 (SRC에서 .cpp를 .o로 변경)
OBJ = $(SRC:.cpp=.o)

# 기본 규칙: 컴파일과 링크
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# 개별 소스 파일에 대한 컴파일 규칙
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# 청소 규칙: 빌드된 파일 삭제
clean:
	rm -f $(OBJ) $(TARGET)

# 디렉토리 청소 규칙
cleanall: clean
	rm -f storage/video.bin storage/log.txt

