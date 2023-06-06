#ifndef DGHV_FHE_PUBLICKEY_H
#define DGHV_FHE_PUBLICKEY_H

#include <gmpxx.h>

class PublicKey {
public:
    std::vector<mpz_class> pk;
    std::vector<mpf_class> y;
    std::vector<mpz_class> e_sk;
};


#endif //DGHV_FHE_PUBLICKEY_H
