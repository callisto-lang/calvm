#ifndef VM_CALVM_H
#define VM_CALVM_H

#include "util.h"
#include "common.h"

typedef struct CalVM CalVM;
typedef void (*EFunc)(CalVM*);

struct CalVM {
	uint8_t* rom;
	uint32_t romSize;
	uint8_t* ram;
	uint32_t ramSize;
	uint32_t ip;
	uint32_t dsp;
	uint32_t rsp;
	bool     halted;
	uint32_t exitCode;
	EFunc    efuncs[1024];
};

void  CalVM_Init(CalVM* vm, uint8_t* program, uint32_t ramSize);
void  CalVM_Free(CalVM* vm);
void  CalVM_DumpState(CalVM* vm);
void  CalVM_DumpRom(CalVM* vm);
char* CalVM_GetString(CalVM* vm, uint32_t addr, size_t length);
void  CalVM_RunInst(CalVM* vm);

// memory
static inline uint8_t CalVM_Read8(CalVM* vm, uint32_t addr) {
	if (addr >= vm->ramSize) {
		fprintf(
			stderr, "\n\nERROR! Out of bounds memory access %.8X (max %.8X)\n", addr,
			vm->ramSize
		);
		exit(1);
	}

	return vm->ram[addr];
}

static inline uint16_t CalVM_Read16(CalVM* vm, uint32_t addr) {
	return
		((uint16_t) CalVM_Read8(vm, addr)) |
		(((uint16_t) CalVM_Read8(vm, addr + 1)) << 8);
}

static inline uint32_t CalVM_Read32(CalVM* vm, uint32_t addr) {
	if (addr + 3 >= vm->ramSize) {
		fprintf(
			stderr, "\n\nERROR! Out of bounds memory access %.8X (max %.8X)\n", addr,
			vm->ramSize
		);
		exit(1);
	}

	return Read32(&vm->ram[addr]);
}

static inline void CalVM_Write8(CalVM* vm, uint32_t addr, uint8_t value) {
	if (addr >= vm->ramSize) {
		fprintf(
			stderr, "\n\nERROR! Out of bounds memory access %.8X (max %.8X)\n", addr,
			vm->ramSize
		);
		exit(1);
	}

	vm->ram[addr] = value;
}

static inline void CalVM_Write16(CalVM* vm, uint32_t addr, uint16_t value) {
	CalVM_Write8(vm, addr,     (uint8_t) (value & 0xFF));
	CalVM_Write8(vm, addr + 1, (uint8_t) ((value & 0xFF00) >> 8));
}

static inline void CalVM_Write32(CalVM* vm, uint32_t addr, uint32_t value) {
	CalVM_Write8(vm, addr,     (uint8_t) (value & 0xFF));
	CalVM_Write8(vm, addr + 1, (uint8_t) ((value & 0xFF00) >> 8));
	CalVM_Write8(vm, addr + 2, (uint8_t) ((value & 0xFF0000) >> 16));
	CalVM_Write8(vm, addr + 3, (uint8_t) ((value & 0xFF000000) >> 24));
}

static inline void CalVM_Push(CalVM* vm, uint32_t value) {
	CalVM_Write32(vm, vm->dsp, value);
	vm->dsp += 4;
}

static inline uint32_t CalVM_Pop(CalVM* vm) {
	vm->dsp -= 4;
	return CalVM_Read32(vm, vm->dsp);
}

static inline void CalVM_PushReturn(CalVM* vm, uint32_t value) {
	vm->rsp -= 4;
	CalVM_Write32(vm, vm->rsp, value);
}

static inline uint32_t CalVM_PopReturn(CalVM* vm) {
	uint32_t ret = CalVM_Read32(vm, vm->rsp);
	vm->rsp += 4;
	return ret;
}

#endif
