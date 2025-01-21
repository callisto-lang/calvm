#include <stdio.h>
#include "safe.h"
#include "common.h"

void* SafeMalloc(size_t size) {
	void* ret = malloc(size);

	if (ret == NULL) {
		fprintf(stderr, "Malloc returned NULL\n");
		exit(1);
	}

	return ret;
}

void* SafeRealloc(void* ptr, size_t size) {
	if (ptr == NULL) {
		return SafeMalloc(size);
	}

	void* ret = realloc(ptr, size);

	if (ret == NULL) {
		fprintf(stderr, "Malloc returned NULL\n");
		exit(1);
	}

	return ret;
}
