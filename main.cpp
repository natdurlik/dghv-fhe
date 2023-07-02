#include <iostream>
#include <iomanip>
#include <gmpxx.h>
#include "src/SomewhatScheme.h"
#include "src/FullyScheme.h"
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
    for (int i = 0; i < 10; i++) {

        std::this_thread::sleep_for(std::chrono::seconds(2));
        long seed = time(nullptr);
//        long seed = 1688292087;
//        long seed = 1688293068;
//    long seed = 1688293532;
        FullyScheme fhe(8, seed);
        auto [secret_key, public_key] = fhe.key_gen();
        cout << endl;
        cout << seed << endl;
        cout << "theta = " << fhe.theta << endl;
        cout << "kappa = " << fhe.kappa << endl;
//    cout << "kappa ? = " << fhe.gamma * fhe.eta / fhe.ro_prim << endl;

        auto m0 = NTL::GF2{1};
        auto m1 = NTL::GF2{1};
        auto c0 = fhe.encrypt(public_key.pk, m0);
        auto c1 = fhe.encrypt(public_key.pk, m1);
        auto c2 = mul_circuit(c0, c1);
        auto m_out = mul_circuit(m0, m1);

        auto s_out = get_squashed_decrypt(fhe, c2, secret_key, public_key);
        auto d_out = fhe.decrypt(secret_key.p, c2);
        mpz_class recrypted = fhe.recrypt(c2, public_key);
        auto r_out = get_squashed_decrypt(fhe, recrypted, secret_key, public_key);

        mpf_class c2_f(c2);
        mpf_class recrypted_f(recrypted);
        cout << "recrypted = " << recrypted_f << endl;
        cout << "c2_f = " << c2_f << endl;


        cout << "squashed decrypted = " << s_out << endl;
        cout << "normal decrypted = " << d_out << endl;
        cout << "message output = " << m_out << endl;
        cout << "recrypted decrypted = " << r_out << endl;
        if (s_out != m_out || r_out != m_out) {
            cout << "FALSE----------------------" << endl;
            counter++;
        }

        cout << endl << "multiplyed" << endl;
        auto c3 = mul_circuit(recrypted, recrypted);
        auto m3 = mul_circuit(m_out, m_out);
        auto c3_r_out = get_squashed_decrypt(fhe, c3, secret_key, public_key);
        auto c3_d_out = fhe.decrypt(secret_key.p, c3);
        auto c3_recrypted = fhe.recrypt(c3, public_key);
        auto c3_r_d = fhe.decrypt(secret_key.p, c3_recrypted);
        auto c3_r_s = get_squashed_decrypt(fhe, c3_recrypted, secret_key, public_key);
        mpf_class c3_f(c3);
        mpf_class c3_recrypted_f(c3_recrypted);
        cout << "c3_f = " << c3_f << endl;
        cout << "c3_recrypted_f = " << c3_recrypted_f << endl;
        cout << "mul_circuit squashed decrypted = " << c3_r_out << endl;
        cout << "mul_circuit normal decrypted = " << c3_d_out << endl;
        cout << "mul_circuit recrypted decrypted = " << c3_r_d << endl;
        cout << "mul_circuit recrypted squashed decrypted = " << c3_r_s << endl;
        cout << "mul_circuit message = " << m3 << endl;
        if (c3_r_d != m3 || c3_r_s != m3) {
            cout << "WRONG-------------------------------------------" << endl;
            counter++;
        }
    }
    cout << "false count=" << counter << endl;


//    auto bg = std::chrono::steady_clock::now();
//    mpz_class recrypted = fhe.squashed_decrypt(c_star_mpz, public_key.e_sk, z_mpz);
//    recrypted = recrypted * recrypted;

//    auto ed = std::chrono::steady_clock::now();
//    cout << "elapsed time: " << chrono::duration_cast<chrono::seconds>(ed - bg).count() << endl;

//}

    return 0;
}
