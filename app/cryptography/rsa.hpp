#pragma once
#include <iostream>
#include <gmpxx.h>
#include <string>
#include <gmp.h>
#include <time.h>
#include <stdlib.h>
#include <vector>


class RSA {
public:
    struct PublicKey {
        mpz_class n;
        mpz_class e;
    };
    struct PrivateKey {
        mpz_class n;
        mpz_class d;
    };
    struct Keys {
        PublicKey public_key;
        PrivateKey private_key;
    };

    static Keys generate_keys();
    static mpz_class encrypt(const std::string& message, const PublicKey& pubkey);
    static std::string decrypt(const mpz_class& encrypted, const PrivateKey& privkey);

private:
    static constexpr const char* PUBLIC_EXPONENT = "65537";
    static void random_odd_512(mpz_class& result, gmp_randstate_t randstate);
    static void random_base(mpz_class& base, const mpz_class& num, gmp_randstate_t randstate);
    static void mod_exp(mpz_class& res, const mpz_class& base, const mpz_class& exp, const mpz_class& mod);
    static bool miller_rabin(const mpz_class& num, int k, gmp_randstate_t randstate);
    static void generate_prime(mpz_class& prime, int k, gmp_randstate_t randstate);
    static bool mod_inverse(mpz_class& inv, const mpz_class& a, const mpz_class& m);
    static void string_to_mpz(mpz_class& result, const std::string& message);
    static std::string mpz_to_string(const mpz_class& num);
};

