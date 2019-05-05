#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include "gmp.h"

void encrpyt_rsa(mpz_t cipher, const mpz_t mess, const mpz_t n, const mpz_t e)
{
    mpz_powm(cipher, mess, e, n); // cipher = mess^e mod n
}

void decrypt_rsa(mpz_t mess, const mpz_t cipher, const mpz_t n, const mpz_t p, const mpz_t q, const mpz_t dp, const mpz_t dq, const mpz_t qi)
{
    mpz_t mp, mq, temp;
    mpz_inits(mp, mq, temp, NULL);
    mpz_powm(mp, cipher, dp, p); // mp = cipher^dp mod p
    mpz_powm(mq, cipher, dq, q); // mq = cipher^dq mod q
    mpz_sub(temp, mq, mp);       // temp = mq - mp;
    mpz_mul(temp, temp, qi);     // temp *= Ip;
    mpz_mod(temp, temp, q);      // temp %= q;
    mpz_mul(temp, temp, p);      // temp *= p;
    mpz_add(mess, mp, temp);     // m = mp + temp;
}

int main(int argc, char const *argv[])
{
    if (argc != 2)
    {
        printf(
            "Invalid execution format. Use the following format\n"
            "<executable code> <message>\n");
        return -1;
    }

    FILE *f_key = fopen("key_crt.txt", "rb");
    if (!f_key)
    {
        printf("Can't open key file\n");
        return -1;
    }

    mpz_t e, n, p, q, dp, dq, qi, mess;
    mpz_inits(e, n, p, q, dp, dq, qi, mess, NULL);

    mpz_set_str(mess, argv[1], 0);

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

    mpz_t cipher, decrypt;
    mpz_init_set_ui(cipher, 0);
    mpz_init_set_ui(decrypt, 0);

    printf("Encrypt:\n");
    printf("Message: 0x%s\n", mpz_get_str(NULL, 16, mess));
    encrpyt_rsa(cipher, mess, n, e);
    printf("Cipher: 0x%s\n", mpz_get_str(NULL, 16, cipher));

    printf("\nDecrypt:\n");
    printf("Cipher: 0x%s\n", mpz_get_str(NULL, 16, cipher));
    decrypt_rsa(decrypt, cipher, n, p, q, dp, dq, qi);
    printf("Message: 0x%s\n", mpz_get_str(NULL, 16, decrypt));

    fclose(f_key);
    return 0;
}
