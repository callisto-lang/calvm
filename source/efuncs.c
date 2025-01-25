#include <stdio.h>
#include "efuncs.h"

enum {
	FUNC_GET_FUNC = 0,
	FUNC_PRINT_CH,
	FUNC_PRINT_INT
};

static void GetFunc(CalVM* vm) {
	size_t length = CalVM_Pop(vm);
	char*  func   = CalVM_GetString(vm, CalVM_Pop(vm), length);

	if (!strcmp(func, "get_func")) {
		CalVM_Push(vm, FUNC_GET_FUNC);
	}
	else if (!strcmp(func, "print_ch")) {
		CalVM_Push(vm, FUNC_PRINT_CH);
	}
	else {
		fprintf(stderr, "\n\nERROR! Unknown function '%s'\n", func);
		exit(1);
	}

	free(func);
}

static void PrintCH(CalVM* vm) {
	putchar((int) CalVM_Pop(vm));
}

void InitEFuncs(EFunc* res) {
	res[FUNC_GET_FUNC] = &GetFunc;
	res[FUNC_PRINT_CH] = &PrintCH;
}
