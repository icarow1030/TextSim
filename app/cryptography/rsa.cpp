#include "rsa.hpp"

RSA::Keys RSA::generate_keys() {
    gmp_randstate_t randstate;
    gmp_randinit_mt(randstate);
    gmp_randseed_ui(randstate, (unsigned long)time(NULL) ^ (unsigned long)clock());
    mpz_class p, q, n, e, d, phi_n, p1, q1, inv;
    generate_prime(p, 10, randstate);
    do {
        generate_prime(q, 10, randstate);
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
        gmp_randclear(randstate);
        throw std::runtime_error("Não foi possível calcular o inverso modular");
    }
    Keys keys;
    keys.public_key.n = n;
    keys.public_key.e = e;
    keys.private_key.n = n;
    keys.private_key.d = d;
    gmp_randclear(randstate);
    return keys;
}

mpz_class RSA::encrypt(const std::string& message, const PublicKey& pubkey) {
    mpz_class m;
    string_to_mpz(m, message);
    mpz_class encrypted;
    mod_exp(encrypted, m, pubkey.e, pubkey.n);
    return encrypted;
}

std::string RSA::decrypt(const mpz_class& encrypted, const PrivateKey& privkey) {
    mpz_class m;
    mod_exp(m, encrypted, privkey.d, privkey.n);
    
    // Calcula o tamanho necessário para o buffer
    size_t count = (mpz_sizeinbase(m.get_mpz_t(), 2) + 7) / 8;
    std::vector<unsigned char> buffer(count);
    
    // Exporta os bytes
    mpz_export(buffer.data(), &count, 1, 1, 0, 0, m.get_mpz_t());
    
    // Converte para string
    return std::string(buffer.begin(), buffer.begin() + count);
}

void RSA::random_odd_512(mpz_class& result, gmp_randstate_t randstate) {
    mpz_urandomb(result.get_mpz_t(), randstate, 512);
    mpz_setbit(result.get_mpz_t(), 511);
    mpz_setbit(result.get_mpz_t(), 0);
}

void RSA::random_base(mpz_class& base, const mpz_class& num, gmp_randstate_t randstate) {
    mpz_class tmp = num - 3;
    mpz_urandomm(base.get_mpz_t(), randstate, tmp.get_mpz_t());
    base += 2;
}

void RSA::mod_exp(mpz_class& res, const mpz_class& base, const mpz_class& exp, const mpz_class& mod) {
    mpz_powm(res.get_mpz_t(), base.get_mpz_t(), exp.get_mpz_t(), mod.get_mpz_t());
}

bool RSA::miller_rabin(const mpz_class& num, int k, gmp_randstate_t randstate) {
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
        random_base(a, num, randstate);
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

void RSA::generate_prime(mpz_class& prime, int k, gmp_randstate_t randstate) {
    do {
        random_odd_512(prime, randstate);
    } while (!miller_rabin(prime, k, randstate));
}

bool RSA::mod_inverse(mpz_class& inv, const mpz_class& a, const mpz_class& m) {
    return mpz_invert(inv.get_mpz_t(), a.get_mpz_t(), m.get_mpz_t()) != 0;
}

void RSA::string_to_mpz(mpz_class& result, const std::string& message) {
    mpz_import(result.get_mpz_t(), message.size(), 1, 1, 0, 0, message.data());
}

std::string RSA::mpz_to_string(const mpz_class& num) {
    size_t count = 0;
    void* str = mpz_export(nullptr, &count, 1, 1, 0, 0, num.get_mpz_t());
    std::string result((char*)str, count);
    free(str);
    return result;
}