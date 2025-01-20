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
| NOP     | 00000000    | Nothing                                                    |
| JMP     | 00000001    | `IP = A`                                                   |
| JNZ     | 00000010    | `if (B != 0) IP = A`                                       |
| JZ      | 00000011    | `if (B == 0  IP = A`                                       |
| ADD     | 00000100    | `#A + B`                                                   |
| SUB     | 00000101    | `#A - B`                                                   |
| MUL     | 00000110    | `#A * B`                                                   |
| IMUL    | 00000111    | `#A s* B`                                                  |
| DIV     | 00001000    | `#A / B`                                                   |
| IDIV    | 00001001    | `#A s/ B`                                                  |
| MOD     | 00001010    | `#A % B`                                                   |
| IMOD    | 00001011    | `#A s% B`                                                  |
| DUP     | 00001100    | `#A, A`                                                    |
| OVER    | 00001101    | `#A, B, A`                                                 |
| SWAP    | 00001110    | `#B, A`                                                    |
| EQU     | 00001111    | `#if (A == B) -1 else 0`                                   |
| NEQU    | 00010000    | `#if (A != B) -1 else 0`                                   |
| GTH     | 00010001    | `#if (A > B) -1 else 0`                                    |
| LTH     | 00010010    | `#if (A < B) -1 else 0`                                    |
| IGTH    | 00010011    | `#if (A s> B) -1 else 0`                                   |
| ILTH    | 00010100    | `#if (A s< B) -1 else 0`                                   |
| AND     | 00010101    | `#A & B`                                                   |
| OR      | 00010110    | `#A | B`                                                   |
| XOR     | 00010111    | `#A ^ B`                                                   |
| NOT     | 00011000    | `#!A`                                                      |
| WRB     | 00011001    | `mem[B] = A ; byte`                                        |
| WRH     | 00011010    | `mem[B] = A ; half-word`                                   |
| WRW     | 00011011    | `mem[B] = A ; word`                                        |
| RDB     | 00011100    | `#mem[A] ; byte`                                           |
| RDH     | 00011101    | `#mem[A] ; half-word`                                      |
| RDW     | 00011110    | `#mem[A] ; word`                                           |
| CALL    | 00011111    | `pushr(IP), IP = A`                                        |
| ECALL   | 00100000    | `ecall(A)`                                                 |
