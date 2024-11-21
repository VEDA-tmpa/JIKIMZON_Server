# Makefile

# 변수 정의
KEY_FILE = keyfile.bin
KEY_LENGTH = 32
LOG_FILE = key_generation.log

# 기본 타겟 (make 실행 시 자동으로 이 타겟이 실행됨)
all: generate_key

# 키 파일 생성 타겟
generate_key: $(KEY_FILE)

$(KEY_FILE): 
	@echo "Generating key file: $(KEY_FILE)"
	openssl rand -out $(KEY_FILE) $(KEY_LENGTH) && \
	echo "Key file $(KEY_FILE) created." >> $(LOG_FILE) && \
	hexdump -v -e '/1 "%02X " "\n"' $(KEY_FILE) && \
	chmod 600 $(KEY_FILE)

# 클린 타겟
clean:
	@echo "Cleaning up..."
	@rm -f $(KEY_FILE) $(LOG_FILE)

.PHONY: all generate_key clean
