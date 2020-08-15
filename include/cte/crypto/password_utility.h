//
// Author: Stefano Di Blasio
//

#pragma once

#include <cryptlib.h>

namespace cte{
    const CryptoPP::word64 N = 16384;
    const CryptoPP::word64 r = 8;
    const CryptoPP::word64 p = 1;

    std::string generate_password(std::string&& password, bool random_salt = true, CryptoPP::word64 interactions = N,
            CryptoPP::word64 block_size = r, CryptoPP::word64 parallelism = p);

    bool verify_password(std::string&& password, const std::string& hash);
}
