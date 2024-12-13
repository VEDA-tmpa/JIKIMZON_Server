# server

## build && run
### 설정
- `test/fixture/Fixture.cpp` 에서 cctv 서버의 ip, video port, json port로 설정
- viewer 는 cctv와 동일한 port 들로 구성해둠

### Makefile
- `make buiild`
- `make run`

혹은, 아래의 각 디렉토리에서 make 진행 및 실행 
- `cctv/video`
- `cctv/json`

- `viwer/video`
- `viwer/json`

## 스택

- Language
	- C++
- Libraries
	- openssl (3.0.15)
	- nlohmann_json (3.11.3)

## 기능

- 통신
	- cctv 로 부터 데이터(영상, 객체 탐지 정보(json)) 수신
	- viewer 에게 저장된 데이터 송신
- 저장
	- 제한된 크기의 파일에 순환큐 형태의 저장

## 프로젝트 구조

```
.
├── Makefile
├── README.md
├── cctv
│   ├── json
│   │   ├── JsonClient.cpp
│   │   ├── JsonClient.h
│   │   ├── Makefile
│   │   └── main.cpp
│   └── video
│       ├── Makefile
│       ├── VideoClient.cpp
│       ├── VideoClient.h
│       └── main.cpp
├── common
│   ├── cipher
│   │   ├── ChaCha20.cpp
│   │   ├── ChaCha20.h
│   │   ├── ChaCha20CipherAdapter.cpp
│   │   ├── ChaCha20CipherAdapter.h
│   │   ├── ICiphable.h
│   │   ├── TlsClientHandler.cpp
│   │   ├── TlsClientHandler.h
│   │   ├── TlsServerHandler.cpp
│   │   └── TlsServerHandler.h
│   ├── frame
│   │   ├── Body.cpp
│   │   ├── Body.h
│   │   ├── Frame.cpp
│   │   ├── Frame.h
│   │   ├── GOP.cpp
│   │   ├── GOP.h
│   │   ├── Header.cpp
│   │   ├── Header.h
│   │   └── ISerializable.h
│   ├── log
│   │   ├── Logger.cpp
│   │   └── Logger.h
│   ├── objectDetection
│   │   ├── Frame.cpp
│   │   ├── Frame.h
│   │   ├── IJsonable.h
│   │   ├── Object.cpp
│   │   └── Object.h
│   ├── storage
│   │   └── StorageUtil.h
│   └── tcp
│       ├── NodeType.h
│       ├── client
│       │   ├── BaseClient.cpp
│       │   └── BaseClient.h
│       └── server
│           ├── BaseServer.cpp
│           └── BaseServer.h
├── storage
│   ├── cipher
│   │   ├── Makefile
│   │   ├── key_generation.log
│   │   └── keyfile.bin
│   └── manager
│       ├── StorageManager.h
│       └── database
│           ├── BaseItem.h
│           ├── H264Item.h
│           ├── ItemFactory.h
│           ├── JsonItem.h
│           └── StorageFile.h
├── test
│   ├── cipher_net
│   │   ├── step1
│   │   ├── step2-1
│   │   └── step2-2
│   ├── fixture
│   │   ├── Fixture.cpp
│   │   └── Fixture.h
│   ├── objectDetection
│   │   └── json
│   │       ├── Makefile
│   │       └── main.cpp
│   ├── serialize_tcp
│   │   ├── Makefile
│   │   └── main.cpp
│   ├── serialize_winsock
│   │   └── main.cpp
│   ├── storage
│   │   ├── Makefile
│   │   ├── item
│   │   │   ├── Makefile
│   │   │   └── main.cpp
│   │   └── main.cpp
│   └── tutorial
│       ├── cctv
│       ├── cipher
│       │   ├── Makefile
│       │   └── chacha20_example.cpp
│       ├── log
│       │   ├── Human.cpp
│       │   ├── Human.h
│       │   ├── Makefile
│       │   └── main.cpp
│       ├── nlohmann-json
│       │   ├── Makefile
│       │   └── json_example.cpp
│       └── serialize
│           ├── Makefile
│           └── main.cpp
└── viewer
    ├── certs
    │   ├── server.cert
    │   └── server.key
    ├── json
    │   ├── JsonServer.cpp
    │   ├── JsonServer.h
    │   ├── Makefile
    │   └── main.cpp
    └── video
        ├── Makefile
        ├── VideoServer.cpp
        ├── VideoServer.h
        └── main.cpp
```