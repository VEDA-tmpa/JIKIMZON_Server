#include <openssl/evp.h>
#include <cstring>
#include <iostream>
#include <vector>

// Encrypt or decrypt using ChaCha20
void chacha20_encrypt_decrypt(const std::vector<uint8_t>& key,
                              const std::vector<uint8_t>& nonce,
                              const std::vector<uint8_t>& input,
                              std::vector<uint8_t>& output) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        throw std::runtime_error("Failed to create EVP_CIPHER_CTX");
    }

    // Initialize the context for ChaCha20 encryption
    if (EVP_EncryptInit_ex(ctx, EVP_chacha20(), nullptr, key.data(), nonce.data()) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("Failed to initialize ChaCha20 context");
    }

    int len = 0;
    output.resize(input.size());

    // Encrypt/Decrypt data
    if (EVP_EncryptUpdate(ctx, output.data(), &len, input.data(), input.size()) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("Failed to process ChaCha20 data");
    }

    int final_len = 0;
    if (EVP_EncryptFinal_ex(ctx, output.data() + len, &final_len) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("Failed to finalize ChaCha20 encryption");
    }

    // Free the context
    EVP_CIPHER_CTX_free(ctx);
}

int main() {
    try {
        // Example 256-bit key (32 bytes) and 96-bit nonce (12 bytes)
        std::vector<uint8_t> key(32, 0x01);    // Replace with secure random key
        std::vector<uint8_t> nonce(12, 0x02); // Replace with secure random nonce

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
        chacha20_encrypt_decrypt(key, nonce, plaintext, ciphertext);

        std::cout << "Ciphertext: ";
        for (uint8_t c : ciphertext) {
            std::cout << std::hex << static_cast<int>(c) << " ";
        }
        std::cout << std::dec << std::endl;

        // Decrypt the ciphertext
        chacha20_encrypt_decrypt(key, nonce, ciphertext, decrypted);

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
