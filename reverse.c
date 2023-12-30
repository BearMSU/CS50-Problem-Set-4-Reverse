#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "wav.h"

int check_format(WAVHEADER header);
int get_block_size(WAVHEADER header);

int main(int argc, char *argv[])
{
    // Ensure proper usage
    // TODO #1
    if (argc != 3)
    {
        printf("Improper Usage! Usage: ./reverse input.wav output.wav \n");
        return 1;
    }

    // Open input file for reading
    // TODO #2
    char *fileIn = argv[1];
    char *fileOut = argv[2];

    FILE *input = fopen(fileIn, "r");
    if (input == NULL)
    {
        printf("Error: Could not open %s. \n", fileIn);
        return 1;
    }

    // Read header
    // TODO #3
    WAVHEADER header;
    fread(&header, sizeof(WAVHEADER), 1, input);

    // Use check_format to ensure WAV format
    // TODO #4
    if (check_format(header) != 0)
    {
        fclose(input);
        return 1;
    }

    // Open output file for writing
    // TODO #5
    FILE *output = fopen(fileOut, "w");
    if (output == NULL)
    {
        printf("Error: could not open %s. Output file unsuccessful.\n", fileOut);
        fclose(output);
        return 1;
    }

    // Write header to file
    // TODO #6
    fwrite(&header, sizeof(WAVHEADER), 1, output);

    // Use get_block_size to calculate size of block
    // TODO #7
    int size = get_block_size(header);

    // Write reversed audio to file
    // TODO #8
    if (fseek(input, size, SEEK_END))
    {
        return 1;
    }

    BYTE buffer[size];
    while (ftell(input) - size > sizeof(header))
    {
        if (fseek(input, -2 * size, SEEK_CUR))
        {
            return 1;
        }
        fread(&buffer, size, 1, input);
        fwrite(&buffer, size, 1, output);
    }

    fclose(input);
    fclose(output);
}

int check_format(WAVHEADER header)
{
    // TODO #4
    int wav[] = {0x57, 0x41, 0x56, 0x45};
    for (int i = 0; i < 4; i++)
    {
        if (header.format[i] != wav[i])
        {
            printf("Error: invalid format. File must be of type .wav! \n");
            return 1;
        }
    }
    return 0;
}

int get_block_size(WAVHEADER header)
{
    // TODO #7
    int s;
    s = header.numChannels * (header.bitsPerSample / 8);

    return s;
}
