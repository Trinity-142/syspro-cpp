#include "Jmpz.h"

namespace Emulator {
    Jmpz::Jmpz(int src) : src(src) {}

    void Jmpz::eval(EmulatorState& emul) {
        emul._pc = (emul._registers[R0] == 0) ? src : (emul._pc + 1);
    }
}