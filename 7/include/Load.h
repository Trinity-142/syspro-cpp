#pragma once
#include "emulator.h"

namespace Emulator {
    class Load : public Instruction {
    Reg dst;
    int src;

    public:
        Load(Reg dst, int src);
        void eval(EmulatorState& emul) override;
    };
}
