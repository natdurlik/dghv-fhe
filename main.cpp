#include <iostream>
#include <gmpxx.h>
#include "src/SomewhatScheme.h"

using namespace std;

bool poly1(bool m1, bool m2, bool m3) {
    return 0;
}

int main() {

    cout << "Scheme" << endl;

    SomewhatScheme scheme(8, time(nullptr));

    bool m1 = 1;
    bool m2 = 0;
    bool m3 = 0;
    bool m4=m1+m1;
    cout<<m4<<endl;


    mpz_class sk;
    vector<mpz_class> pk;
    tie(sk, pk) = scheme.key_gen();

    cout << "pub key size= " << pk.size() << endl;
    cout << "sk= " << sk << endl;
    cout << is_even(sk) << endl;
    cout << pk[0] << endl;
    cout << "largest idx=" << distance(pk.begin(), max_element(pk.begin(), pk.end())) << endl;
    cout << "pk even: " << is_even(pk[0]) << endl;
    cout << "pk rest even: " << is_even(rem(pk[0], sk)) << endl;
    cout << endl;

    mpz_class c1 = scheme.encrypt(pk, m1);
    mpz_class c2 = scheme.encrypt(pk, m2);
    mpz_class c3 = scheme.encrypt(pk, m3);

    mpz_class ev = c1 * c2 + c3;
    mpz_class dec_m = scheme.decrypt(sk, ev);

    cout << "decrypted " << dec_m << endl;
    cout << "should be " << 0 << endl;



    return 0;
}
