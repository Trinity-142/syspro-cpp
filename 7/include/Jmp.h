#pragma once
#include "emulator.h"

namespace Emulator {
    class Jmp : public Instruction {
    int src;

    public:
        explicit Jmp(int src);
        void eval(EmulatorState& emul) override;
    };
}
