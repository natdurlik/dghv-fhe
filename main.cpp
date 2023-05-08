#include <iostream>
#include <gmpxx.h>
#include "src/SomewhatScheme.h"

using namespace std;


int main() {
    cout << "Hello, World!" << endl;

//    mpz_class a;
//    mpz_ui_pow_ui(a.get_mpz_t(), 2, 500);
//    cout << a << endl;

    gmp_randclass rand(gmp_randinit_mt);
    rand.seed(time(nullptr));

//    mpz_class b = rand.get_z_bits(3);
//
//    mpz_class c;
//    mpz_ui_pow_ui(c.get_mpz_t(), 2, 3);
//    cout<<c<<endl;
//
//    for(int i=0;i<50;i++) {
//        cout<<rand.get_z_bits(1)<<" ";
//    }
//    cout<<endl;

    SomewhatScheme scheme(1,time(nullptr));

    mpz_class m=true;
    cout<<m<<endl;
    return 0;
}
