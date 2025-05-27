#include "rsa.hpp"
#include <gmp.h>
#include <time.h>
#include <stdlib.h>

RSA::RSA() {
    gmp_randinit_mt(randstate);
    gmp_randseed_ui(randstate, (unsigned long)time(NULL) ^ (unsigned long)clock());
}

RSA::~RSA() {
    gmp_randclear(randstate);
}

void RSA::generate_keys() {
    mpz_class p, q, n, e, d, phi_n, p1, q1, inv;
    generate_prime(p, 10);
    do {
        generate_prime(q, 10);
    } while (p == q);
    n = p * q;
    p1 = p - 1;
    q1 = q - 1;
    phi_n = p1 * q1;
    e = mpz_class(PUBLIC_EXPONENT);
    if (mod_inverse(inv, e, phi_n)) {
        d = inv;
        if (d >= phi_n) d %= phi_n;
    } else {
        throw std::runtime_error("Não foi possível calcular o inverso modular");
    }
    public_key.n = n;
    public_key.e = e;
    private_key.n = n;
    private_key.d = d;
}

mpz_class RSA::encrypt(const std::string& message) const {
    mpz_class m;
    string_to_mpz(m, message);
    mpz_class encrypted;
    mod_exp(encrypted, m, public_key.e, public_key.n);
    return encrypted;
}

std::string RSA::decrypt(const mpz_class& encrypted) const {
    mpz_class m;
    mod_exp(m, encrypted, private_key.d, private_key.n);
    return mpz_to_string(m);
}

const RSA::PublicKey& RSA::get_public_key() const {
    return public_key;
}

const RSA::PrivateKey& RSA::get_private_key() const {
    return private_key;
}

void RSA::random_odd_512(mpz_class& result) {
    mpz_urandomb(result.get_mpz_t(), randstate, 512);
    mpz_setbit(result.get_mpz_t(), 511);
    mpz_setbit(result.get_mpz_t(), 0);
}

void RSA::random_base(mpz_class& base, const mpz_class& num) {
    mpz_class tmp = num - 3;
    mpz_urandomm(base.get_mpz_t(), randstate, tmp.get_mpz_t());
    base += 2;
}

void RSA::mod_exp(mpz_class& res, const mpz_class& base, const mpz_class& exp, const mpz_class& mod) const {
    mpz_powm(res.get_mpz_t(), base.get_mpz_t(), exp.get_mpz_t(), mod.get_mpz_t());
}

bool RSA::miller_rabin(const mpz_class& num, int k) {
    if (num < 2) return false;
    if (num == 2) return true;
    if (mpz_even_p(num.get_mpz_t())) return false;
    mpz_class d = num - 1;
    unsigned long s = 0;
    while (mpz_even_p(d.get_mpz_t())) {
        d >>= 1;
        s++;
    }
    for (int i = 0; i < k; i++) {
        mpz_class a;
        random_base(a, num);
        mpz_class x;
        mod_exp(x, a, d, num);
        if (x == 1 || x == num - 1) continue;
        bool composite = true;
        for (unsigned long r = 1; r < s; r++) {
            mod_exp(x, x, 2, num);
            if (x == num - 1) {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}

void RSA::generate_prime(mpz_class& prime, int k) {
    do {
        random_odd_512(prime);
    } while (!miller_rabin(prime, k));
}

bool RSA::mod_inverse(mpz_class& inv, const mpz_class& a, const mpz_class& m) {
    return mpz_invert(inv.get_mpz_t(), a.get_mpz_t(), m.get_mpz_t()) != 0;
}

void RSA::string_to_mpz(mpz_class& result, const std::string& message) const {
    mpz_import(result.get_mpz_t(), message.size(), 1, 1, 0, 0, message.data());
}

std::string RSA::mpz_to_string(const mpz_class& num) const {
    size_t count = 0;
    void* str = mpz_export(nullptr, &count, 1, 1, 0, 0, num.get_mpz_t());
    std::string result((char*)str, count);
    free(str);
    return result;
}