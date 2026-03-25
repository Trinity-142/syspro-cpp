#pragma once
#include "emulator.h"

namespace Emulator {
    class Mov : public Instruction {
    Reg dst, src_reg;
    int src_imm;
    bool is_immediate;

    public:
        Mov(Reg dst, Reg src);
        Mov(Reg dst, int imm);
        void eval(EmulatorState& emul) override;
    };
}