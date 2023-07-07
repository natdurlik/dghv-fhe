#ifndef DGHV_FHE_PUBLICKEY_H
#define DGHV_FHE_PUBLICKEY_H

#include <gmpxx.h>
#include <memory>

class PublicKey {
public:
    std::vector<mpz_class> pk;
    std::vector<mpf_class> y;
    std::vector<mpz_class> e_sk;
    mpz_class mod_red;
};


#endif //DGHV_FHE_PUBLICKEY_H
