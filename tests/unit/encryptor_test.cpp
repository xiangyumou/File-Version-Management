/**
 * @file encryptor_test.cpp
 * @brief Unit tests for the Encryptor class
 */

#include <gtest/gtest.h>
#include "encryptor.h"
#include <vector>
#include <string>

// Test fixture for Encryptor tests
class EncryptorTest : public ::testing::Test, protected Encryptor {
protected:
    void SetUp() override {
        // Setup code if needed
    }

    void TearDown() override {
        // Cleanup code if needed
    }
};

// Test that encryption followed by decryption returns the original sequence
// Note: encrypt_sequence modifies the input vector (adds padding), so we compare
// only the original elements after decryption
TEST_F(EncryptorTest, EncryptDecryptRoundTrip) {
    std::vector<int> original = {72, 101, 108, 108, 111};  // "Hello" in ASCII
    const size_t original_size = original.size();
    std::vector<std::pair<double, double>> encrypted;
    std::vector<int> decrypted;

    ASSERT_TRUE(encrypt_sequence(original, encrypted));
    ASSERT_FALSE(encrypted.empty());

    ASSERT_TRUE(decrypt_sequence(encrypted, decrypted));
    
    // The decrypted size should match the original size (before padding was added)
    ASSERT_EQ(original_size, decrypted.size());

    for (size_t i = 0; i < original_size; ++i) {
        // Note: original vector has been modified by encrypt_sequence (padding added)
        // We compare against the expected values directly
        std::vector<int> expected = {72, 101, 108, 108, 111};
        EXPECT_EQ(expected[i], decrypted[i]) << "Mismatch at index " << i;
    }
}

// Test with empty sequence
TEST_F(EncryptorTest, EncryptEmptySequence) {
    std::vector<int> original;
    std::vector<std::pair<double, double>> encrypted;
    std::vector<int> decrypted;

    ASSERT_TRUE(encrypt_sequence(original, encrypted));
    ASSERT_TRUE(decrypt_sequence(encrypted, decrypted));
    EXPECT_TRUE(decrypted.empty());
}

// Test with a longer sequence
TEST_F(EncryptorTest, EncryptLongSequence) {
    std::vector<int> original;
    const size_t test_size = 2000;
    // Create a sequence longer than FFT block size
    for (size_t i = 0; i < test_size; ++i) {
        original.push_back(static_cast<int>(i % 256));
    }
    
    // Make a copy for comparison (encrypt_sequence will modify original)
    std::vector<int> original_copy = original;
    
    std::vector<std::pair<double, double>> encrypted;
    std::vector<int> decrypted;

    ASSERT_TRUE(encrypt_sequence(original, encrypted));
    ASSERT_TRUE(decrypt_sequence(encrypted, decrypted));
    
    ASSERT_EQ(test_size, decrypted.size());
    for (size_t i = 0; i < test_size; ++i) {
        EXPECT_EQ(original_copy[i], decrypted[i]) << "Mismatch at index " << i;
    }
}

// Test with string content
TEST_F(EncryptorTest, EncryptStringContent) {
    std::string test_string = "File Version Management System - Test Content!";
    std::vector<int> original(test_string.begin(), test_string.end());
    const size_t original_size = original.size();
    
    // Make a copy for verification
    std::vector<int> original_copy = original;
    
    std::vector<std::pair<double, double>> encrypted;
    std::vector<int> decrypted;

    ASSERT_TRUE(encrypt_sequence(original, encrypted));
    ASSERT_TRUE(decrypt_sequence(encrypted, decrypted));
    
    ASSERT_EQ(original_size, decrypted.size());
    
    std::string result_string(decrypted.begin(), decrypted.end());
    EXPECT_EQ(test_string, result_string);
}

// Test that encrypted data is different from original
TEST_F(EncryptorTest, EncryptedDataIsDifferent) {
    std::vector<int> original = {1, 2, 3, 4, 5, 6, 7, 8};
    std::vector<int> original_copy = original;  // Keep copy before modification
    std::vector<std::pair<double, double>> encrypted;

    ASSERT_TRUE(encrypt_sequence(original, encrypted));
    
    // Encrypted data should not be equal to original values
    bool all_same = true;
    for (size_t i = 0; i < std::min(original_copy.size(), encrypted.size()); ++i) {
        if (static_cast<int>(encrypted[i].first) != original_copy[i]) {
            all_same = false;
            break;
        }
    }
    EXPECT_FALSE(all_same) << "Encrypted data appears identical to original";
}

// Test block boundary handling
TEST_F(EncryptorTest, BlockBoundaryHandling) {
    // Test with exactly block size - 1 elements
    const size_t test_size = N - 1;  // N is FFT block size
    std::vector<int> original(test_size, 42);
    std::vector<int> original_copy = original;
    
    std::vector<std::pair<double, double>> encrypted;
    std::vector<int> decrypted;

    ASSERT_TRUE(encrypt_sequence(original, encrypted));
    ASSERT_TRUE(decrypt_sequence(encrypted, decrypted));
    
    ASSERT_EQ(test_size, decrypted.size());
    for (size_t i = 0; i < test_size; ++i) {
        EXPECT_EQ(original_copy[i], decrypted[i]);
    }
}
