#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "aes.c"

#define ENCRYPT 1
#define DECRYPT 2

typedef unsigned char BYTE;

void aes_encrypt(BYTE *cipher, BYTE *message, BYTE *key);

const BYTE iv[16] = {0};
char xor_key[16];

void aes_encrypt_cbc_init()
{
    memcpy(xor_key, iv, 16);
}

void aes_encrypt_cbc(BYTE *cipher, BYTE *message, BYTE *key)
{
    BYTE xor_mess[16];
    //new message = message XOR key
    for (int i = 0; i < 16; i++)
    {
        xor_mess[i] = message[i] ^ xor_key[i];
    }
    // Encrypted with new message
    aes_encrypt(cipher, xor_mess, key);
    // Save new message encrypted into xor_key
    memcpy(xor_key, cipher, 16);
}

int main(int argc, char const *argv[])
{
    if (argc != 2)
    {
        printf(
            "Invalid execution format. Use the following format\n"
            "<executable code> <input file>\n");
        return -1;
    }

    // Enter the byte
    BYTE K[16] = {0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6, 0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C};

    BYTE buffer[16];
    BYTE buffer_enc[16];

    FILE *input = fopen(argv[1], "rb");
    FILE *output = fopen(argv[2], "wb");

    if (input == NULL)
    {
        printf("Can't open the file %s\n", argv[1]);
        return -1;
    }
    aes_encrypt_cbc_init();

    while (1)
    {
        memset(buffer, 0, 16);
        int len = fread(buffer, 1, 16, input);
        if (len <= 0)
        {
            break;
        }
        aes_encrypt_cbc(buffer_enc, buffer, K);
        fwrite(buffer_enc, 1, 16, output);
    }

    printf("Sucessful!\n");
    return 0;
}