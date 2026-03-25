#pragma once
#include "emulator.h"

namespace Emulator {
    class Store : public Instruction {
    int dst;
    Reg src;

    public:
        Store(int dst, Reg src);
        void eval(EmulatorState& emul) override;
    };
}
