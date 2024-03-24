#include "pcb.h"

Pcb::Pcb()
{
    for (int i = 0; i < MaxFileId; i++)
        fileIdUse[i] = false;
}

void Pcb::setExitStatus(int ExitStatus)
{
    this->exitStatus = ExitStatus;
}

int Pcb::getExitStatus()
{
    return exitStatus;
}

Pcb::~Pcb()
{
    if (space != NULL)
    {
        DEBUG('x', "delete space\n");
        delete space;
        space = NULL;
    }
    for (int i = 0; i < MaxFileId; i++)
        if (fileIdUse[i])
        {
            fileIdUse[i] = false;
            delete files[i];
            files[i] = NULL;
        }
}

int Pcb::addFile(OpenFile *openfile)
{
    for (int i = 2; i < MaxFileId; i++)
        if (fileIdUse[i] && files[i] == openfile)
        {
            printf("duplicate file\n");
            ASSERT(false);
            break;
        }
    for (int i = 2; i < MaxFileId; i++)
        if (!fileIdUse[i])
        {
            DEBUG('x', "addFile success\n");
            fileIdUse[i] = true;
            files[i] = openfile;
            return i;
        }
    printf("file full\n");
    ASSERT(false);
    return -1;
}

OpenFile *Pcb::getFile(int fileId)
{
    if (fileIdUse[fileId])
        return files[fileId];
    else
        return NULL;
}

void Pcb::releaseFile(int fileId)
{
    if (fileIdUse[fileId])
    {
        fileIdUse[fileId] = false;
        delete files[fileId];
        files[fileId] = NULL;
    }
    else
    {
        printf("thr file to close not find\n");
        ASSERT(false);
    }
}