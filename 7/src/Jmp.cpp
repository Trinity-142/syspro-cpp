#include "Jmp.h"

namespace Emulator {
    Jmp::Jmp(int src) : src(src) {}

    void Jmp::eval(EmulatorState& emul) {
        emul._pc = src;
    }
}