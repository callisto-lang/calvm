# CalVM Instructions
All instructions are a 8-bit opcode. Opcodes have this structure:

| Bit/s | Description                                                                |
| ----- | -------------------------------------------------------------------------- |
| 0-7   | Instruction                                                                |
| 1     | Immediate mode                                                             |

## Important information
- All integers are little endian
- Integer types are named like this:
	- Byte = 8 bits
	- Half-Word = 16 bits
	- Word = 32 bits
- The first 64 instructions are reserved for simple CPU instructions that are always
  present. The last 64 instructions are reserved for extension instructions.

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

Syntax:
- `;` = comment

| Name    | Instruction |Does                                                        |
| ------- | ----------- | ---------------------------------------------------------- |
| NOP     | `I0000000`  | Nothing                                                    |
| JMP     | `I0000001`  | `IP = A`                                                   |
| JNZ     | `I0000010`  | `if (B != 0) IP = A`                                       |
| JZ      | `I0000011`  | `if (B == 0  IP = A`                                       |
| ADD     | `I0000100`  | `#A + B`                                                   |
| SUB     | `I0000101`  | `#A - B`                                                   |
| MUL     | `I0000110`  | `#A * B`                                                   |
| IMUL    | `I0000111`  | `#A s* B`                                                  |
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
