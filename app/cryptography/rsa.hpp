#pragma once
#include <iostream>
#include <gmpxx.h>
#include <string>

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

    RSA();
    ~RSA();

    void generate_keys();
    mpz_class encrypt(const std::string& message) const;
    std::string decrypt(const mpz_class& encrypted) const;
    const PublicKey& get_public_key() const;
    const PrivateKey& get_private_key() const;

private:
    gmp_randstate_t randstate;
    PublicKey public_key;
    PrivateKey private_key;
    static constexpr const char* PUBLIC_EXPONENT = "65537";

    void random_odd_512(mpz_class& result);
    void random_base(mpz_class& base, const mpz_class& num);
    void mod_exp(mpz_class& res, const mpz_class& base, const mpz_class& exp, const mpz_class& mod) const;
    bool miller_rabin(const mpz_class& num, int k);
    void generate_prime(mpz_class& prime, int k);
    bool mod_inverse(mpz_class& inv, const mpz_class& a, const mpz_class& m);
    void string_to_mpz(mpz_class& result, const std::string& message) const;
    std::string mpz_to_string(const mpz_class& num) const;
};

