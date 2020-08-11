//
// Author: Stefano Di Blasio
//

#pragma once

#include <cryptlib.h>

static class PasswordManager {
    static const CryptoPP::word64 N = 16384;
    static const CryptoPP::word64 r = 8;
    static const CryptoPP::word64 p = 1;
    static const size_t digest_size = 64;

public:
    static std::string generate_password(const std::string& password, bool random_salt = false, CryptoPP::word64 interactions = PasswordManager::N,
            CryptoPP::word64 block_size = PasswordManager::r, CryptoPP::word64 parallelism = PasswordManager::p);
    static bool verify_password(const std::string& password, const std::string& hash);
};