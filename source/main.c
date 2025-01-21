#include <stdio.h>
#include "safe.h"
#include "calvm.h"
#include "common.h"

const char* usage =
	"Usage: %s <PROGRAM.cvm>\n";

int main(int argc, char** argv) {
	if (argc == 1) {
		printf(usage, argv[0]);
		return 0;
	}

	FILE* file = fopen(argv[1], "rb");

	if (file == NULL) {
		perror(argv[1]);
		return 1;
	}

	fseek(file, 0, SEEK_END);
	size_t size = (size_t) ftell(file);
	rewind(file);

	uint8_t* program = SafeMalloc(size);
	fread(program, 1, size, file);
	fclose(file);

	CalVM vm;
	CalVM_Init(&vm, program, 16777216);

	while (!vm.halted) {
		CalVM_RunInst(&vm);
	}

	CalVM_Free(&vm);
}
