#include <iostream>
#include <iomanip>
#include <gmpxx.h>
#include "src/SomewhatScheme.h"
#include "src/FullyScheme.h"
#include <NTL/GF2.h>

using namespace std;

template<typename T>
T add_circuit(T m1, T m2) {
    return m1 + m2;
}

//mpz_class recrypt(const mpz_class &c, const PublicKey &public_key) {
//    auto [c_star_bits, z_bits] = post_process(c, public_key.y);
//    std::vector<mpz_class> c_star_mpz = c_star_to_mpz(c_star_bits);
//    std::vector<std::vector<mpz_class>> z_mpz = z_to_mpz(z_bits);
//
//    return greased_decrypt(c_star_mpz, public_key.e_sk, z_mpz);
//}


int main() {
    // not working: 1687899577 1687899613 1687899648 1687899700 1687899741 1687899757 1687899772 1687900131
    // 1.9375?? 1687899624 1687899678 0.9375 1687899782

    long seed = time(nullptr);
    FullyScheme fhe(16, seed);
    auto [secret_key, public_key] = fhe.key_gen();
//    for (int i = 0; i < 5; i++) {
    auto c0 = fhe.encrypt(public_key.pk, NTL::GF2{0});
    auto c1 = fhe.encrypt(public_key.pk, NTL::GF2{1});
    auto c2 = c1;

    auto [c_star, z] = fhe.post_process(c2, public_key.y);

    auto g_out = fhe.greased_decrypt(c_star, secret_key.s, z);
    auto d_out = fhe.decrypt(secret_key.p, c2);
    if (g_out != d_out) cout << "FALSE" << endl;
//        else cout << "FALSE" << endl;
    cout << "greased decrypted = " << g_out << endl;
    cout << "normal decrypted = " << d_out << endl;
//    }

    std::vector<mpz_class> c_star_mpz = c_star_to_mpz(c_star);
    std::vector<std::vector<mpz_class>> z_mpz = z_to_mpz(z);
    mpz_class recrypted = fhe.greased_decrypt(c_star_mpz, public_key.e_sk, z_mpz);
    auto r_out = fhe.decrypt(secret_key.p, recrypted);
    cout << "recrypted decrypted = " << r_out << endl;

    return 0;
}
