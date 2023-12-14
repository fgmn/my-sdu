/*
 * @Description: 
 * @Author: Wangzhe
 * @Date: 2020-11-06 18:47:46
 * @LastEditors: Wangzhe
 * @LastEditTime: 2020-11-06 19:14:52
 * @FilePath: \lab3\src\shell.h
 */
/***************************************************************/
/*                                                             */
/*   MIPS-32 Instruction Level Simulator                       */
/*                                                             */
/*   Computer Architecture - Professor Onur Mutlu              */
/*                                                             */
/***************************************************************/

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
/*          DO NOT MODIFY THIS FILE!                            */
/*          You should only change sim.c!                       */
/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

#ifndef _SIM_SHELL_H_
#define _SIM_SHELL_H_ 

#include <stdint.h>

#define BOOL int
#define FALSE 0
#define TRUE  1

extern int RUN_BIT;	/* run bit */

/* only the cache touches these functions */
uint32_t mem_read_32(uint32_t address);
void     mem_write_32(uint32_t address, uint32_t value);

/* statistics */
extern uint32_t stat_cycles, stat_inst_retire, stat_inst_fetch, stat_squash;

#endif
