#ifndef EXCEPTION_H
#define EXCEPTION_H

#define ZERO_EXCEPTION_ID 0
#define INVALID_OPCODE_EXCEPTION_ID 6
#define REGS_AMOUNT 18

void printRegisters();

void exceptionDispatcher(int exception);

void intToHex(uint64_t num, char* hex);


#endif