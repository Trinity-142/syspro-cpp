#include "Mul.h"

namespace Emulator {
    Mul::Mul(Reg dst, Reg src) : dst(dst), src_reg(src), src_imm(0), is_immediate(false) {}
    Mul::Mul(Reg dst, int imm) : dst(dst), src_reg(R0), src_imm(imm), is_immediate(true) {}

    void Mul::eval(EmulatorState& emul) {
        if (is_immediate) {
            emul._registers[dst] *= src_imm;
        } else {
            emul._registers[dst] *= emul._registers[src_reg];
        }
    }
}