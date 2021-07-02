/*
 *	Project-based Learning II (CPU)
 *
 *	Program:	instruction set simulator of the Educational CPU Board
 *	File Name:	cpuboard.c
 *	Descrioption:	simulation(emulation) of an instruction
 */

#include	"cpuboard.h"
#include <stdio.h>


/*=============================================================================
 *   Simulation of a Single Instruction
 *===========================================================================*/
int
step(Cpub *cpub)
{
	//printf("%x", (cpub->mem[cpub->pc]&0xf0));
	if (0x70 == (cpub->mem[cpub->pc] & 0xf0)) // ST命令
	{
		int address;
		if(0x04 == (cpub->mem[cpub->pc] & 0x0f) || 0x05 == (cpub->mem[cpub->pc] & 0x0f)) // オペランドAがACC
		{
			if (0x04 == (cpub->mem[cpub->pc] & 0x0f)) // オペランドBがプログラム領域
			{
				cpub->pc++;
				address = cpub->mem[cpub->pc];
			}
			else if (0x05 == (cpub->mem[cpub->pc] & 0x0f)) // オペランドBがデータ領域
			{
				cpub->pc++;
				address = IMEMORY_SIZE + (int)cpub->mem[cpub->pc];
			}
			cpub->mem[address] = cpub->acc;
		}
		else if(0x0c == (cpub->mem[cpub->pc] & 0x0f) || 0x0d == (cpub->mem[cpub->pc] & 0x0f)) // オペランドAがIX
		{
			if (0x0c == (cpub->mem[cpub->pc] & 0x0f)) // オペランドBがプログラム領域
			{
				cpub->pc++;
				address = cpub->mem[cpub->pc];
			}
			else if (0x0d == (cpub->mem[cpub->pc] & 0x0f)) // オペランドBがデータ領域
			{
				cpub->pc++;
				address = IMEMORY_SIZE + (int)cpub->mem[cpub->pc];
			}
			cpub->mem[address] = cpub->ix; 
		}
		return RUN_STEP;
	}
	return RUN_HALT;
}


