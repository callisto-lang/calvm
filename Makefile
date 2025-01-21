SRC   = $(wildcard source/*.c) $(wildcard source/**/*.c)
DEPS  = $(wildcard source/*.h) $(wildcard source/**/*.h)
OBJ   = $(addsuffix .o,$(subst source/,bin/,$(basename ${SRC})))
OUT   = calvm
FLAGS = -std=c99 -Wall -Wextra -pedantic -Istb -Wuninitialized

ifeq ($(BUILD), release)
	FLAGS += -O3 -s
else
	FLAGS += -g -Og
endif

compile: ./bin $(OBJ) $(SRC) $(DEPS)
	$(CC) $(OBJ) $(LIBS) -o $(OUT) $(FLAGS)

./bin:
	mkdir -p bin

bin/%.o: source/%.c $(DEPS)
	$(CC) -c $< $(FLAGS) -o $@

clean:
	rm -r bin/* $(OUT)
	if [ -f bin ]; then rm -r bin; fi
	if [ -f $(OUT) ]; then rm $(OUT); fi


