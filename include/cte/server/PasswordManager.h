//
// Author: Stefano Di Blasio
//

#pragma once

#include <cryptlib.h>

namespace cte{
    static const CryptoPP::word64 N = 16384;
    static const CryptoPP::word64 r = 8;
    static const CryptoPP::word64 p = 1;
    static const size_t digest_size = 64;
    static const size_t salt_size = 16;

    extern std::string generate_password(std::string&& password, bool random_salt = false, CryptoPP::word64 interactions = N,
            CryptoPP::word64 block_size = r, CryptoPP::word64 parallelism = p);
    extern bool verify_password(std::string&& password, const std::string& hash);
}