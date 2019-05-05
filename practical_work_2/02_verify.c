#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gmp.h"

int main(int argc, char const *argv[])
{
    if (argc != 3)
    {
        printf(
            "Invalid execution format. Use the following format\n"
            "<executable code> <input file> <RSA key file>\n");
        return -1;
    }

    FILE *f_key = fopen(argv[2], "rb");
    if (!f_key)
    {
        printf("Can't open key file\n");
        return -1;
    }

    mpz_t e, n, p, q, dp, dq, qi, mess, h;
    mpz_inits(e, n, p, q, dp, dq, qi, mess, h, NULL);

    char *key, *value;
    while (fscanf(f_key, "%m[^=]=%ms\n", &key, &value) == 2)
    {
        switch (key[0])
        {
        case 'e':
            mpz_set_str(e, value, 0);
            break;
        case 'n':
            mpz_set_str(n, value, 0);
            break;
        case 'p':
            mpz_set_str(p, value, 0);
            break;
        case 'd':
            if (key[1] == 'p')
            {
                mpz_set_str(dp, value, 0);
            }
            else if (key[1] == 'q')
            {
                mpz_set_str(dq, value, 0);
            }
            break;
        case 'q':
            if (key[1] == 'i')
            {
                mpz_set_str(qi, value, 0);
            }
            else
            {
                mpz_set_str(q, value, 0);
            }

            break;
        default:
            break;
        }
    }

    // calculate md5 of file
    char md5[32] = {0};
    char *md5_cmd = malloc(sizeof(argv[1]) + 10);
    sprintf(md5_cmd, "md5sum %s", argv[1]);
    FILE *f_md5 = popen(md5_cmd, "r");
    if (f_md5)
    {
        fgets(md5, 33, f_md5);
        pclose(f_md5);
    }

    printf("MD5: %s\n", md5);
    mpz_set_str(h, md5, 16);

    // calculate sig
    mpz_t s, mp, mq, temp;
    mpz_inits(s, mp, mq, temp, NULL);

    mpz_powm(mp, h, dp, p);  // mp = h^dp mod p
    mpz_powm(mq, h, dq, q);  // mq = h^dq mod q
    mpz_sub(temp, mq, mp);   // temp = mq - mp;
    mpz_mul(temp, temp, qi); // temp *= qi;
    mpz_mod(temp, temp, q);  // temp %= q;
    mpz_mul(temp, temp, p);  // temp *= p;
    mpz_add(s, mp, temp);    // m = mp + temp

    gmp_printf("MD5 Signature: %Zx\n", s);

    fclose(f_key);
    return 0;
}
