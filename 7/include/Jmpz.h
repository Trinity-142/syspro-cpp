#pragma once
#include "emulator.h"

namespace Emulator {
    class Jmpz : public Instruction {
    int src;

    public:
        explicit Jmpz(int src);
        void eval(EmulatorState& emul) override;
    };
}
