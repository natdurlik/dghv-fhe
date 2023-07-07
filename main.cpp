#include <iostream>
#include "src/FullyScheme.h"
#include "Ciphertext.h"
#include <NTL/GF2.h>
#include <chrono>

using namespace std;

template<typename T>
T circuit(T m1, T m2, T m3) {
    return m1 + m2 * m3;
}


NTL::GF2
get_squashed_decrypt(FullyScheme &fhe, const mpz_class &c, const SecretKey &secret_key, const PublicKey &public_key) {
    auto [c_star, z] = fhe.post_process(c, public_key.y);
    return fhe.squashed_decrypt(c_star, secret_key.s, z);
}

int main() {
    long seed = time(nullptr);
    cout << "seed = " << seed << endl;
    FullyScheme fhe(6, 35, 35, seed);
    auto [secret_key, public_key] = fhe.key_gen();

    auto m0 = NTL::GF2{1};
    auto m1 = NTL::GF2{1};
    auto m2 = NTL::GF2{0};
    auto c0 = fhe.encrypt(public_key, m0);
    auto c1 = fhe.encrypt(public_key, m1);
    auto c2 = fhe.encrypt(public_key, m2);

    for (int i = 0; i < 10; i++) {
        c0 = circuit(c0, c1, c2);
        m0 = circuit(m0, m1, m2);
        fhe.recrypt(c0, public_key);
    }

    auto c_out = get_squashed_decrypt(fhe, c0, secret_key, public_key);
    cout << (c_out == m0) << endl;

    return 0;
}
