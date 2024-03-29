#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef unsigned char BYTE;
void aes_encrypt(BYTE *cipher, BYTE *message, BYTE *key);

int main(int argc, char const *argv[])
{
    if (argc != 3)
    {
        printf(
            "Invalid execution format. Use the following format\n"
            "<executable code> <input file> <output file>\n");
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
    if (output == NULL)
    {
        printf("Can't open the file %s\n", argv[2]);
        return -1;
    }

    while (1)
    {
        memset(buffer, 0, 16);
        int len = fread(buffer, 1, 16, input);
        if (len <= 0)
        {
            break;
        }
        for (int i = len; i < 16; ++i)
        {
            buffer[i] = 16 - len;
        }
        aes_encrypt(buffer_enc, buffer, K);
        fwrite(buffer_enc, 1, 16, output);
    }
    fclose(input);
    fclose(output);
    printf("Sucessful encrypted!\n");
    return 0;
}