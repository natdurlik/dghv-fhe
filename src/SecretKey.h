#ifndef DGHV_FHE_SECRETKEY_H
#define DGHV_FHE_SECRETKEY_H

#include <gmpxx.h>
#include <NTL/GF2.h>

class SecretKey {
public:
    mpz_class p;
    std::vector<NTL::GF2> s;
};


#endif //DGHV_FHE_SECRETKEY_H
