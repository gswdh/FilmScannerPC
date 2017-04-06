#ifndef FILE_H
#define FILE_H

#include <QString>

#include <fstream>

#define BIT_DEPTH_8 255
#define BIT_DEPTH_16 65535

class file
{
public:
    file(char name[], unsigned int width, unsigned int height, unsigned int bit_depth);
    void write_line(unsigned short int line[]);
    void close(void);

private:
    FILE* pFile;
};

#endif // FILE_H
