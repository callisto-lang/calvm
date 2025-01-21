# CalVM Instructions
All instructions are a 8-bit opcode. Opcodes have this structure:

| Bit/s | Description                                                                |
| ----- | -------------------------------------------------------------------------- |
| 0-6   | Instruction                                                                |
| 7     | Immediate mode                                                             |

## Important information
- All integers are little endian
- Integer types are named like this:
	- Byte = 8 bits
	- Half-Word = 16 bits
	- Word = 32 bits
- The first 64 instructions are reserved for simple CPU instructions that are always
  present. The last 64 instructions are reserved for extension instructions.
- Parameters for instructions are always 32-bit, but they are truncated in these instances:
	- For `WRB`, the first parameter is truncated to 8 bits
	- For `WRH`, the first parameter is truncated to 16 bits
- Integer division is truncated division
- The program is seperate from RAM, and it is in ROM. The program is loaded at address
  0.

# Immediate mode
Immediate mode is a mode for instructions where the final parameter is taken from after
the instruction in ROM instead of from the stack. The behaviour of it is that it
pushes the immediate parameter to the stack before running the instruction. Instructions
with no parameter will still do this in immediate mode. This means that instructions
like `NOP` are used for pushing literals to the stack.

# Instructions
The `Does` section is pseudocode. Parameters are letters in alphabetical order. For example,
`ADD` does this: `A + B`. If an instruction returns multiple values, the operations are
delimited with a comma. The parameter type bit is `I` in the opcodes. Returned values
begin with `#`.


Operations:
- s* = Signed multiply
- s/ = Signed division
- s% = Signed modulus
- s> = Signed greater than
- s< = Signed lesser than
- ! = Bitwise not
- ^ = Bitwise xor

Functions:
- `ecall(N)` - Calls the external function of the ID `N`
- `pushr(N)` - Pushes N to the return stack
- `popr()` - Pops from the return stack and returns the popped value
- `popd()` - Pops from the data stack and returns the popped value
- `exit(N)` - Exits with exit code N

Registers:
- `IP` - Instruction Pointer
- `DSP` - Data Stack Pointer, this is the main stack used by all instructions
- `RSP` - Return Stack Pointer, this is where return addresses go and it's where you
  would usually allocate local variables

Syntax:
- `;` = comment

| Name    | Instruction |Does                                                        |
| ------- | ----------- | ---------------------------------------------------------- |
| NOP     | `I0000000`  | Nothing                                                    |
| JMP     | `I0000001`  | `IP = A`                                                   |
| JNZ     | `I0000010`  | `if (B != 0) IP = A`                                       |
| JZ      | `I0000011`  | `if (B == 0) IP = A`                                       |
| ADD     | `I0000100`  | `#A + B`                                                   |
| SUB     | `I0000101`  | `#A - B`                                                   |
| MUL     | `I0000110`  | `#A * B`                                                   |
| IECALL  | `I0001111`  | `ecall(mem[A]) ; reads a word`                             |
| DIV     | `I0001000`  | `#A / B`                                                   |
| IDIV    | `I0001001`  | `#A s/ B`                                                  |
| MOD     | `I0001010`  | `#A % B`                                                   |
| IMOD    | `I0001011`  | `#A s% B`                                                  |
| DUP     | `I0001100`  | `#A, A`                                                    |
| OVER    | `I0001101`  | `#A, B, A`                                                 |
| SWAP    | `I0001110`  | `#B, A`                                                    |
| EQU     | `I0001111`  | `#if (A == B) -1 else 0`                                   |
| NEQU    | `I0010000`  | `#if (A != B) -1 else 0`                                   |
| GTH     | `I0010001`  | `#if (A > B) -1 else 0`                                    |
| LTH     | `I0010010`  | `#if (A < B) -1 else 0`                                    |
| IGTH    | `I0010011`  | `#if (A s> B) -1 else 0`                                   |
| ILTH    | `I0010100`  | `#if (A s< B) -1 else 0`                                   |
| AND     | `I0010101`  | `#A & B`                                                   |
| OR      | `I0010110`  | `#A | B`                                                   |
| XOR     | `I0010111`  | `#A ^ B`                                                   |
| NOT     | `I0011000`  | `#!A`                                                      |
| WRB     | `I0011001`  | `mem[B] = A ; byte`                                        |
| WRH     | `I0011010`  | `mem[B] = A ; half-word`                                   |
| WRW     | `I0011011`  | `mem[B] = A ; word`                                        |
| RDB     | `I0011100`  | `#mem[A] ; byte`                                           |
| RDH     | `I0011101`  | `#mem[A] ; half-word`                                      |
| RDW     | `I0011110`  | `#mem[A] ; word`                                           |
| CALL    | `I0011111`  | `pushr(IP), IP = A`                                        |
| ECALL   | `I0100000`  | `ecall(A)`                                                 |
| RET     | `I0100001`  | `IP = popr()`                                              |
| SHL     | `I0100010`  | `#A << B`                                                  |
| SHR     | `I0100011`  | `#A >> B`                                                  |
| POP     | `I0100100`  | `popd()`                                                   |
| HALT    | `I0100101`  | `if (I) exit(A) else exit(0)`                              |
| RDSP    | `I0100110`  | `#dsp`                                                     |
| WDSP    | `I0100111`  | `dsp = A`                                                  |
| RRSP    | `I0101000`  | `#rsp`                                                     |
| WRSP    | `I0101001`  | `rsp = A`                                                  |
