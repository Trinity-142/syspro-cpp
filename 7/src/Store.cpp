#include "Store.h"

namespace Emulator {
    Store::Store(int dst, Reg src) : dst(dst), src(src) {}

    void Store::eval(EmulatorState& emul) {
        emul._mem[dst] = emul._registers[src];
    }
}