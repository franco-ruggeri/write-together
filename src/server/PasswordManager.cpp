//
// Author: Stefano Di Blasio
//

#include <cte/server/PasswordManager.h>
#include <secblock.h>
#include <scrypt.h>
#include <osrng.h>
#include <hex.h>
#include <base64.h>

namespace cte {
    static CryptoPP::byte default_salt[] = "KClO3";

    std::string generate_password(std::string &&password, bool random_salt, CryptoPP::word64 interactions,
                                       CryptoPP::word64 block_size, CryptoPP::word64 parallelism) {
        CryptoPP::SecByteBlock derived(digest_size);
        CryptoPP::SecByteBlock pass(reinterpret_cast<const CryptoPP::byte *>(&password[0]), password.size());
        CryptoPP::SecByteBlock salt(default_salt, 5);
        if (random_salt) {
            CryptoPP::AutoSeededRandomPool rng;
            salt = CryptoPP::SecByteBlock(salt_size);
            rng.GenerateBlock(salt, salt_size);
        }
        CryptoPP::Scrypt scrypt;
        scrypt.DeriveKey(derived, digest_size, pass, pass.size(), salt, salt.size(), interactions,
                         block_size, parallelism);
        std::string pure_hash;
        std::string enc_salt;
        std::string digest;
        CryptoPP::StringSource(salt, salt.size(), true,
                               new CryptoPP::Base64Encoder(new CryptoPP::StringSink(enc_salt), false));
        CryptoPP::StringSource(derived, digest_size, true,
                               new CryptoPP::HexEncoder(new CryptoPP::StringSink(pure_hash)));
        digest.append(std::to_string(interactions)).append("$").append(std::to_string(block_size)).append("$").append(
                        std::to_string(parallelism)).append("$")
                .append(enc_salt).append("$").append(pure_hash);
        return digest;
    }

    bool verify_password(std::string &&password, const std::string &hash) {
        CryptoPP::SecByteBlock derived(digest_size);
        CryptoPP::SecByteBlock pass(reinterpret_cast<const CryptoPP::byte *>(&password[0]), password.size());
        CryptoPP::SecByteBlock salt;

        // parse the hash in its components
        std::size_t current, previous = 0;
        current = hash.find('$');
        CryptoPP::word64 interactions = std::stoull(hash.substr(previous, current - previous));
        previous = current + 1;
        current = hash.find('$', previous);
        CryptoPP::word64 block_size = std::stoull(hash.substr(previous, current - previous));
        previous = current + 1;
        current = hash.find('$', previous);
        CryptoPP::word64 parallelism = std::stoull(hash.substr(previous, current - previous));
        previous = current + 1;
        current = hash.find('$', previous);
        std::string enc_salt = hash.substr(previous, current - previous);
        previous = current + 1;
        CryptoPP::SecByteBlock db_digest(reinterpret_cast<const CryptoPP::byte *>(&hash[previous]),
                                         hash.size() - previous);
        // end parsing; decode the salt, to be used to calculate the derived key
        std::string dec_salt;
        CryptoPP::Base64Decoder decoder;
        decoder.Put(reinterpret_cast<CryptoPP::byte *>(enc_salt.data()), enc_salt.size());
        decoder.MessageEnd();
        CryptoPP::word64 size = decoder.MaxRetrievable();
        if (size && size < SIZE_MAX) {
            dec_salt.resize(size);
            decoder.Get((CryptoPP::byte *) &dec_salt[0], dec_salt.size());
        }
        // original salt value in dec_salt
        // calculate the derived key from the retrieved parameters and match it against the derived key in db
        CryptoPP::Scrypt scrypt;
        scrypt.DeriveKey(derived, digest_size, pass, pass.size(), (CryptoPP::byte *) &dec_salt[0],
                         dec_salt.size(), interactions, block_size, parallelism);
        // alternative check, to be preferred
        CryptoPP::HexDecoder digest_decoder;
        digest_decoder.Put(db_digest.BytePtr(), db_digest.size());
        digest_decoder.MessageEnd();
        size = digest_decoder.MaxRetrievable();
        if (size && size < SIZE_MAX) {
            db_digest.resize(size);
            digest_decoder.Get(db_digest, db_digest.size());
        }
        return derived == db_digest;
    }
}