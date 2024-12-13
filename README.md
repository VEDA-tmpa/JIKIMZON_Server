# server

## build && run
- 

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
├── README.md
├── cctv
│   ├── 192.168.0.18
│   │   └── keyfile.bin
│   ├── json
│   │   ├── JsonClient.cpp
│   │   ├── JsonClient.h
│   │   ├── Makefile
│   │   ├── main.cpp
│   │   └── main.out
│   └── video
│       ├── Makefile
│       ├── VideoClient.cpp
│       ├── VideoClient.h
│       ├── main.cpp
│       └── main.out
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
│   ├── 192.168.50.14
│   ├── 192.168.50.14.h264
│   ├── cipher
│   │   ├── Makefile
│   │   ├── key_generation.log
│   │   └── keyfile.bin
│   ├── log.txt
│   ├── manager
│   │   ├── StorageManager.h
│   │   └── database
│   │       ├── BaseItem.h
│   │       ├── H264Item.h
│   │       ├── ItemFactory.h
│   │       ├── JsonItem.h
│   │       └── StorageFile.h
│   └── temp.txt
├── test
│   ├── cipher_net
│   │   ├── step1
│   │   │   └── client.out
│   │   ├── step2-1
│   │   │   ├── client.out
│   │   │   └── server.out
│   │   └── step2-2
│   │       ├── client.out
│   │       └── server.out
│   ├── fixture
│   │   ├── Fixture.cpp
│   │   └── Fixture.h
│   ├── objectDetection
│   │   └── json
│   │       ├── Makefile
│   │       ├── main.cpp
│   │       └── main.out
│   ├── serialize_tcp
│   │   ├── Makefile
│   │   ├── a.out
│   │   └── main.cpp
│   ├── serialize_winsock
│   │   └── main.cpp
│   ├── storage
│   │   ├── Makefile
│   │   ├── a.out
│   │   ├── item
│   │   │   ├── Makefile
│   │   │   ├── a.out
│   │   │   └── main.cpp
│   │   └── main.cpp
│   └── tutorial
│       ├── cctv
│       │   └── main.out
│       ├── cipher
│       │   ├── Makefile
│       │   ├── a.out
│       │   └── chacha20_example.cpp
│       ├── log
│       │   ├── Human.cpp
│       │   ├── Human.h
│       │   ├── Makefile
│       │   └── main.cpp
│       ├── nlohmann-json
│       │   ├── Makefile
│       │   ├── json_example.cpp
│       │   └── json_example.out
│       └── serialize
│           ├── Makefile
│           ├── a.out
│           └── main.cpp
└── viewer
    ├── certs
    │   ├── server.cert
    │   └── server.key
    ├── json
    │   ├── JsonServer.cpp
    │   ├── JsonServer.h
    │   ├── Makefile
    │   ├── main.cpp
    │   └── main.out
    ├── keyfile.bin
    └── video
        ├── Makefile
        ├── VideoServer.cpp
        ├── VideoServer.h
        ├── main.cpp
        └── main.out
```