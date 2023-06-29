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

int main() {
    // not working: 1687899577 1687899613 1687899648 1687899700 1687899741 1687899757 1687899772 1687900131
    // 1.9375?? 1687899624 1687899678 0.9375 1687899782

    long seed = time(nullptr);
    cout << "with seed = " << seed << endl;
    FullyScheme fhe(8, seed);
    std::cout << "theta = " << fhe.theta << std::endl;
    std::cout << "n = " << fhe.n << std::endl;
    auto [secret_key, public_key] = fhe.key_gen();
    cout << "key_gen" << endl;
    auto c = fhe.encrypt(public_key.pk, NTL::GF2{1});
    cout << "c<0? = " << (c < 0) << endl;

    auto [c_star, z] = fhe.post_process(c, public_key.y);

    auto a = z;
    for (int i = 0; i < a.size(); i++) {
        for (int j = 0; j < a[i].size(); j++) {
            a[i][j] *= secret_key.s[i];
        }
    }

    auto w = fhe.generate_fewer_numbers(a);

    cout << a.size() << endl;
    cout << z[0].size() << endl;
    cout << w.size() << endl;

    mpf_class w_sum(0, fhe.n + 1);
    mpf_class a_sum(0, fhe.n + 1);
    mpf_class curr(0, fhe.n + 1);

    for (int i = 0; i < a.size(); i++) {
        curr = bits_to_mpf(a[i], fhe.n + 1);
        a_sum += curr;
        a_sum = mod2f(a_sum);
    }

    for (int i = 0; i < w.size(); i++) {
        curr = bits_to_mpf(w[i], fhe.n + 1);
        w_sum += curr;
        w_sum = mod2f(w_sum);
    }

    cout << "a_sum = " << a_sum << endl;
    cout << "w_sum = " << w_sum << endl;

    return 0;
}
