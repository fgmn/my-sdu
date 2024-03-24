#include "syscall.h"
int main()
{   // shell：交互式应用程序
    SpaceId newProc;
    OpenFileId input = ConsoleInput;
    OpenFileId output = ConsoleOutput;
    char prompt[7], ch, buffer[60];
    char Hbuffer[80];
    char Cbuffer[80];
    int i, j, k, h, m;
    char c;
    // 提示词
    prompt[0] = 'N';
    prompt[1] = 'a';
    prompt[2] = 'c';
    prompt[3] = 'h';
    prompt[4] = 'o';
    prompt[5] = 's';
    prompt[6] = '$';
    prompt[7] = ':';

    while (1)
    {
        Write(prompt, 8, output);
        i = 0;
        k = 0;
        h = 0;
        m = 0;
        do
        {
            // 读取终端输入
            Read(&c, 1, input);

            buffer[i] = c;
            Hbuffer[h++] = buffer[i];

            k++;
        } while (buffer[i++] != '\n');

        Hbuffer[--h] = '\0';

        buffer[--i] = '.';
        buffer[i++] = '.';
        buffer[i++] = 'n';
        buffer[i++] = 'o';
        buffer[i++] = 'f';
        buffer[i++] = 'f';
        buffer[i] = '\0';

        if (k == 1)
            continue;
        if (h > 0)
        {
            newProc = Exec(Hbuffer);

            if (newProc == -1)
                //Write("Invalid Command, Enter again.", 30, output);
                // 利用Write系统调用进行输出验证
                Write("Invalid Command, Enter again, or try \"help\"\n", 44, output);
            else if (newProc != 127)
            {
                Join(newProc);

                Write("Command \"", 9, output);
                Write(Hbuffer, k, output);
                Write("\" Execute Completely.\n", 22, output);
            }
        }
    }
}
