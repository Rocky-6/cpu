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
void cflag(Cpub *cpub, Uword operand_a, Uword operand_b, Uword x) { // cpub:演算結果 operand_a:acc or ixの判別 operand_b:operand_b x:演算前のacc or ixの中身
	Uword b = operand_b >> 7;
	Uword xshift = x >> 7;
	Uword xb = (x + operand_b) >> 7;

	if(operand_a == 0) {
		cpub->nf = cpub->acc >> 7;
	}
	else {
		cpub->nf = cpub->ix >> 7;
	}

	if(xshift == 0x01 && b == 0x01) {
		cpub->cf = 1;
		if(xb == 0x00) cpub->vf = 1;
	}
	else if(xshift == 0x01 && b == 0x00) {
		if(xb == 0x00) cpub->cf = 1;
		cpub->vf = 0;
	}
	else if(xshift == 0x00 && b == 0x01) {
		if(xb == 0x00) cpub->cf = 1;
		cpub->vf = 0;
	}
	else if(xshift == 0x00 && b == 0x00) {
		cpub->cf = 0;
		if(xb == 0x01) cpub->vf = 1;
	}

	if(operand_a == 0){
		if(cpub->acc == 0x00) cpub->zf = 1;
		else cpub->zf = 0;
	}
	else {
		if(cpub->ix == 0x00) cpub->zf = 1;
		else cpub->zf = 0;
	}
}

void flag(Cpub *cpub, Uword operand_a, Uword operand_b, Uword x) {
	Uword b = operand_b >> 7;
	Uword xshift = x >> 7;
	Uword xb = (x + operand_b) >> 7;

	if(operand_a == 0) {
		cpub->nf = cpub->acc >> 7;
	}
	else {
		cpub->nf = cpub->ix >> 7;
	}

	if(xshift == 0x01 && b == 0x01) {
		//cpub->cf = 1;
		if(xb == 0x00) cpub->vf = 1;
	}
	else if(xshift == 0x01 && b == 0x00) {
		//if(xb == 0x00) cpub->cf = 1;
		cpub->vf = 0;
	}
	else if(xshift == 0x00 && b == 0x01) {
		//if(xb == 0x00) cpub->cf = 1;
		cpub->vf = 0;
	}
	else if(xshift == 0x00 && b == 0x00) {
		//cpub->cf = 0;
		if(xb == 0x01) cpub->vf = 1;
	}

	if(operand_a == 0){
		if(cpub->acc == 0x00) cpub->zf = 1;
		else cpub->zf = 0;
	}
	else {
		if(cpub->ix == 0x00) cpub->zf = 1;
		else cpub->zf = 0;
	}
}



int
step(Cpub *cpub)
{
 	Uword operand_a, operand_b;
	Uword ir = cpub->pc;
	Uword x;

	if (0x70 == (cpub->mem[cpub->pc] & 0xf0)) // ST命令を実行
{
	int address = 0;
	if(0x04 == (cpub->mem[cpub->pc] & 0x0f) || 0x05 == (cpub->mem[cpub->pc] & 0x0f)) // オペランドAがACC
	{
		if (0x04 == (cpub->mem[cpub->pc] & 0x0f)) // オペランドBがプログラム領域
		{
			cpub->pc++; // PCをインクリメント
			address = cpub->mem[cpub->pc]; // ACCに入っているデータの移動先アドレス
		}
		else if(0x05 == (cpub->mem[cpub->pc] & 0x0f)) // オペランドBがデータ領域
		{
			cpub->pc++;
			address = IMEMORY_SIZE + (int)cpub->mem[cpub->pc];
		}
		cpub->mem[address] = cpub->acc; //ACCに入っているデータを移動させる
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
	cpub->pc++;
	return RUN_STEP;
}

	switch (cpub->mem[ir] & 0x0f) // オペランドAを決定
	{
		case 0x00:
		case 0x01:
		case 0x02:
		case 0x04:
		case 0x05:
		case 0x06:
		case 0x07:
			operand_a = 0; // オペランドAがACC
			break;

		case 0x08:
		case 0x09:
		case 0x0a:
		case 0x0c:
		case 0x0d:
		case 0x0e:
		case 0x0f:
			operand_a = 1; // オペランドAがIX
			break;

		default:
			break;
	}

	switch (cpub->mem[ir] & 0x0f) {
		case 0x00:
		case 0x08:
			operand_b = cpub->acc; // オペランドBがACC
			break;

		case 0x01:
		case 0x09:
			operand_b = cpub->ix; // オペランドBがIX
			break;

		case 0x02:
		case 0x0a:
			ir++;
			operand_b = cpub->mem[ir]; // オペランドBが即値アドレス
			break;

		case 0x04:
		case 0x0c:
			ir++;
			operand_b = cpub->mem[cpub->mem[ir]]; // オペランドBが絶対アドレス(プログラム領域)
			break;

		case 0x05:
		case 0x0d:
			ir++;
			operand_b = cpub->mem[IMEMORY_SIZE + cpub->mem[ir]]; // オペランドBが絶対アドレス(データ領域)
			break;

		case 0x06:
		case 0x0e:
			ir++;
			operand_b = cpub->mem[cpub->ix + cpub->mem[ir]]; // オペランドBがIX修飾アドレス(プログラム領域)
			break;

		case 0x07:
		case 0x0f:
			ir++;
			operand_b = cpub->mem[IMEMORY_SIZE + cpub->ix + cpub->mem[ir]]; // オペランドBがIX修飾アドレス(データ領域)
			break;

		default:
			break;
	}

	switch (cpub->mem[cpub->pc]) {
		case 0x31:
			cpub->pc++;
			if(cpub->zf == 0) cpub->pc = cpub->mem[cpub->pc];
			else cpub->pc++;
			return RUN_STEP;
			break;

		case 0x0f:
			cpub->pc++;
			return RUN_HALT;
	}



	switch (cpub->mem[cpub->pc] & 0xf0) {
		case 0x60: // LD
			if (operand_a == 0)
			{
				cpub->acc = operand_b;
			}
			else
			{
				cpub->ix = operand_b;
			}
			break;

		case 0xb0: // ADD
			if(operand_a == 0)
			{
				//flag(cpub, cpub->acc, operand_b);
				x = cpub->acc;
				cpub->acc = cpub->acc + operand_b;
				flag(cpub, operand_a, operand_b, x); // cpub:演算結果 operand_a:acc or ixの判別 operand_b:operand_b x:演算前のacc or ixの中身
			}
			else
			{
				x = cpub->ix;
				cpub->ix = cpub->ix + operand_b;
				flag(cpub, operand_a, operand_b, x);
			}
			break;

		case 0x90: // ADC
			if(operand_a == 0)
			{
				x = cpub->acc;
				cpub->acc = cpub->acc + operand_b + cpub->cf;
				cflag(cpub, operand_a, operand_b, x);
			}
			else
			{
				x = cpub->ix;
				cpub->ix = cpub->ix + operand_b + cpub->cf;
				cflag(cpub, operand_a, operand_b, x);
			}
			break;

		case 0xa0: // SUB
			if(operand_a == 0)
			{
				x = cpub->acc;
				cpub->acc = cpub->acc - operand_b;
				flag(cpub, operand_a, operand_b | 0x80, x);
			}
			else
			{
				x = cpub->ix;
				cpub->ix = cpub->ix - operand_b;
				flag(cpub, operand_a, operand_b | 0x80, x);
			}
			break;


		case 0x80: // SBC
			if(operand_a == 0)
			{
				x = cpub->acc;
				cpub->acc = cpub->acc - operand_b - cpub->cf;
				cflag(cpub, operand_a, operand_b | 0x80, x);
			}
			else
			{
				x = cpub->ix;
				cpub->ix = cpub->ix - operand_b - cpub->cf;
				cflag(cpub, operand_a, operand_b | 0x80, x);
			}
			break;


		case 0xf0: // CMP
			if(operand_a == 0)
			{
				x = cpub->acc;
				flag(cpub, operand_a, operand_b, x);
			}
			else
			{
				x = cpub->ix;
				flag(cpub, operand_a, operand_b, x);
			}
			break;


		case 0xe0: // AND
			if(operand_a == 0)
			{
				x = cpub->acc;
				flag(cpub, operand_a, operand_b, x);
				cpub->acc = cpub->acc & operand_b;
			}
			else
			{
				x = cpub->ix;
				flag(cpub, operand_a, operand_b, x);
				cpub->ix = cpub->ix & operand_b;
			}
			cpub->vf = 0;
			break;

		case 0xd0: // OR
			if(operand_a == 0)
			{
				x = cpub->acc;
				flag(cpub, operand_a, operand_b, x);
				cpub->acc = cpub->acc || operand_b;
			}
			else
			{
				x = cpub->ix;
				flag(cpub, operand_a, operand_b, x);
				cpub->ix = cpub->ix || operand_b;
			}
			cpub->vf = 0;
			break;

		case 0xc0: // EOR
			if(operand_a == 0)
			{
				x = cpub->acc;
				flag(cpub, operand_a, operand_b, x);
				cpub->acc = cpub->acc ^ operand_b;
			}
			else
			{
				x = cpub->ix;
				flag(cpub, operand_a, operand_b, x);
				cpub->ix = cpub->ix ^ operand_b;
			}
			cpub->vf = 0;
			break;

			default:
			return RUN_HALT;
	}
	cpub->pc = ir;
	cpub->pc++;
	return RUN_STEP;
}
