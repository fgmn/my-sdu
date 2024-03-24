// fstest.cc 
//	Simple test routines for the file system.  
//
//	We implement:
//	   Copy -- copy a file from UNIX to Nachos
//	   Print -- cat the contents of a Nachos file 
//	   Perftest -- a stress test for the Nachos file system
//		read and write a really large file in tiny chunks
//		(won't work on baseline system!)
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"

#include "utility.h"
#include "filesys.h"
#include "system.h"
#include "thread.h"
#include "disk.h"
#include "stats.h"
#include "filesys.h"
#include "directory.h"

#define TransferSize 	10 	// make it small, just to be difficult

//----------------------------------------------------------------------
// Copy
// 	Copy the contents of the UNIX file "from" to the Nachos file "to"
//----------------------------------------------------------------------

void CopyTest(char *from, char *to)
{
    FILE *fp;
    OpenFile *openFile;
    int amountRead, fileLength;
    char *buffer;

    // Open UNIX file
    if ((fp = fopen(from, "r")) == NULL)
    {
        printf("Copy: couldn't open input file %s\n", from);
        return;
    }

    // Figure out length of UNIX file
    fseek(fp, 0, 2);
    fileLength = ftell(fp);
    fseek(fp, 0, 0);

    // Create a Nachos file of the same length
    DEBUG('f', "Copying file %s, size %d, to file %s\n", from, fileLength, to);
    printf("u:1\n");
    if (!fileSystem->CreateTest(to, fileLength))
    { // Create Nachos file
        printf("Copy: couldn't create output file %s\n", to);
        fclose(fp);
        return;
    }
    printf("u:2\n");
    openFile = fileSystem->OpenTest(to);
    ASSERT(openFile != NULL);

    // Copy the data in TransferSize chunks
    buffer = new char[TransferSize];
    while ((amountRead = fread(buffer, sizeof(char), TransferSize, fp)) > 0)
        openFile->Write(buffer, amountRead);
    delete[] buffer;

    // Close the UNIX and the Nachos files
    delete openFile;
    fclose(fp);
    printf("u:CopyTest done!\n");
}

void AppendTest(char *from, char *to, int half)
{
    FILE *fp;
    OpenFile *openFile;
    int amountRead, fileLength;
    char *buffer;

    //  start position for appending
    int start;

    // Open UNIX file
    if ((fp = fopen(from, "r")) == NULL)
    {
        printf("Copy: couldn't open input file %s\n", from);
        return;
    }

    // Figure out length of UNIX file
    fseek(fp, 0, 2);
    fileLength = ftell(fp);
    fseek(fp, 0, 0);

    if (fileLength == 0)
    {
        printf("Append: nothing to append from file %s\n", from);
        return;
    }

    if ((openFile = fileSystem->OpenTest(to)) == NULL)
    {
        // file "to" does not exits, then create one
        if (!fileSystem->CreateTest(to, 0))
        {
            printf("Append: couldn't create the file %s to append\n", to);
            fclose(fp);
            return;
        }
        openFile = fileSystem->OpenTest(to);
    }

    ASSERT(openFile != NULL);
    // append from position "start"
    start = openFile->Length();
    if (half)
        start = start / 2;
    openFile->Seek(start);

    // Append the data in TransferSize chunks
    buffer = new char[TransferSize];
    while ((amountRead = fread(buffer, sizeof(char), TransferSize, fp)) > 0)
    {
        int result;
        //	printf("start value: %d,  amountRead %d, ", start, amountRead);
        //	result = openFile->WriteAt(buffer, amountRead, start);
        result = openFile->Write(buffer, amountRead);
        //	printf("result of write: %d\n", result);
        // ASSERT(result == amountRead);
        //	start += amountRead;
        //	ASSERT(start == openFile->Length());

        if (result < 0) //文件过大，或空闲磁盘块不足
        {
            printf("\nERROR!!!!!!\n");
            printf("Insuficient Disk Space, or File is Too Big!\n");
            printf("Writting Terminated.\n\n");
            break;
        }
        ASSERT(result == amountRead);
        start += amountRead;
    }
    delete[] buffer;

    // Write the inode back to the disk, because we have changed it
    openFile->WriteBack();
    printf("inodes have been written back\n");

    // Close the UNIX and the Nachos files
    delete openFile;
    fclose(fp);
}

//----------------------------------------------------------------------
// NAppend
// 	NAppend is the same as Append except that the "from" file is a
//         Nachos file instead of a UNIX file. It appends the contents
//         of Nachos file "from" to the end of Nachos file "to". NAppend与Append相同，只是“from”文件是Nachos文件，而不是UNIX文件。它将Nachos文件的内容“from”附加到Nachos文件的末尾“to”。

//      If Nachos file "to" does not exist, create the nachos file
//         "to" with lengh 0, then append the contents of UNIX file
//         "from" to the end of it. 如果Nachos文件“to”不存在，请创建长度为0的Nachos文件“to”，然后将UNIX文件“from”的内容追加到其末尾。
//----------------------------------------------------------------------

void NAppendTest(char *from, char *to)
{
    OpenFile *openFileFrom;
    OpenFile *openFileTo;
    int amountRead, fileLength;
    char *buffer;

    //  start position for appending
    int start;

    // if (!strncmp(from, to, FileNameMaxLen))
    // {
    //     //  "from" should be the same as "to"
    //     printf("NAppend: should be different files\n");
    //     return;
    // }

    if (!strcmp(from, to))
    {
        //  "from" should be the same as "to"
        printf("NAppend: should be different files\n");
        return;
    }

    if ((openFileFrom = fileSystem->OpenTest(from)) == NULL)
    {
        // file "from" does not exits, give up
        printf("NAppend:  file %s does not exist\n", from);
        return;
    }

    fileLength = openFileFrom->Length();
    if (fileLength == 0)
    {
        printf("NAppend: nothing to append from file %s\n", from);
        return;
    }

    if ((openFileTo = fileSystem->OpenTest(to)) == NULL)
    {
        // file "to" does not exits, then create one
        if (!fileSystem->CreateTest(to, 0))
        {
            printf("Append: couldn't create the file %s to append\n", to);
            delete openFileFrom;
            return;
        }
        openFileTo = fileSystem->OpenTest(to);
    }

    ASSERT(openFileTo != NULL);
    // append from position "start"
    start = openFileTo->Length();
    openFileTo->Seek(start);

    // Append the data in TransferSize chunks
    buffer = new char[TransferSize];
    openFileFrom->Seek(0);
    while ((amountRead = openFileFrom->Read(buffer, TransferSize)) > 0)
    {
        int result;
        //	printf("start value: %d,  amountRead %d, ", start, amountRead);
        //	result = openFile->WriteAt(buffer, amountRead, start);
        result = openFileTo->Write(buffer, amountRead);
        //	printf("result of write: %d\n", result);
        // ASSERT(result == amountRead);
        //	start += amountRead;
        //	ASSERT(start == openFile->Length());

        if (result < 0)
        {
            printf("\nERROR!!!!!!\n");
            printf("Insuficient Disk Space, or File is Too Big!\n");
            printf("Writting Terminated.\n\n");
            break;
        }
        ASSERT(result == amountRead);
        start += amountRead;
    }
    delete[] buffer;

    // Write the inode back to the disk, because we have changed it
    openFileTo->WriteBack();
    printf("inodes have been written back\n");

    // Close both Nachos files
    delete openFileTo;
    delete openFileFrom;
}

void
Copy(char *from, char *to)
{
    FILE *fp;
    OpenFile* openFile;
    int amountRead, fileLength;
    char *buffer;

// Open UNIX file
    if ((fp = fopen(from, "r")) == NULL) {	 
	printf("Copy: couldn't open input file %s\n", from);
	return;
    }

// Figure out length of UNIX file
    fseek(fp, 0, 2);		
    fileLength = ftell(fp);
    fseek(fp, 0, 0);

// Create a Nachos file of the same length
    DEBUG('f', "Copying file %s, size %d, to file %s\n", from, fileLength, to);
    // to:文件名，文件大小
    if (!fileSystem->Create(to, fileLength)) {	 // Create Nachos file
	printf("Copy: couldn't create output file %s\n", to);
	fclose(fp);
	return;
    }
    
    openFile = fileSystem->Open(to);
    ASSERT(openFile != NULL);
    
    // 通过UNIX系统的文件读操作将文件内容读出，
    // 通过Nachos系统的OpenFile类的写方法将内容写入DISK中

// Copy the data in TransferSize chunks
    buffer = new char[TransferSize];
    while ((amountRead = fread(buffer, sizeof(char), TransferSize, fp)) > 0)
	openFile->Write(buffer, amountRead);	
    delete [] buffer;

// Close the UNIX and the Nachos files
    delete openFile;
    fclose(fp);
}


//----------------------------------------------------------------------
// Append
// 	Append the contents of the UNIX file "from" to the Nachos file "to".
//        if  "half" is non-zero, start at the middle of file "to" to
//        appending the contents of "from"; Otherwise, do real appneding
//        after the end of file "to".
//
//      If Nachos file "to" does not exist, create the nachos file 
//         "to" with lengh 0, then append the contents of UNIX file 
//         "from" to the end of it.
//----------------------------------------------------------------------

void
Append(char *from, char *to, int half)
{
    FILE *fp;
    OpenFile* openFile;
    int amountRead, fileLength;
    char *buffer;

//  start position for appending
    int start;

// Open UNIX file
    if ((fp = fopen(from, "r")) == NULL) {	 
	printf("Copy: couldn't open input file %s\n", from);
	return;
    }

// Figure out length of UNIX file
    fseek(fp, 0, 2);		
    fileLength = ftell(fp);
    fseek(fp, 0, 0);

    if (fileLength == 0) 
    {
	printf("Append: nothing to append from file %s\n", from);
	return;
    }
	 
    if ( (openFile = fileSystem->Open(to)) == NULL)
    {
	// file "to" does not exits, then create one
	if (!fileSystem->Create(to, 0)) 
	{
	    printf("Append: couldn't create the file %s to append\n", to);
	    fclose(fp);
	    return;
	}
	openFile = fileSystem->Open(to);
    }

    ASSERT(openFile != NULL);
    // append from position "start"
    start = openFile->Length();
    // half=0:从文件中间开始附加
    // 否则从文件末尾开始附加
    if (half) start = start / 2;
    openFile->Seek(start);

// Append the data in TransferSize chunks
    buffer = new char[TransferSize];
    while ((amountRead = fread(buffer, sizeof(char), TransferSize, fp)) > 0) 
    {
//         // 利用隐式文件指针进行操作
//         int result;
// //	printf("start value: %d,  amountRead %d, ", start, amountRead);
// //	result = openFile->WriteAt(buffer, amountRead, start);
// 	result = openFile->Write(buffer, amountRead);
// //	printf("result of write: %d\n", result);
// 	ASSERT(result == amountRead);
// //	start += amountRead;
// //	ASSERT(start == openFile->Length());

        // modified
        int result;
        result = openFile->Write(buffer, amountRead);
        if (result < 0) {
            printf("\nERROR!!!!!\n");
            printf("Insuficient Disk Space, or File is Too Big!\n");
            printf("Writting Terminated.\n\n");
            break;
        }
        ASSERT(result == amountRead);
        start += amountRead;

    }
    delete [] buffer;

// Write the inode back to the disk, because we have changed it
    openFile->WriteBack();
    printf("inodes have been written back\n");
    
// Close the UNIX and the Nachos files
    delete openFile;
    fclose(fp);
}

//----------------------------------------------------------------------
// NAppend
// 	NAppend is the same as Append except that the "from" file is a
//         Nachos file instead of a UNIX file. It appends the contents
//         of Nachos file "from" to the end of Nachos file "to".

//      If Nachos file "to" does not exist, create the nachos file 
//         "to" with lengh 0, then append the contents of UNIX file 
//         "from" to the end of it.
//----------------------------------------------------------------------

void
NAppend(char *from, char *to)
{
    OpenFile* openFileFrom;
    OpenFile* openFileTo;
    int amountRead, fileLength;
    char *buffer;

    //  start position for appending
    int start;

    if (!strncmp(from, to, FileNameMaxLen))
    {   // 同一文件，不进行操作
	//  "from" should be the same as "to"
	printf("NAppend: should be different files\n");
	return;
    }

    if ( (openFileFrom = fileSystem->Open(from)) == NULL)
    {   // 源文件不存在
	// file "from" does not exits, give up
	printf("NAppend:  file %s does not exist\n", from);
	return;
    }

    fileLength = openFileFrom->Length();
    if (fileLength == 0) 
    {   // 源文件为空
	printf("NAppend: nothing to append from file %s\n", from);
	return;
    }
	 
    if ( (openFileTo = fileSystem->Open(to)) == NULL)
    {
	// file "to" does not exits, then create one
	if (!fileSystem->Create(to, 0)) 
	{   // 目标文件创建失败
	    printf("Append: couldn't create the file %s to append\n", to);
	    delete openFileFrom;
	    return;
	}
	openFileTo = fileSystem->Open(to);
    }

    ASSERT(openFileTo != NULL);
    // append from position "start"
    start = openFileTo->Length();
    openFileTo->Seek(start);
    
// Append the data in TransferSize chunks
    buffer = new char[TransferSize];
    openFileFrom->Seek(0);
    while ( (amountRead = openFileFrom->Read(buffer, TransferSize)) > 0) 
    {   // amountRead：读取到的字节数
//         int result;
// //	printf("start value: %d,  amountRead %d, ", start, amountRead);
// //	result = openFile->WriteAt(buffer, amountRead, start);
// 	result = openFileTo->Write(buffer, amountRead);
// //	printf("result of write: %d\n", result);
// 	ASSERT(result == amountRead);
// //	start += amountRead;
// //	ASSERT(start == openFile->Length());
        // modified
        int result;
        result = openFileTo->Write(buffer, amountRead);
        if (result < 0) {
            printf("\nERROR!!!!!\n");
            printf("Insuficient Disk Space, or File is Too Big!\n");
            printf("Writting Terminated.\n\n");
            break;
        }
        ASSERT(result == amountRead);
        start += amountRead;    // 实际文件指针会自己移动，这里外部维护一个副本
    }
    delete [] buffer;

// Write the inode back to the disk, because we have changed it
    openFileTo->WriteBack();
    printf("inodes have been written back\n");
    // 
// Close both Nachos files
    delete openFileTo;
    delete openFileFrom;
}




//----------------------------------------------------------------------
// Print
// 	Print the contents of the Nachos file "name".
//----------------------------------------------------------------------

void
Print(char *name)
{
    OpenFile *openFile;    
    int i, amountRead;
    char *buffer;

    if ((openFile = fileSystem->Open(name)) == NULL) {
	printf("Print: unable to open file %s\n", name);
	return;
    }
    
    buffer = new char[TransferSize];
    while ((amountRead = openFile->Read(buffer, TransferSize)) > 0)
	for (i = 0; i < amountRead; i++)
	    printf("%c", buffer[i]);
    delete [] buffer;

    delete openFile;		// close the Nachos file
    return;
}

//----------------------------------------------------------------------
// PerformanceTest
// 	Stress the Nachos file system by creating a large file, writing
//	it out a bit at a time, reading it back a bit at a time, and then
//	deleting the file.
//
//	Implemented as three separate routines:
//	  FileWrite -- write the file
//	  FileRead -- read the file
//	  PerformanceTest -- overall control, and print out performance #'s
//----------------------------------------------------------------------

#define FileName 	"TestFile"
#define Contents 	"1234567890"
#define ContentSize 	strlen(Contents)
#define FileSize 	((int)(ContentSize * 5000))

static void 
FileWrite()
{
    OpenFile *openFile;    
    int i, numBytes;

    printf("Sequential write of %d byte file, in %d byte chunks\n", 
	FileSize, ContentSize);
    if (!fileSystem->Create(FileName, 0)) {
      printf("Perf test: can't create %s\n", FileName);
      return;
    }
    openFile = fileSystem->Open(FileName);
    if (openFile == NULL) {
	printf("Perf test: unable to open %s\n", FileName);
	return;
    }
    for (i = 0; i < FileSize; i += ContentSize) {
        numBytes = openFile->Write(Contents, ContentSize);
	if (numBytes < 10) {
	    printf("Perf test: unable to write %s\n", FileName);
	    delete openFile;
	    return;
	}
    }
    delete openFile;	// close file
}

static void 
FileRead()
{
    OpenFile *openFile;    
    char *buffer = new char[ContentSize];
    int i, numBytes;

    printf("Sequential read of %d byte file, in %d byte chunks\n", 
	FileSize, ContentSize);

    if ((openFile = fileSystem->Open(FileName)) == NULL) {
	printf("Perf test: unable to open file %s\n", FileName);
	delete [] buffer;
	return;
    }
    for (i = 0; i < FileSize; i += ContentSize) {
        numBytes = openFile->Read(buffer, ContentSize);
	if ((numBytes < 10) || strncmp(buffer, Contents, ContentSize)) {
	    printf("Perf test: unable to read %s\n", FileName);
	    delete openFile;
	    delete [] buffer;
	    return;
	}
    }
    delete [] buffer;
    delete openFile;	// close file
}

void
PerformanceTest()
{
    printf("Starting file system performance test:\n");
    stats->Print();
    FileWrite();
    FileRead();
    if (!fileSystem->Remove(FileName)) {
      printf("Perf test: unable to remove %s\n", FileName);
      return;
    }
    stats->Print();
}




