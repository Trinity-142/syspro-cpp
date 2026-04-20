#pragma once
#include "emulator.h"

namespace Emulator {
    class Mul : public Instruction {
    Reg dst, src_reg;
    int src_imm;
    bool is_immediate;

    public:
        Mul(Reg dst, Reg src);
        Mul(Reg dst, int imm);
        void eval(EmulatorState& emul) override;
    };
}
