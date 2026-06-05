#pragma once
#include "emulator.h"

namespace Emulator {
    class Store : public Instruction {
    int dst;
    Reg src;

    public:
        Store(Reg src, int dst);
        void eval(EmulatorState& emul) override;
    };
}
