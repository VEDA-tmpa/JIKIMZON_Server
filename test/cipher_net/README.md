# 평문 대상으로 테스트

- target
    - 65 (== ‘A’) , 100 byte

## C++ 에서 테스트

### step 1

- key : 0 으로 32 바이트
    - `std::vector<uint8_t> key(32, 0x00);`
- nonce : 0 으로 12 바이트
    - `std::vector<uint8_t> nonce(12, 0x00);`

### step 2 - 1

- key : key 생성 및 적용
- nonce : 0 으로 12 바이트
    - `std::vector<uint8_t> nonce(12, 0x00);`

### step 2 - 2

- key : 0 으로 32 바이트
    - `std::vector<uint8_t> key(32, 0x00);`
- nonce : timestamp

### step 3

- key : key 생성 및 적용
- nonce : timestamp