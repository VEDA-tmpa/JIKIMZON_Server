#include <openssl/evp.h>
#include <cstring>
#include <iostream>
#include <vector>
#include "common/cipher/Cipher.h"

int main() {
    try {
        // Example 256-bit key (32 bytes) and 96-bit nonce (12 bytes)
        std::vector<uint8_t> key(32, 0x01);    // Replace with secure random key
        std::vector<uint8_t> nonce(12, 0x02); // Replace with secure random nonce

		cipher::Cipher cipherHandler(key);

        // Example plaintext
        std::vector<uint8_t> plaintext = { 'H', 'e', 'l', 'l', 'o', ' ', 'W', 'o', 'r', 'l', 'd', '!' };
        std::cout << "plaintext: ";
        for (uint8_t c : plaintext) {
            std::cout << static_cast<char>(c);
        }
        std::cout << std::endl;
		
        std::vector<uint8_t> ciphertext;
        std::vector<uint8_t> decrypted;

        // Encrypt the plaintext
        // chacha20_encrypt_decrypt(key, nonce, plaintext, ciphertext);
		cipherHandler.Chacha20_encrypt_decrypt(nonce, plaintext, ciphertext);

        std::cout << "Ciphertext: ";
        for (uint8_t c : ciphertext) {
            std::cout << std::hex << static_cast<int>(c) << " ";
        }
        std::cout << std::dec << std::endl;

        // Decrypt the ciphertext
        // chacha20_encrypt_decrypt(key, nonce, ciphertext, decrypted);
		cipherHandler.Chacha20_encrypt_decrypt(nonce, ciphertext, decrypted);

        std::cout << "Decrypted: ";
        for (uint8_t c : decrypted) {
            std::cout << static_cast<char>(c);
        }
        std::cout << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
