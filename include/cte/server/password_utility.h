//
// Author: Stefano Di Blasio
//

#pragma once

#include <cryptlib.h>
#include <secblock.h>

namespace cte{
    const CryptoPP::word64 N = 16384;
    const CryptoPP::word64 r = 8;
    const CryptoPP::word64 p = 1;

    typedef std::basic_string<char, std::char_traits<char>, CryptoPP::AllocatorWithCleanup<char>> secure_string;

    std::string generate_password(secure_string &&password, bool random_salt = false, CryptoPP::word64 interactions = N,
                                  CryptoPP::word64 block_size = r, CryptoPP::word64 parallelism = p);
    bool verify_password(secure_string &&password, const std::string& hash);
}