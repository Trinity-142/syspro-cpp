#include "Load.h"

namespace Emulator {
    Load::Load(Reg dst, int src) : dst(dst), src(src) {}

    void Load::eval(EmulatorState& emul) {
        emul._registers[dst] = emul._mem[src];
    }
}