#ifndef VM_INSTS_H
#define VM_INSTS_H

#include "binary.h"

enum {
	INST_NOP    = b_00000000,
	INST_JMP    = b_00000001,
	INST_JNZ    = b_00000010,
	INST_JZ     = b_00000011,
	INST_ADD    = b_00000100,
	INST_SUB    = b_00000101,
	INST_MUL    = b_00000110,
	INST_IECALL = b_00000111,
	INST_DIV    = b_00001000,
	INST_IDIV   = b_00001001,
	INST_MOD    = b_00001010,
	INST_IMOD   = b_00001011,
	INST_DUP    = b_00001100,
	INST_OVER   = b_00001101,
	INST_SWAP   = b_00001110,
	INST_EQU    = b_00001111,
	INST_NEQU   = b_00010000,
	INST_GTH    = b_00010001,
	INST_LTH    = b_00010010,
	INST_IGTH   = b_00010011,
	INST_ILTH   = b_00010100,
	INST_AND    = b_00010101,
	INST_OR     = b_00010110,
	INST_XOR    = b_00010111,
	INST_NOT    = b_00011000,
	INST_WRB    = b_00011001,
	INST_WRH    = b_00011010,
	INST_WRW    = b_00011011,
	INST_RDB    = b_00011100,
	INST_RDH    = b_00011101,
	INST_RDW    = b_00011110,
	INST_CALL   = b_00011111,
	INST_ECALL  = b_00100000,
	INST_RET    = b_00100001,
	INST_SHL    = b_00100010,
	INST_SHR    = b_00100011,
	INST_POP    = b_00100100,
	INST_HALT   = b_00100101,
	INST_RDSP   = b_00100110,
	INST_WDSP   = b_00100111,
	INST_RRSP   = b_00101000,
	INST_WRSP   = b_00101001
};

#endif
