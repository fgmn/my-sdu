#include "syscall.h"
int main()
{
    char fn[5];
    char cont[12];
    char buffer[50];
    int fd;
    char c;
    int i;
    OpenFileId input = ConsoleInput;
    OpenFileId output = ConsoleOutput;
    fn[0] = 'f'; fn[1] = 'i'; fn[2] = 'l'; fn[3] = 'e'; fn[4] = '\0';
    cont[0] = 'H'; cont[1] = 'e'; cont[2] = 'l'; cont[3] = 'l'; cont[4] = 'o'; cont[5] = ' '; 
    cont[6] = 'w'; cont[7] = 'o'; cont[8] = 'r'; cont[9] = 'l'; cont[10] = 'd'; cont[11] = '\0';
    fd = Open(fn);
    Write(cont, 11, output);
    Write(cont, 11, fd);

    Read(buffer, 20, fd);
    Close(fd);
    Exit(99);
}