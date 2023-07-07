#include <iostream>
#include <iomanip>
#include <gmpxx.h>
#include "src/SomewhatScheme.h"
#include "src/FullyScheme.h"
#include "Ciphertext.h"
#include <NTL/GF2.h>
#include <chrono>
#include <thread>

using namespace std;

template<typename T>
T add_circuit(T m1, T m2) {
    return m1 + m2;
}

template<typename T>
T mul_circuit(T m1, T m2) {
    return m1 * m2;
}

NTL::GF2
get_squashed_decrypt(FullyScheme &fhe, const mpz_class &c, const SecretKey &secret_key, const PublicKey &public_key) {
    auto [c_star, z] = fhe.post_process(c, public_key.y);
    return fhe.squashed_decrypt(c_star, secret_key.s, z);
}


int main() {
    int counter = 0;
    for (int i = 0; i < 20; i++) {
        this_thread::sleep_for(std::chrono::seconds(2));
        long seed = time(nullptr);
        cout << seed << endl;
        FullyScheme fhe(6, 35, 35, seed);
        auto [secret_key, public_key] = fhe.key_gen();
        auto m = NTL::GF2{1};
        auto c = fhe.encrypt(public_key, m);
        cout << "c mod2 = " << (c % 2) << endl;
        if (c % 2 == 1 || c % 2 == -1) counter++;
        c = fhe.recrypt(c, public_key);
        cout << "recrypted mod2 = " << (c % 2) << endl;
        auto out = get_squashed_decrypt(fhe, c, secret_key, public_key);
        if(out!=m) {
            cout << "------------------- FALSE -------------------" << endl;
        }

//        SomewhatScheme scheme(8, seed);
////        cout << "gamma = " << scheme.gamma << endl;
////        cout << "eta = " << scheme.eta << endl;
////        cout << "ro = " << scheme.ro << endl;
////        cout << "tau = " << scheme.tau << endl;
//        auto [p, public_key] = scheme.key_gen();
//        mpf_class pf(p);
//
//        auto m0 = NTL::GF2{1};
//        auto m1 = NTL::GF2{1};
//
//        auto c0 = scheme.encrypt(public_key, m0);
//        auto c1 = scheme.encrypt(public_key, m1);
//        auto noise_before = rem(c1, p);
//        mpf_class noise_before_f(noise_before);
//
//        auto c2 = c1 * c0 * c1;
//        auto noise = rem(c2, p);
//        mpf_class c2f(c2);
//        mpf_class tf(c2.threshold);
//        mpf_class noisef(noise);
//
//        std::cout << "p = " << pf << std::endl;
//        std::cout << "threshold = " << tf << std::endl;
//        std::cout << "c2 = " << c2f << std::endl;
//        std::cout << "noise = " << noisef << std::endl;
//        std::cout << "noise before = " << noise_before_f << std::endl;
//        auto d_c2 = scheme.decrypt(p, c2);
//        if (d_c2 != m0 * m1 * m1) {
//            cout << "------------------- FALSE -------------------" << endl;
//            counter++;
//        }
//        if (abs(c2) > c2.threshold) {
//            cout << "------------------- THRESHHOLD -------------------" << endl;
//        }
    }
    cout << counter << endl;
//    cout << c1.mod_red->size() << endl;
//    cout << c2.mod_red->size() << endl;
//    cout << (c2.mod_red == c1.mod_red) << endl;
//    cout << (public_key.mod_red == c1.mod_red) << endl;
//    cout << scheme.gamma << endl;
//    mpz_class x = 1;
//
//    cout << "c3" << endl;
//    auto c3 = c2;
//    cout << (c3.mod_red == c2.mod_red) << endl;
//    cout << (c3 == c2) << endl;

//    auto bg = std::chrono::steady_clock::now();
//    mpz_class recrypted = fhe.squashed_decrypt(c_star_mpz, public_key.e_sk, z_mpz);
//    recrypted = recrypted * recrypted;

//    auto ed = std::chrono::steady_clock::now();
//    cout << "elapsed time: " << chrono::duration_cast<chrono::seconds>(ed - bg).count() << endl;

//}

    return 0;
}
