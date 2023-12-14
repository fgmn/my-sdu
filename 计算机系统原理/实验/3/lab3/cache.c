#include <stdlib.h>
#include "cache.h"
#include "shell.h"
extern int locked = -1;

/* locked 
-1: 正常执行，未发生阻塞
0: 阻塞完成，cache从主存中获取数据
>0: 正在阻塞*/

cache INSTRUCTION_CACHE =
        {

                INSTRUCTION_CACHE_SIZE,
                INSTRUCTION_CACHE_SET_NUMBER,
                INSTRUCTION_CACHE_BLOCK_SIZE,
                INSTRUCTION_CACHE_WAYS,
                NULL
        };
cache DATA_CACHE=
        {
                DATA_CACHE_SIZE,
                DATA_CACHE_SET_NUMBER,
                DATA_CACHE_BLOCK_SIZE,
                DATA_CACHE_WAYS,
                NULL
        };

// void changeLRU(int i)
// {

// }

/*
LRU策略：
LRU为 0 - 7
初始化时，将LRU置为 7
这样，我们的替换方式可以统一为将 LRU == 7 的缓存块替换
*/


/***************************************************************/
/*                                                             */
/* Procedure: cache_read_32                                    */
/*                                                             */
/* Purpose: Read a 32-bit word from cache                      */
/*                                                             */
/***************************************************************/
uint32_t cache_read_32(uint32_t address)
{
        if (locked>0)
    {
        return;             //仍在阻塞
    }
    if (address >= MEM_DATA_START && address < MEM_DATA_START + MEM_DATA_SIZE)
    {
        // 解析地址
        uint8_t offset = address & 0x1f;           // 0 -- 31
        uint8_t SetsIndex = (address >> 5) & 0xff; // 0 -- 255
        uint32_t tag = (address >> 13) & 0x7ffff;
        BOOL FLAG = 0;

        // cache组首地址
        int SetStart = (int)SetsIndex * DATA_CACHE_WAYS;

        int i;
        for (i = 0; i < DATA_CACHE_WAYS; i++)
        {
            if (DATA_CACHE.header[SetStart + i].v == 1 && DATA_CACHE.header[SetStart + i].tag == tag)
            {
                FLAG = 1; // 命中

                //change LRU
                int j;
                for (j = 0; j < DATA_CACHE_WAYS; j++)
                {
                    if (DATA_CACHE.header[SetStart + j].frequency < DATA_CACHE.header[SetStart + i].frequency)
                    {
                        DATA_CACHE.header[SetStart + j].frequency++;
                    }
                }
                DATA_CACHE.header[SetStart + i].frequency = 0;

                return (DATA_CACHE.header[SetStart + i].data[offset + 3] << 24) |
                       (DATA_CACHE.header[SetStart + i].data[offset + 2] << 16) |
                       (DATA_CACHE.header[SetStart + i].data[offset + 1] << 8) |
                       (DATA_CACHE.header[SetStart + i].data[offset + 0] << 0);
            }
        }

        // Miss
        if (!FLAG)
        {
            if (locked==0){
                locked = -1;        //阻塞结束，继续运行
            }
            else if (locked == -1) {   // *
                locked = 50;        //说明应该开始阻塞
            }
            
            uint32_t value = mem_read_32(address);
            for (i = 0; i < DATA_CACHE_WAYS; i++)
            {
                // 根据最近最少用原则在组内寻找frequency == 7的替换块
                if (DATA_CACHE.header[SetStart + i].frequency == 7)
                {
                    // 写回改变过且不是空cache
                    if (DATA_CACHE.header[SetStart + i].changed == 1 && DATA_CACHE.header[SetStart + i].v == 1)
                    {
                        uint32_t source_address;
                        int j;
                        
                        for(j = 0; j < 8; j++){
                            source_address = ((DATA_CACHE.header[SetStart + i].tag << 13) & 0xFFFFE000) |
                                             ((SetsIndex << 5) &  0x1FE0)|
                                             ((j << 2) & 0x1C);
                            mem_write_32(source_address , 
                                        (DATA_CACHE.header[SetStart + i].data[j * 4 + 3] << 24) |
                                        (DATA_CACHE.header[SetStart + i].data[j * 4 + 2] << 16) |
                                        (DATA_CACHE.header[SetStart + i].data[j * 4 + 1] << 8) |
                                        (DATA_CACHE.header[SetStart + i].data[j * 4 + 0] << 0)
                                        );
                        }
                        DATA_CACHE.header[SetStart + i].changed = 0;
                        DATA_CACHE.header[SetStart + i].v = 0;
                    }
                    /* 完整读入读入应读入的cache行 */
                    uint32_t source_address;
                    int j;
                    for(j = 0; j < 8; j++){
                        source_address = (address & 0xFFFFFFE0) |
                                         ((j << 2) & 0x1C );
                        uint32_t result =mem_read_32(source_address);
                        DATA_CACHE.header[SetStart + i].data[j * 4 + 3] = (result >> 24) & 0xFF;
                        DATA_CACHE.header[SetStart + i].data[j * 4 + 2] = (result >> 16) & 0xFF;
                        DATA_CACHE.header[SetStart + i].data[j * 4 + 1] = (result >> 8) & 0xFF;
                        DATA_CACHE.header[SetStart + i].data[j * 4 + 0] = (result >> 0) & 0xFF;
                    }    

                    DATA_CACHE.header[SetStart + i].v = 1;
                    DATA_CACHE.header[SetStart + i].tag = tag;

                    //change LRU
                    for (j = 0; j < DATA_CACHE_WAYS; j++)
                    {
                        if (DATA_CACHE.header[SetStart + j].frequency != 7)
                        {
                            DATA_CACHE.header[SetStart + j].frequency++;
                        }
                    }
                    DATA_CACHE.header[SetStart + i].frequency = 0;      // 7 -> 0
                    break;
                }
            }

            return value;
        }
    }
    /*************************************************************************************/
    else if (address >= MEM_TEXT_START && address < MEM_TEXT_START + MEM_TEXT_SIZE)
    {
         // 解析地址
        uint8_t offset = address & 0x1f;           // 0 -- 31
        uint8_t SetsIndex = (address >> 5) & 0x3f; // 0 -- 63
        uint32_t tag = (address >> 11) & 0x1ffff;
        BOOL FLAG = 0;

        int SetStart = (int)SetsIndex * INSTRUCTION_CACHE_WAYS;

        int i;
        for (i = 0; i < INSTRUCTION_CACHE_WAYS; i++)
        {
            if (INSTRUCTION_CACHE.header[SetStart + i].v == 1 && INSTRUCTION_CACHE.header[SetStart + i].tag == tag)
            {
                FLAG = 1; // 命中

                //change LRU
                int j;
                for (j = 0; j < INSTRUCTION_CACHE_WAYS; j++)
                {
                    if (INSTRUCTION_CACHE.header[SetStart + j].frequency < INSTRUCTION_CACHE.header[SetStart + i].frequency)
                    {
                        INSTRUCTION_CACHE.header[SetStart + j].frequency++;
                    }
                }
                INSTRUCTION_CACHE.header[SetStart + i].frequency = 0;

                return (INSTRUCTION_CACHE.header[SetStart + i].data[offset + 3] << 24) |
                       (INSTRUCTION_CACHE.header[SetStart + i].data[offset + 2] << 16) |
                       (INSTRUCTION_CACHE.header[SetStart + i].data[offset + 1] << 8) |
                       (INSTRUCTION_CACHE.header[SetStart + i].data[offset + 0] << 0);
            }
        }

        // Miss
        if (!FLAG)
        {
            if (locked==0){
                locked = -1;        //阻塞结束，继续运行
            }else {
                locked = 50;        //说明应该开始阻塞
            }
            uint32_t value = mem_read_32(address);
            for (i = 0; i < INSTRUCTION_CACHE_WAYS; i++)
            {
                // frequency == 3: 空闲 或 最近最少用
                if (INSTRUCTION_CACHE.header[SetStart + i].frequency == 3)
                {
                    //改变过且不是空cache
                    if (INSTRUCTION_CACHE.header[SetStart + i].changed == 1 && INSTRUCTION_CACHE.header[SetStart + i].v == 1)
                    {
                        uint32_t source_address;
                        int j;
                        
                        for(j = 0; j < 8; j++){
                            source_address = ((INSTRUCTION_CACHE.header[SetStart + i].tag << 11) & 0xFFFFF800) |
                                             ((SetsIndex << 5) &  0x7E0)|
                                             ((j << 2) & 0x1C);
                            mem_write_32(source_address , 
                                        (INSTRUCTION_CACHE.header[SetStart + i].data[j * 4 + 3] << 24) |
                                        (INSTRUCTION_CACHE.header[SetStart + i].data[j * 4 + 2] << 16) |
                                        (INSTRUCTION_CACHE.header[SetStart + i].data[j * 4 + 1] << 8) |
                                        (INSTRUCTION_CACHE.header[SetStart + i].data[j * 4 + 0] << 0)
                                        );
                        }
                        INSTRUCTION_CACHE.header[SetStart + i].changed = 0;
                        INSTRUCTION_CACHE.header[SetStart + i].v = 0;
                    }
                    /* 完整读入读入应读入的cache行 */
                    uint32_t source_address;
                    int j;
                    for(j = 0; j < 8; j++){
                        source_address = (address & 0xFFFFFFE0) |
                                         ((j << 2) & 0x1C );
                        uint32_t result = mem_read_32(source_address);
                        INSTRUCTION_CACHE.header[SetStart + i].data[j * 4 + 3] = (result >> 24) & 0xFF;
                        INSTRUCTION_CACHE.header[SetStart + i].data[j * 4 + 2] = (result >> 16) & 0xFF;
                        INSTRUCTION_CACHE.header[SetStart + i].data[j * 4 + 1] = (result >> 8) & 0xFF;
                        INSTRUCTION_CACHE.header[SetStart + i].data[j * 4 + 0] = (result >> 0) & 0xFF;
                    }    

                    INSTRUCTION_CACHE.header[SetStart + i].v = 1;
                    INSTRUCTION_CACHE.header[SetStart + i].tag = tag;

                    //change LRU
                    for (j = 0; j < INSTRUCTION_CACHE_WAYS; j++)
                    {
                        if (INSTRUCTION_CACHE.header[SetStart + j].frequency != 3)
                        {
                            INSTRUCTION_CACHE.header[SetStart + j].frequency++;
                        }
                    }
                    INSTRUCTION_CACHE.header[SetStart + i].frequency = 0;      // 3 -> 0
                    break;
                }
            }

            return value;
        }       
    }
}

/***************************************************************/
/*                                                             */
/* Procedure: cache_write_32                                   */
/*                                                             */
/* Purpose: Write a 32-bit word to memory (and cache)          */
/*                                                             */
/***************************************************************/

void cache_write_32(uint32_t address, uint32_t value)
{
    if (address >= MEM_DATA_START && address < MEM_DATA_START + MEM_DATA_SIZE)
    {
        // 解析地址
        uint8_t offset = address & 0x1f;           // 0 -- 31
        uint8_t SetsIndex = (address >> 5) & 0xff; // 0 -- 255
        uint32_t tag = (address >> 13) & 0x7ffff;   //发现这个可以使用计算器的程序员模式计算
        BOOL FLAG = 0;

        int SetStart = (int)SetsIndex * DATA_CACHE_WAYS;

        int i;
        for (i = 0; i < DATA_CACHE_WAYS; i++)
        {
            if (DATA_CACHE.header[SetStart + i].v == 1 && DATA_CACHE.header[SetStart + i].tag == tag)
            {
                FLAG = 1; // 命中
                //change LRU
                int j;
                for (j = 0; j < DATA_CACHE_WAYS; j++)
                {
                    if (DATA_CACHE.header[SetStart + j].frequency < DATA_CACHE.header[SetStart + i].frequency)
                    {
                        DATA_CACHE.header[SetStart + j].frequency++;
                    }
                }
                DATA_CACHE.header[SetStart + i].frequency = 0;
                // 数据写入cache，这一整行都变成了已修改状态
                DATA_CACHE.header[SetStart + i].data[offset + 3] = (value >> 24) & 0xFF;
                DATA_CACHE.header[SetStart + i].data[offset + 2] = (value >> 16) & 0xFF;
                DATA_CACHE.header[SetStart + i].data[offset + 1] = (value >> 8) & 0xFF;
                DATA_CACHE.header[SetStart + i].data[offset + 0] = (value >> 0) & 0xFF;

                DATA_CACHE.header[SetStart + i].changed=1;
            }
        }

        // Miss
        if (!FLAG)
        {
            // uint32_t value = mem_read_32(address);
            for (i = 0; i < DATA_CACHE_WAYS; i++)
            {
                // frequency == 7: 空闲 或 最近最少用
                if (DATA_CACHE.header[SetStart + i].frequency == 7)
                {
                    //改变过且不是空cache
                    if (DATA_CACHE.header[SetStart + i].changed == 1 && DATA_CACHE.header[SetStart + i].v == 1)
                    {
                        uint32_t source_address;
                        int j;
                        
                        for(j = 0; j < 8; j++){
                            source_address = ((DATA_CACHE.header[SetStart + i].tag << 13) & 0xFFFFE000) |
                                             ((SetsIndex << 5) &  0x1FE0)|
                                             ((j << 2) & 0x1C);
                            mem_write_32(source_address , 
                                        (DATA_CACHE.header[SetStart + i].data[j * 4 + 3] << 24) |
                                        (DATA_CACHE.header[SetStart + i].data[j * 4 + 2] << 16) |
                                        (DATA_CACHE.header[SetStart + i].data[j * 4 + 1] << 8) |
                                        (DATA_CACHE.header[SetStart + i].data[j * 4 + 0] << 0)
                                        );
                        }
                        DATA_CACHE.header[SetStart + i].changed = 0;
                        DATA_CACHE.header[SetStart + i].v = 0;
                    }
                    //这个之前的是不合理的，应该一次性读所有的8个数据,或者写8个数据。

                    /* 完整读入读入应读入的cache行 */
                    uint32_t source_address;
                    int j;
                    for(j = 0; j < 8; j++){
                        source_address = (address & 0xFFFFFFE0) |
                                         ((j << 2) & 0x1C );
                        uint32_t result = mem_read_32(source_address);
                        DATA_CACHE.header[SetStart + i].data[j * 4 + 3] = (result >> 24) & 0xFF;
                        DATA_CACHE.header[SetStart + i].data[j * 4 + 2] = (result >> 16) & 0xFF;
                        DATA_CACHE.header[SetStart + i].data[j * 4 + 1] = (result >> 8) & 0xFF;
                        DATA_CACHE.header[SetStart + i].data[j * 4 + 0] = (result >> 0) & 0xFF;
                    }    
                    DATA_CACHE.header[SetStart + i].tag = tag;

                    DATA_CACHE.header[SetStart + i].v = 1;
                    //设置脏位，并处理替换，替换出现在：写不命中，读不命中，程序结束，停用cache
                    //change LRU
                    for (j = 0; j < DATA_CACHE_WAYS; j++)
                    {
                        if (DATA_CACHE.header[SetStart + j].frequency != 7)
                        {
                            DATA_CACHE.header[SetStart + j].frequency++;
                        }
                    }
                    DATA_CACHE.header[SetStart + i].frequency = 0;

                    break;
                }
            }

        }
    }
    /*************************************************************************************/
    else if (address >= MEM_TEXT_START && address < MEM_TEXT_START + MEM_TEXT_SIZE)
    {
         // 解析地址
        uint8_t offset = address & 0x1f;           // 0 -- 31
        uint8_t SetsIndex = (address >> 5) & 0x3f; // 0 -- 63
        uint32_t tag = (address >> 11) & 0x1ffff;
        BOOL FLAG = 0;

        int SetStart = (int)SetsIndex * INSTRUCTION_CACHE_WAYS;

        int i;
        for (i = 0; i < INSTRUCTION_CACHE_WAYS; i++)
        {
            if (INSTRUCTION_CACHE.header[SetStart + i].v == 1 && INSTRUCTION_CACHE.header[SetStart + i].tag == tag)
            {
                FLAG = 1; // 命中
                //change LRU
                int j;
                for (j = 0; j < INSTRUCTION_CACHE_WAYS; j++)
                {
                    if (INSTRUCTION_CACHE.header[SetStart + j].frequency < INSTRUCTION_CACHE.header[SetStart + i].frequency)
                    {
                        INSTRUCTION_CACHE.header[SetStart + j].frequency++;
                    }
                }
                INSTRUCTION_CACHE.header[SetStart + i].frequency = 0;
                //数据写入cache，这一整行都变成了已修改状态
                INSTRUCTION_CACHE.header[SetStart + i].data[offset + 3] = (value >> 24) & 0xFF;
                INSTRUCTION_CACHE.header[SetStart + i].data[offset + 2] = (value >> 16) & 0xFF;
                INSTRUCTION_CACHE.header[SetStart + i].data[offset + 1] = (value >> 8) & 0xFF;
                INSTRUCTION_CACHE.header[SetStart + i].data[offset + 0] = (value >> 0) & 0xFF;

                INSTRUCTION_CACHE.header[SetStart + i].changed=1;
            }
        }

        // Miss
        if (!FLAG)
        {
            // uint32_t value = mem_read_32(address);
            for (i = 0; i < INSTRUCTION_CACHE_WAYS; i++)
            {
                // frequency == 3: 空闲 或 最近最少用
                if (INSTRUCTION_CACHE.header[SetStart + i].frequency == 3)
                {
                    //改变过且不是空cache
                    if (INSTRUCTION_CACHE.header[SetStart + i].changed == 1 && INSTRUCTION_CACHE.header[SetStart + i].v == 1)
                    {
                        uint32_t source_address;
                        int j;
                        
                        for(j = 0; j < 8; j++){
                            source_address = ((INSTRUCTION_CACHE.header[SetStart + i].tag << 13) & 0xFFFFE000) |
                                             ((SetsIndex << 5) &  0x1FE0)|
                                             ((j << 2) & 0x1C);
                            mem_write_32(source_address , 
                                        (INSTRUCTION_CACHE.header[SetStart + i].data[j * 4 + 3] << 24) |
                                        (INSTRUCTION_CACHE.header[SetStart + i].data[j * 4 + 2] << 16) |
                                        (INSTRUCTION_CACHE.header[SetStart + i].data[j * 4 + 1] << 8) |
                                        (INSTRUCTION_CACHE.header[SetStart + i].data[j * 4 + 0] << 0)
                                        );
                        }
                        INSTRUCTION_CACHE.header[SetStart + i].changed = 0;
                        INSTRUCTION_CACHE.header[SetStart + i].v = 0;
                    }
                    //这个之前的是不合理的，应该一次性读所有的8个数据,或者写8个数据。

                    /* 完整读入读入应读入的cache行 */
                    uint32_t source_address;
                    int j;
                    for(j = 0; j < 8; j++){
                        source_address = (address & 0xFFFFFFE0) |
                                         ((j << 2) & 0x1C );
                        uint32_t result = mem_read_32(source_address);
                        INSTRUCTION_CACHE.header[SetStart + i].data[j * 4 + 3] = (result >> 24) & 0xFF;
                        INSTRUCTION_CACHE.header[SetStart + i].data[j * 4 + 2] = (result >> 16) & 0xFF;
                        INSTRUCTION_CACHE.header[SetStart + i].data[j * 4 + 1] = (result >> 8) & 0xFF;
                        INSTRUCTION_CACHE.header[SetStart + i].data[j * 4 + 0] = (result >> 0) & 0xFF;
                    } 
                                      
                    INSTRUCTION_CACHE.header[SetStart + i].v = 1;

                    INSTRUCTION_CACHE.header[SetStart + i].tag = tag;

                    //设置脏位，并处理替换，替换出现在：写不命中，读不命中，程序结束，停用cache
                    //change LRU
                    for (j = 0; j < INSTRUCTION_CACHE_WAYS; j++)
                    {
                        if (INSTRUCTION_CACHE.header[SetStart + j].frequency != 3)
                        {
                            INSTRUCTION_CACHE.header[SetStart + j].frequency++;
                        }
                    }
                    INSTRUCTION_CACHE.header[SetStart + i].frequency = 0;
                    break;
                }
            }

        }
    }
    return;
}


/***************************************************************/
/*                                                             */
/* Procedure : init_cache                                     */
/*                                                             */
/* Purpose   : Allocate and zero cache                      */
/*                                                             */
/***************************************************************/

void init_cache()
{
    // 初始化指令cache
    int LineOfins = INSTRUCTION_CACHE.setNumber * INSTRUCTION_CACHE.ways;
    int LineOfdata = DATA_CACHE.setNumber * DATA_CACHE.ways;

    INSTRUCTION_CACHE.header = malloc(LineOfins * sizeof(cacheBlock));
    int i;
    for (i = 0; i < LineOfins; i++)
    {
        INSTRUCTION_CACHE.header[i].frequency = 3;
        INSTRUCTION_CACHE.header[i].v = 0;
        INSTRUCTION_CACHE.header[i].changed = 0;

        memset(INSTRUCTION_CACHE.header[i].data, 0, 32); //INSTRUCTION_CACHE.header[i].data +
    }
    // 初始化数据cache
    DATA_CACHE.header = malloc(LineOfdata * sizeof(cacheBlock));
    for (i = 0; i < LineOfdata; i++)
    {
        DATA_CACHE.header[i].frequency = 7;
        DATA_CACHE.header[i].v = 0;
        DATA_CACHE.header[i].changed = 0;

        memset(DATA_CACHE.header[i].data, 0, 32); //DATA_CACHE.header[i].data +
    }
}

void delete_cache()
{
    int LineOfins = INSTRUCTION_CACHE.setNumber * INSTRUCTION_CACHE.ways;
    int LineOfdata = DATA_CACHE.setNumber * DATA_CACHE.ways;
    int SetsIndex;
    int i,j;
    for (i = 0; i < LineOfdata; i++)
    {
        if(DATA_CACHE.header[i].v==1 && DATA_CACHE.header[i].changed==1){
            uint32_t source_address;
            SetsIndex = i / DATA_CACHE_WAYS;
            for (j = 0; j < 8; j++)//遍历块
            {
                source_address = ((DATA_CACHE.header[i].tag << 13) & 0xFFFFE000) |
                                ((SetsIndex << 5) & 0x1FE0) |
                                ((j << 2) & 0x1C);
                mem_write_32(source_address,
                            (DATA_CACHE.header[i].data[j * 4 + 3] << 24) |
                            (DATA_CACHE.header[i].data[j * 4 + 2] << 16) |
                            (DATA_CACHE.header[i].data[j * 4 + 1] << 8) |
                            (DATA_CACHE.header[i].data[j * 4 + 0] << 0));
            }
            DATA_CACHE.header[i].changed = 0;
            DATA_CACHE.header[i].v = 0;
        }
    }
    for (i = 0; i < LineOfins; i++)
    {
        if(INSTRUCTION_CACHE.header[i].v==1 && INSTRUCTION_CACHE.header[i].changed==1){
            uint32_t source_address;
            SetsIndex = i / INSTRUCTION_CACHE_WAYS;
            for (j = 0; j < 8; j++)//遍历块
            {
                source_address = ((INSTRUCTION_CACHE.header[i].tag << 13) & 0xFFFFE000) |
                                ((SetsIndex << 5) & 0x1FE0) |
                                ((j << 2) & 0x1C);
                mem_write_32(source_address,
                            (INSTRUCTION_CACHE.header[i].data[j * 4 + 3] << 24) |
                            (INSTRUCTION_CACHE.header[i].data[j * 4 + 2] << 16) |
                            (INSTRUCTION_CACHE.header[i].data[j * 4 + 1] << 8) |
                            (INSTRUCTION_CACHE.header[i].data[j * 4 + 0] << 0));
            }
            INSTRUCTION_CACHE.header[i].changed = 0;
            INSTRUCTION_CACHE.header[i].v = 0;
        }
    }
}