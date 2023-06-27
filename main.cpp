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

vector<NTL::GF2> to_bits(mpf_class f, unsigned n) {
    vector<NTL::GF2> bits(n + 1, NTL::GF2{0}); // n+1 because n bits of precision AFTER binary point
    mp_exp_t exp;
    string bits_str = f.get_str(exp, 2, n);
    if (-exp + 1 < 0) {
        cout << "unexpected exp" << endl;
        return bits;
    }
    unsigned start = -exp + 1;

//    unsigned str_idx = 0u;
    for (auto i = start, str_idx = 0u; i < bits.size() && str_idx < bits_str.size(); i++, str_idx++) {
        if (bits_str[str_idx] == '1') {
            bits[i] = NTL::GF2{1};
        }
    }
    return bits;
}

int main() {

//    FullyScheme fhe(6, 0);
//    SecretKey secret_key;
//    PublicKey public_key;
//    std::tie(secret_key, public_key) = fhe.key_gen();
//    auto [sk, pk] = fhe.key_gen();
//
//    cout << "kappa = " << fhe.kappa << endl;
//    cout << "gamma = " << fhe.gamma / fhe.ro << endl;
//    cout << "n = " << fhe.n << endl;
//
//    mpf_class f(0.5, 100);
//    mp_exp_t exp = 1;
//    cout << f.get_str(exp, 2, fhe.n) << endl;
//    cout << "exp = " << exp << endl;
//    auto bits = mpf_to_bits(f, fhe.n);
//
//    cout << "GF2 bits" << endl;
//    for (auto x: bits) {
//        cout << x;
//    }
//    cout << endl;
    //

//    mpf_class f("0.001", 100, 2);
//    cout << f << endl;
//
//    vector<NTL::GF2> bits{NTL::GF2{0}, NTL::GF2{0}, NTL::GF2{0}, NTL::GF2{1}};
//    mpf_class f2 = bits_to_mpf(bits, 100);
//    cout << f2 << endl;
//    mpz_class x = 8;
//    auto stri = x.get_str(2);
//    cout << stri << endl;
//    mpf_class()

//    mpf_class x("1.99999999999", 100, 10);
//    cout << x << endl;
//    mp_exp_t exp;
//    std::string bits_str = x.get_str(exp, 2);
//    cout << exp << endl;
//    cout << bits_str << endl;


//    cout << "size mpf=" << sizeof(mpf_class) << endl;
//    cout << "size mpf instance?=" << sizeof(f) << endl;
//    cout << "size mpz=" << sizeof(mpz_class) << endl;
//    cout << "size vector=" << sizeof(vector<int>) << endl;
//    cout << "size vector vector int=" << sizeof(vector<vector<int>>) << endl;
//    cout << "size void*=" << sizeof(void *) << endl;
//    cout << "size int=" << sizeof(int) << endl;

    FullyScheme fhe(8, time(nullptr));
    std::cout << "theta = " << fhe.theta << std::endl;
    std::cout << "n = " << fhe.n << std::endl;
    auto [secret_key, public_key] = fhe.key_gen();
    cout << "key_gen" << endl;
    auto c = fhe.encrypt(public_key.pk, NTL::GF2{0});
    cout << "c<0? = " << (c < 0) << endl;

    std::vector<mpf_class> zf(public_key.y.size());

    for (int i = 0; i < zf.size(); i++) {
        zf[i].set_prec(fhe.n + 1);
        zf[i] = mod2f(public_key.y[i] * c); // fixme?
    }

    mpf_class sum(0, fhe.n + 1);
    for (int i = 0; i < zf.size(); i++) {
        if (IsOne(secret_key.s[i])) {
            sum += zf[i];
            sum = mod2f(sum);
        }
    }

    std::cout << "sum = " << sum << std::endl;

    auto bits = mpf_to_bits(sum, 5);
    std::cout << bits[1] << std::endl;
    std::cout << bits[2] << std::endl;
    if (bits[1] != bits[2]) {
        cout << "FALSE" << endl;
    }

    mpf_class c_div_p(c, fhe.kappa + 2);
    c_div_p /= secret_key.p;
    cout << "c/p = " << c_div_p << endl;
    c_div_p = mod2f(c_div_p);
    cout << "c/p mod2 = " << c_div_p << endl;

//    mpf_class f(-121.8);
//    cout << round_to_closest(f) << endl;

//    auto [c_star, z] = fhe.post_process(c0, public_key.y);
//    mpf_class sum(0, fhe.n + 1);
//
//    std::cout << "zi size = " << z[0].size() << std::endl;
//
//    for (int i = 0; i < z.size(); i++) {
//        if (IsOne(secret_key.s[i])) {
//            mpf_class curr = bits_to_mpf(z[i], fhe.n + 1);
//            sum += curr;
//            sum = mod2f(sum);
//        }
//    }
//    std::cout << "sum = " << sum << std::endl;
//
//    auto bits = mpf_to_bits(sum, 5);
//    std::cout << bits[1] << std::endl;
//    std::cout << bits[2] << std::endl;
//
//    mpf_class c_div_p(c0, fhe.kappa + 2);
//    c_div_p /= secret_key.p;
//    c_div_p = mod2f(c_div_p);
//    cout << "c/p = " << c_div_p << endl;

    return 0;
}
