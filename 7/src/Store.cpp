#include "Store.h"

namespace Emulator {
    Store::Store(Reg src, int dst) : dst(dst), src(src) {}

    void Store::eval(EmulatorState& emul) {
        emul._mem[dst] = emul._registers[src];
    }
}