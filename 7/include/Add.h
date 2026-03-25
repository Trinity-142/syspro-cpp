#pragma once
#include "emulator.h"

namespace Emulator {
    class Add : public Instruction {
    Reg dst, src_reg;
    int src_imm;
    bool is_immediate;

    public:
        Add(Reg dst, Reg src);
        Add(Reg dst, int imm);
        void eval(EmulatorState& emul) override;
    };
}
