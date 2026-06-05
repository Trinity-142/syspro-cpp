#pragma once
#include "emulator.h"

namespace Emulator {
    class Div : public Instruction {
    Reg dst, src_reg;
    int src_imm;
    bool is_immediate;

    public:
        Div(Reg dst, Reg src);
        Div(Reg dst, int imm);
        void eval(EmulatorState& emul) override;
    };
}