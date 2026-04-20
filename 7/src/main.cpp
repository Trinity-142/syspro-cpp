#include <iostream>
#include <cassert>
#include <string>
#include "emulator.h"

int main() {
    std::string factorial =
        "Mov R0 5\n"
        "Mov R1 1\n"
        "Jmpz 6\n"
        "Mul R1 R0\n"
        "Sub R0 1\n"
        "Jmp 2\n"
        "Mov R0 R1";
    assert(Emulator::emulate(factorial) == 120);

    std::string math_test =
        "Mov R0 10\n"
        "Add R0 5\n"
        "Sub R0 2\n"
        "Mul R0 2\n"
        "Div R0 2";
    assert(Emulator::emulate(math_test) == 13);

    std::string mem_test =
        "Mov R0 42\n"
        "Store R0 100\n"
        "Mov R0 0\n"
        "Load R0 100";
    assert(Emulator::emulate(mem_test) == 42);

    return 0;
}