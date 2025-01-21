#include <stdio.h>
#include <assert.h>
#include "safe.h"
#include "calvm.h"
#include "insts.h"

static uint32_t Read32(uint8_t* ptr) {
	return
		((uint32_t) ptr[0]) |
		(((uint32_t) ptr[1]) << 8) |
		(((uint32_t) ptr[2]) << 16) |
		(((uint32_t) ptr[3]) << 24);
}

void CalVM_Init(CalVM* vm, uint8_t* program, uint32_t ramSize) {
	vm->rom     = program + 8;
	vm->romSize = Read32(program);
	vm->ram     = SafeMalloc(ramSize);
	vm->ramSize = ramSize;
	vm->ip      = 0;
	vm->halted  = false;
	vm->dsp     = ramSize - 512 * 4;
	vm->rsp     = vm->dsp;

	// read data
	memcpy(vm->ram, program + Read32(program) + 8, Read32(program + 4));
}

void CalVM_Free(CalVM* vm) {
	free(vm->ram);
}

// memory
uint16_t CalVM_Read16(CalVM* vm, uint32_t addr) {
	return ((uint16_t) vm->ram[addr]) | (((uint16_t) vm->ram[addr + 1]) << 8);
}

uint32_t CalVM_Read32(CalVM* vm, uint32_t addr) {
	return Read32(&vm->ram[addr]);
}

void CalVM_Write16(CalVM* vm, uint32_t addr, uint16_t value) {
	vm->ram[addr]     = (uint8_t) (value & 0xFF);
	vm->ram[addr + 1] = (uint8_t) ((value & 0xFF00) >> 8);
}

void CalVM_Write32(CalVM* vm, uint32_t addr, uint32_t value) {
	vm->ram[addr]     = (uint8_t) (value & 0xFF);
	vm->ram[addr + 1] = (uint8_t) ((value & 0xFF00) >> 8);
	vm->ram[addr + 2] = (uint8_t) ((value & 0xFF0000) >> 16);
	vm->ram[addr + 3] = (uint8_t) ((value & 0xFF000000) >> 24);
}

void CalVM_Push(CalVM* vm, uint32_t value) {
	CalVM_Write32(vm, vm->dsp, value);
	vm->dsp += 4;
}

uint32_t CalVM_Pop(CalVM* vm) {
	vm->dsp -= 4;
	return CalVM_Read32(vm, vm->dsp);
}

void CalVM_PushReturn(CalVM* vm, uint32_t value) {
	vm->rsp -= 4;
	CalVM_Write32(vm, vm->rsp, value);
}

uint32_t CalVM_PopReturn(CalVM* vm) {
	uint32_t ret = CalVM_Read32(vm, vm->rsp);
	vm->rsp += 4;
	return ret;
}

static uint8_t NextByte(CalVM* vm) {
	uint8_t ret = vm->ram[vm->ip];
	++ vm->ip;

	return ret;
}

static uint32_t NextWord(CalVM* vm) {
	uint32_t ret;
	ret |= (uint32_t) NextByte(vm);
	ret |= ((uint32_t) NextByte(vm)) << 8;
	ret |= ((uint32_t) NextByte(vm)) << 16;
	ret |= ((uint32_t) NextByte(vm)) << 24;
	return ret;
}

#define VOID_UNARY(BLOCK) do { \
	uint32_t a = CalVM_Pop(vm); \
	BLOCK; \
} while(0)

#define VOID_BIN(BLOCK) do { \
	uint32_t b = CalVM_Pop(vm); \
	uint32_t a = CalVM_Pop(vm); \
	BLOCK; \
} while(0)

#define OP_BIN(BLOCK) do { \
	uint32_t b = CalVM_Pop(vm); \
	uint32_t a = CalVM_Pop(vm); \
	BLOCK; \
	CalVM_Push(vm, a); \
} while(0)

// TODO: always use the thingy format for signed numbers
#define OP_SIGNED_BIN(BLOCK) do { \
	int32_t b = (int32_t) CalVM_Pop(vm); \
	int32_t a = (int32_t) CalVM_Pop(vm); \
	BLOCK; \
	CalVM_Push(vm, (uint32_t) a); \
} while(0)

#define OP_UNARY(BLOCK) do { \
	uint32_t a = CalVM_Pop(vm); \
	BLOCK; \
	CalVM_Push(vm, a); \
} while(0)

void CalVM_RunInst(CalVM* vm) {
	if (vm->halted) return;

	uint8_t inst  = NextByte(vm);
	bool    imm   = (inst & 0x80)? true : false;
	inst         &= 0x7F;

	if (imm) {
		CalVM_Push(vm, NextWord(vm));
	}

	switch (inst) {
		case INST_NOP:    break;
		case INST_JMP:    VOID_UNARY(vm->ip = a); break;
		case INST_JNZ:    VOID_BIN(if (b != 0) vm->ip = a); break;
		case INST_JZ:     VOID_BIN(if (b == 0) vm->ip = a); break;
		case INST_ADD:    OP_BIN(a += b); break;
		case INST_SUB:    OP_BIN(a -= b); break;
		case INST_MUL:    OP_BIN(a *= b); break;
		case INST_IECALL: assert(0); // TODO
		case INST_DIV:    OP_BIN(a /= b); break;
		case INST_IDIV:   OP_SIGNED_BIN(a /= b); break;
		case INST_MOD:    OP_BIN(a %= b); break;
		case INST_IMOD:   OP_SIGNED_BIN(a %= b); break;
		case INST_DUP: {
			uint32_t value = CalVM_Pop(vm);
			CalVM_Push(vm, value);
			CalVM_Push(vm, value);
			break;
		}
		case INST_OVER: {
			uint32_t b = CalVM_Pop(vm);
			uint32_t a = CalVM_Pop(vm);

			CalVM_Push(vm, a);
			CalVM_Push(vm, b);
			CalVM_Push(vm, a);
			break;
		}
		case INST_SWAP: {
			uint32_t b = CalVM_Pop(vm);
			uint32_t a = CalVM_Pop(vm);

			CalVM_Push(vm, b);
			CalVM_Push(vm, a);
			break;
		}
		case INST_EQU:   OP_BIN(a = (a == b? 0xFFFFFFFF : 0)); break;
		case INST_NEQU:  OP_BIN(a = (a != b? 0xFFFFFFFF : 0)); break;
		case INST_GTH:   OP_BIN(a = (a > b? 0xFFFFFFFF : 0)); break;
		case INST_LTH:   OP_BIN(a = (a < b? 0xFFFFFFFF : 0)); break;
		case INST_IGTH:  OP_SIGNED_BIN(a = (a > b? 0xFFFFFFFF : 0)); break;
		case INST_ILTH:  OP_SIGNED_BIN(a = (a < b? 0xFFFFFFFF : 0)); break;
		case INST_AND:   OP_BIN(a &= b); break;
		case INST_OR:    OP_BIN(a |= b); break;
		case INST_XOR:   OP_BIN(a ^= b); break;
		case INST_NOT:   OP_UNARY(a = ~a); break;
		case INST_WRB:   VOID_BIN(vm->ram[b] = a); break;
		case INST_WRH:   VOID_BIN(CalVM_Write16(vm, b, a)); break;
		case INST_WRW:   VOID_BIN(CalVM_Write32(vm, b, a)); break;
		case INST_RDB:   OP_UNARY(a = vm->ram[a]); break;
		case INST_RDH:   OP_UNARY(a = CalVM_Read16(vm, a)); break;
		case INST_RDW:   OP_UNARY(a = CalVM_Read32(vm, a)); break;
		case INST_CALL:  VOID_UNARY(CalVM_PushReturn(vm, vm->ip); vm->ip = a); break;
		case INST_ECALL: assert(0); // TODO
		case INST_RET:   vm->ip = CalVM_PopReturn(vm); break;
		case INST_SHL:   OP_BIN(a <<= b); break;
		case INST_SHR:   OP_BIN(a >>= b); break;
		case INST_POP:   CalVM_Pop(vm); break;
		case INST_HALT:  if (imm) exit(0); else exit((int) CalVM_Pop(vm)); break;
		case INST_RDSP:  CalVM_Push(vm, vm->dsp); break;
		case INST_WDSP:  vm->dsp = CalVM_Pop(vm); break;
		case INST_RRSP:  CalVM_Push(vm, vm->rsp); break;
		case INST_WRSP:  vm->rsp = CalVM_Pop(vm); break;
		default: {
			fprintf(stderr, "Unknown instruction %.2X\n", inst);
		}
	}
}
