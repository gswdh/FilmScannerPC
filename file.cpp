#include "file.h"

file::file(char name[], unsigned int width, unsigned int height, unsigned int bit_depth)
{
    // Form the file name

    // Open or create the file
    pFile = fopen(name, "wb");

    // Construct the header
    fprintf(pFile, "P5\n%d %d\n%d\n", width, height, bit_depth);
}

void file::write_line(unsigned short int line[])
{
    // Write the data to file
    fwrite(line, sizeof(unsigned short int), sizeof(line), pFile);
}

void file::close(void)
{
    // Close the file
    fclose(pFile);
}
