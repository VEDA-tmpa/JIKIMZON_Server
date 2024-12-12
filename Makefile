.PHONY: all clean run

# 프로젝트 루트 디렉토리 변수 설정
PROJECT_ROOT := $(shell pwd)

# All target: 빌드와 실행을 포함
all: build run

# Build target: 하위 디렉토리의 Makefile 실행
build: cctv-video cctv-json viewer-video viewer-json

# cctv/video/Makefile 실행
cctv-video:
	$(MAKE) -C $(PROJECT_ROOT)/cctv/video

# cctv/json/Makefile 실행
cctv-json:
	$(MAKE) -C $(PROJECT_ROOT)/cctv/json

# viewer/video/Makefile 실행
viewer-video:
	$(MAKE) -C $(PROJECT_ROOT)/viewer/video

# viewer/json/Makefile 실행
viewer-json:
	$(MAKE) -C $(PROJECT_ROOT)/viewer/json

# Run target: 빌드된 실행 파일 순차적으로 실행
run:
	@echo "Running cctv/video executable..."
	$(PROJECT_ROOT)/cctv/video/main.out &
	@echo "Running cctv/json executable..."
	$(PROJECT_ROOT)/cctv/json/main.out &
	@echo "Running viewer/video executable..."
	$(PROJECT_ROOT)/viewer/video/main.out &
	@echo "Running viewer/json executable..."
	$(PROJECT_ROOT)/viewer/json/main.out &

# Clean target: 하위 디렉토리의 clean 호출
clean:
	$(MAKE) -C $(PROJECT_ROOT)/cctv/video clean
	$(MAKE) -C $(PROJECT_ROOT)/cctv/json clean
	$(MAKE) -C $(PROJECT_ROOT)/viewer/video clean
	$(MAKE) -C $(PROJECT_ROOT)/viewer/json clean
