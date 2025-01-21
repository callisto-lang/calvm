#ifndef VM_CALVM_H
#define VM_CALVM_H

#include "common.h"

typedef struct {
	uint8_t* rom;
	uint32_t romSize;
	uint8_t* ram;
	uint32_t ramSize;
	uint32_t ip;
	uint32_t dsp;
	uint32_t rsp;
	bool     halted;
} CalVM;

void CalVM_Init(CalVM* vm, uint8_t* program, uint32_t ramSize);
void CalVM_Free(CalVM* vm);

// memory
uint16_t CalVM_Read16(CalVM* vm, uint32_t addr);
uint32_t CalVM_Read32(CalVM* vm, uint32_t addr);
void     CalVM_Write16(CalVM* vm, uint32_t addr, uint16_t value);
void     CalVM_Write32(CalVM* vm, uint32_t addr, uint32_t value);
void     CalVM_Push(CalVM* vm, uint32_t value);
uint32_t CalVM_Pop(CalVM* vm);
void     CalVM_PushReturn(CalVM* vm, uint32_t value);
uint32_t CalVM_PopReturn(CalVM* vm);

void CalVM_RunInst(CalVM* vm);

#endif
