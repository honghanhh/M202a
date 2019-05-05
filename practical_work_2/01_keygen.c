#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include "gmp.h"

void gen_ramdom_prime(gmp_randstate_t prng, mpz_t output, int k)
{
    mpz_init_set_ui(output, 0);

    // get a random number
    mpz_urandomb(output, prng, k);

    // find a prime
    while (!mpz_probab_prime_p(output, 25))
    {
        mpz_add_ui(output, output, 1);
    }
}

int main(int argc, char const *argv[])
{
    if (argc != 3)
    {
        printf(
            "Invalid execution format. Use the following format\n"
            "<executable code> <k-bit> <public exponent>\n");
        return -1;
    }

    int k = atoi(argv[1]);
    int e_i = atoi(argv[2]);

    int key = open("key.txt", O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);

    if (!key)
    {
        printf("Can't open file\n");
        return -1;
    }
    dup2(key, STDOUT_FILENO);

    mpz_t e;
    mpz_init_set_ui(e, e_i);

    // gen prime number p, q
    mpz_t p, q;
    mpz_t p_1, q_1, gcd;

    mpz_init_set_ui(p_1, 0);
    mpz_init_set_ui(q_1, 0);
    mpz_init_set_ui(gcd, 0);

    gmp_randstate_t prng;       // Random generator declaration
    gmp_randinit_default(prng); // Random generator initialisation
    gmp_randseed_ui(prng, time(NULL));

    do
    {
        gen_ramdom_prime(prng, p, k);
        mpz_sub_ui(p_1, p, 1); // p_1 = p -1
        mpz_gcd(gcd, e, p_1);

    } while (mpz_cmp_ui(gcd, 1) != 0);

    do
    {
        gen_ramdom_prime(prng, q, k);
        mpz_sub_ui(q_1, q, 1); // q_1 = q -1
        mpz_gcd(gcd, e, q_1);

    } while (mpz_cmp_ui(gcd, 1) != 0);

    // calculate n = p*q
    mpz_t n;
    mpz_init_set_ui(n, 0);
    mpz_mul(n, p, q);

    // calculate d
    mpz_t phi, d;
    mpz_init_set_ui(phi, 0);
    mpz_init_set_ui(d, 0);

    mpz_mul(phi, p_1, q_1); // phi = (p - 1)*(q - 1)
    mpz_invert(d, e, phi);  // inversion of e modulo phi

    //print the output
    gmp_printf("e = 0x%Zx\n", e);
    gmp_printf("n = 0x%Zx\n", n);
    gmp_printf("d = 0x%Zx\n", d);

    gmp_randclear(prng);
    mpz_clears(p, q, e, phi, d, n, p_1, q_1, gcd, NULL);
    close(key);

    return 0;
}
