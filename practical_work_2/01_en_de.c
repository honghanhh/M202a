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

void decrypt_rsa(mpz_t mess, const mpz_t cipher, const mpz_t n, const mpz_t d)
{
    mpz_powm(mess, cipher, d, n); // mess = cipher^d mod n
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

    FILE *f_key = fopen("key.txt", "rb");
    if (!f_key)
    {
        printf("Can't open key file\n");
        return -1;
    }

    mpz_t d, e, n, mess;
    mpz_inits(d, e, n, mess, NULL);

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
        case 'd':
            mpz_set_str(d, value, 0);
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
    decrypt_rsa(decrypt, cipher, n, d);
    printf("Message: 0x%s\n", mpz_get_str(NULL, 16, decrypt));

    fclose(f_key);
    return 0;
}
