// // openfile.cc 
// //	Routines to manage an open Nachos file.  As in UNIX, a
// //	file must be open before we can read or write to it.
// //	Once we're all done, we can close it (in Nachos, by deleting
// //	the OpenFile data structure).
// //
// //	Also as in UNIX, for convenience, we keep the file header in
// //	memory while the file is open.
// //
// // Copyright (c) 1992-1993 The Regents of the University of California.
// // All rights reserved.  See copyright.h for copyright notice and limitation 
// // of liability and disclaimer of warranty provisions.

// #include "copyright.h"
// #include "filehdr.h"
// #include "openfile.h"
// #include "system.h"

// //----------------------------------------------------------------------
// // OpenFile::OpenFile
// // 	Open a Nachos file for reading and writing.  Bring the file header
// //	into memory while the file is open.
// //
// //	"sector" -- the location on disk of the file header for this file
// //----------------------------------------------------------------------

// OpenFile::OpenFile(int sector)
// { 
//     hdr = new FileHeader;
//     hdr->FetchFrom(sector);
//     seekPosition = 0;
//     hdrSector = sector; // 打开文件的文件头所在扇区
// }

// //----------------------------------------------------------------------
// // OpenFile::~OpenFile
// // 	Close a Nachos file, de-allocating any in-memory data structures.
// //----------------------------------------------------------------------

// OpenFile::~OpenFile()
// {
//     delete hdr;
// }

// //----------------------------------------------------------------------
// // OpenFile::Seek
// // 	Change the current location within the open file -- the point at
// //	which the next Read or Write will start from.
// //
// //	"position" -- the location within the file for the next Read/Write
// //----------------------------------------------------------------------

// void
// OpenFile::Seek(int position)
// {
//     seekPosition = position;
// }	

// //----------------------------------------------------------------------
// // OpenFile::Read/Write
// // 	Read/write a portion of a file, starting from seekPosition.
// //	Return the number of bytes actually written or read, and as a
// //	side effect, increment the current position within the file.
// //
// //	Implemented using the more primitive ReadAt/WriteAt.
// //
// //	"into" -- the buffer to contain the data to be read from disk 
// //	"from" -- the buffer containing the data to be written to disk 
// //	"numBytes" -- the number of bytes to transfer
// //----------------------------------------------------------------------

// int
// OpenFile::Read(char *into, int numBytes)
// {
//    int result = ReadAt(into, numBytes, seekPosition);
//    seekPosition += result;
//    return result;
// }

// int
// OpenFile::Write(char *into, int numBytes)
// {
//    int result = WriteAt(into, numBytes, seekPosition);
//    seekPosition += result;
//    return result;
// }

// //----------------------------------------------------------------------
// // OpenFile::ReadAt/WriteAt
// // 	Read/write a portion of a file, starting at "position".
// //	Return the number of bytes actually written or read, but has
// //	no side effects (except that Write modifies the file, of course).
// //
// //	There is no guarantee the request starts or ends on an even disk sector
// //	boundary; however the disk only knows how to read/write a whole disk
// //	sector at a time.  Thus:
// //
// //	For ReadAt:
// //	   We read in all of the full or partial sectors that are part of the
// //	   request, but we only copy the part we are interested in.
// //	For WriteAt:
// //	   We must first read in any sectors that will be partially written,
// //	   so that we don't overwrite the unmodified portion.  We then copy
// //	   in the data that will be modified, and write back all the full
// //	   or partial sectors that are part of the request.
// //
// //	"into" -- the buffer to contain the data to be read from disk 
// //	"from" -- the buffer containing the data to be written to disk 
// //	"numBytes" -- the number of bytes to transfer
// //	"position" -- the offset within the file of the first byte to be
// //			read/written
// //----------------------------------------------------------------------

// int
// OpenFile::ReadAt(char *into, int numBytes, int position)
// {
//     int fileLength = hdr->FileLength();
//     int i, firstSector, lastSector, numSectors;
//     char *buf;

//     if ((numBytes <= 0) || (position >= fileLength))
//     	return 0; 				// check request
//     if ((position + numBytes) > fileLength)		
// 	numBytes = fileLength - position;
//     DEBUG('f', "Reading %d bytes at %d, from file of length %d.\n", 	
// 			numBytes, position, fileLength);

//     // 除法，向下取整数
//     firstSector = divRoundDown(position, SectorSize);
//     lastSector = divRoundDown(position + numBytes - 1, SectorSize);
//     numSectors = 1 + lastSector - firstSector;

//     // read in all the full and partial sectors that we need
//     buf = new char[numSectors * SectorSize];
//     for (i = firstSector; i <= lastSector; i++)	    // 将字节偏移量映射到实际扇区
//         synchDisk->ReadSector(hdr->ByteToSector(i * SectorSize), 
// 					&buf[(i - firstSector) * SectorSize]);

//     // 并不是所有首尾磁盘块的内容都是需要的
//     // copy the part we want
//     bcopy(&buf[position - (firstSector * SectorSize)], into, numBytes);
//     delete [] buf;
//     return numBytes;
// }

// int
// OpenFile::WriteAt(char *from, int numBytes, int position)
// {   // 拓展为可以从文件任何位置开始写入数据
//     int fileLength = hdr->FileLength();
//     int i, firstSector, lastSector, numSectors;
//     bool firstAligned, lastAligned;
//     char *buf;

//     // 以下两个约束使得文件无法进行拓展
//     // if ((numBytes <= 0) || (position >= fileLength))
// 	// return 0;				// check request
//     // if ((position + numBytes) > fileLength)
// 	// numBytes = fileLength - position;

//     // 约束1
//     if (numBytes <= 0 || (position > fileLength)) {
//         return -1;
//     }
//     // 约束2
//     // printf("u:%d + %d > %d\n", position, numBytes, fileLength);
//     if ((position + numBytes) > fileLength) { // 证明文件需要进行拓展
//         int incrementBytes = (position + numBytes) - fileLength;    // 拓展的字节数
//         BitMap *freeBitMap = fileSystem->getBitMap();   // 未定义标识符！

//         // printf("u:check bitmap\n");
//         // freeBitMap->Print();

//         bool hdrRet;
//         // 输入参数：位示图，当前文件长度，增加字节数
//         hdrRet = hdr->Allocate(freeBitMap, fileLength, incrementBytes);
//         if (!hdrRet) {
//             // printf("constrait 2\n");
//             return -1;
//         }
//         fileSystem->setBitMap(freeBitMap);
//     }

//     DEBUG('f', "Writing %d bytes at %d, from file of length %d.\n", 	
// 			numBytes, position, fileLength);

//     firstSector = divRoundDown(position, SectorSize);
//     lastSector = divRoundDown(position + numBytes - 1, SectorSize);
//     numSectors = 1 + lastSector - firstSector;

//     buf = new char[numSectors * SectorSize];

//     firstAligned = (bool)(position == (firstSector * SectorSize));
//     lastAligned = (bool)((position + numBytes) == ((lastSector + 1) * SectorSize));

//     // 扇区存在部分修改，则先读出修改后再写入
//     // 优化：全部修改则无需读出
// // read in first and last sector, if they are to be partially modified
//     if (!firstAligned)
//         ReadAt(buf, SectorSize, firstSector * SectorSize);	
//         // 将该条件表达式取反：
//         // lastAligned || ((firstSector == lastSector) && !firstAligned)
//         // 不用将该扇区读出的条件：
//         // 写操作恰好覆盖最后一个磁盘块 或者 修改操作仅覆盖一个块，且该块因为前面没有对齐已经读出
//     if (!lastAligned && ((firstSector != lastSector) || firstAligned))
//         ReadAt(&buf[(lastSector - firstSector) * SectorSize], 
// 				SectorSize, lastSector * SectorSize);	

// // copy in the bytes we want to change 
//     bcopy(from, &buf[position - (firstSector * SectorSize)], numBytes);

// // write modified sectors back    
//     for (i = firstSector; i <= lastSector; i++)	
//         synchDisk->WriteSector(hdr->ByteToSector(i * SectorSize), 
// 					&buf[(i - firstSector) * SectorSize]);
//     delete [] buf;
//     return numBytes;
// }

// //----------------------------------------------------------------------
// // OpenFile::Length
// // 	Return the number of bytes in the file.
// //----------------------------------------------------------------------

// int
// OpenFile::Length() 
// { 
//     return hdr->FileLength(); 
// }

// void 
// OpenFile::WriteBack() {
//    hdr-> WriteBack(hdrSector);
//    // 拓展之后将文件头更新
// }

