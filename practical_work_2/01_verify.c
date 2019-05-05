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

    mpz_t d, n, h, s;
    mpz_inits(d, n, h, s, NULL);

    char *key, *value;
    while (fscanf(f_key, "%m[^=]=%ms\n", &key, &value) == 2)
    {
        switch (key[0])
        {
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

    // calculate s = h^d mod n
    mpz_powm(s, h, d, n);

    gmp_printf("MD5 Signature: %Zx\n", s);

    fclose(f_key);
    return 0;
}
