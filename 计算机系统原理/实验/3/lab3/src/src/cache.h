#ifndef _CACHE_H_
#define _CACHE_H_

//v tag data frequency按字（4Byte）寻址 按使用频率替换
//存储使用的频率
//cache大小
#define INSTRUCTION_CACHE_SIZE 0x2000
#define DATA_CACHE_SIZE 0x10000
//（路数）每组的cache行数
#define INSTRUCTION_CACHE_WAYS 4
#define DATA_CACHE_WAYS 8
//cache块大小
#define INSTRUCTION_CACHE_BLOCK_SIZE 32
#define DATA_CACHE_BLOCK_SIZE 32
//cache组数
#define INSTRUCTION_CACHE_SET_NUMBER INSTRUCTION_CACHE_SIZE/INSTRUCTION_CACHE_WAYS/INSTRUCTION_CACHE_BLOCK_SIZE
#define DATA_CACHE_SET_NUMBER DATA_CACHE_SIZE/DATA_CACHE_WAYS/DATA_CACHE_BLOCK_SIZE

// 主存空间
#define MEM_DATA_START  0x10000000
#define MEM_DATA_SIZE   0x00100000
#define MEM_TEXT_START  0x00400000
#define MEM_TEXT_SIZE   0x00100000

#include "shell.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef struct
{
    uint8_t frequency; //根据每组的cache行数确定位数，每组n行，需要2^n位
    BOOL v;            //1bit
    BOOL changed;
    uint32_t tag;      //内存标志
    // uint32_t data[8];    //不定长的数组，取决于总的cache块的大小
    uint8_t data[32];    //不定长的数组，取决于总的cache块的大小
} cacheBlock;

typedef struct
{

    int Size;           //总的大小 8kB 64kB
    int setNumber;      //多少组 Size/blockSize/ways 64 256
    int blockSize;      //cache行的大小 32byte
    int ways;           //每组几个 取指：4 访存：8
    cacheBlock *header; //cacheBlock数组
} cache;

/*解析内存
    PC [10:0]       PC[10:5]组的索引  PC[4:2]块内偏移 PC[1:0]一个字
    PC [31:11]      PC[31:11] tag

    Address [12:0]  Address[12:5] Address[4:2]块内偏移 Address[1:0]一个字
    Address [31:13] tag
*/

#define CACHE_NREGIONS (sizeof(CACHE_REGIONS)/sizeof(cache))


// cache instructionCache = {
//     (cacheBlock *)malloc(INSTRUCTION_CACHE_SET_NUMBER * sizeof(cacheBlock)),
//     INSTRUCTION_CACHE_SIZE,
//     INSTRUCTION_CACHE_SET_NUMBER,
//     INSTRUCTION_CACHE_BLOCK_SIZE,
//     INSTRUCTION_CACHE_WAYS};
// cache dataCache = {
//     (cacheBlock *)malloc(DATA_CACHE_SET_NUMBER * sizeof(cacheBlock)),
//     DATA_CACHE_SIZE,
//     DATA_CACHE_SET_NUMBER,
//     DATA_CACHE_BLOCK_SIZE,
//     DATA_CACHE_WAYS};

uint32_t cache_read_32(uint32_t address);
void cache_write_32(uint32_t address, uint32_t value);
void init_cache();
void delete_cache();
#endif