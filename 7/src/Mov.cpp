#include "Mov.h"

namespace Emulator {
    Mov::Mov(Reg dst, Reg src) : dst(dst), src_reg(src), src_imm(0), is_immediate(false) {}
    Mov::Mov(Reg dst, int imm) : dst(dst), src_reg(R0), src_imm(imm), is_immediate(true) {}

    void Mov::eval(EmulatorState& emul) {
        if (is_immediate) {
            emul._registers[dst] = src_imm;
        } else {
            emul._registers[dst] = emul._registers[src_reg];
        }
    }
}