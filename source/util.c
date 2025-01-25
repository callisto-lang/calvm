#include "util.h"

uint32_t Read32(uint8_t* ptr) {
	return
		((uint32_t) ptr[0]) |
		(((uint32_t) ptr[1]) << 8) |
		(((uint32_t) ptr[2]) << 16) |
		(((uint32_t) ptr[3]) << 24);
}
