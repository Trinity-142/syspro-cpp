#pragma once
#include <optional>
#include <vector>
#include <string>

namespace Emulator {

    enum Reg {
        R0, R1, R2, R3
    };

    struct EmulatorState {
        static const size_t regs_size = 4;
        static const size_t mem_size  = 1024;

        std::vector<int> _registers{regs_size};
        std::vector<int> _mem{mem_size};

        size_t _pc = 0;
    };

    class Instruction {
    public:
        virtual void eval(EmulatorState& emul) = 0;
        virtual ~Instruction() = default;
    };

    Reg strtoreg(const std::string& str);
    std::vector<Instruction*> parse(const std::string& program);
    int emulate(const std::string& program_text);
}

std::vector<std::string> split(const std::string& s, char delim);
std::optional<std::string> readStringFromFile(const std::string& filename);
