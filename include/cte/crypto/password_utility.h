/*
 * Utilities for storing hashes instead of plain-text passwords.
 */

#pragma once

#include <cryptlib.h>
#include <secblock.h>
#include <QtCore/QString>

namespace cte{
    const CryptoPP::word64 N = 16384;
    const CryptoPP::word64 r = 8;
    const CryptoPP::word64 p = 1;

    std::string generate_password(QString&& password, bool random_salt=true, CryptoPP::word64 interactions=N,
                                  CryptoPP::word64 block_size=r, CryptoPP::word64 parallelism=p);

    bool verify_password(QString&& password, const QString& hash);
}
