#pragma once
#include "emulator.h"

namespace Emulator {
    class Sub : public Instruction {
    Reg dst, src_reg;
    int src_imm;
    bool is_immediate;

    public:
        Sub(Reg dst, Reg src);
        Sub(Reg dst, int imm);
        void eval(EmulatorState& emul) override;
    };
}
