#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <optional>
#include <stdexcept>

#include "emulator.h"
#include "Add.h"
#include "Div.h"
#include "Jmp.h"
#include "Jmpz.h"
#include "Load.h"
#include "Mov.h"
#include "Mul.h"
#include "Store.h"
#include "Sub.h"

std::vector<std::string> split(const std::string& s, char delim) {
    std::stringstream ss(s);
    std::string a;
    std::vector<std::string> res;
    while (std::getline(ss, a, delim)) {
        res.push_back(a);
    }
    return res;
}

std::optional<std::string> readStringFromFile(const std::string& filename) {
    std::ifstream file{filename};
    if (!file) return {};
    std::stringstream buf;
    buf << file.rdbuf();
    return buf.str();
}

namespace Emulator {
    std::optional<Reg> strtoreg(const std::string& str) {
        if (str == "R0") return R0;
        if (str == "R1") return R1;
        if (str == "R2") return R2;
        if (str == "R3") return R3;
        return std::nullopt;
    }

    template <typename OpClass>
    void parseTwoOperandInstruction(std::vector<std::string>& words, std::vector<Instruction*>& instructions) {
        auto dst = strtoreg(words[1]);
        if (!dst.has_value()) {
            throw std::invalid_argument("Invalid destination register");
        }
        if (auto src = strtoreg(words[2]); src.has_value()) {
            instructions.push_back(new OpClass(dst.value(), src.value()));
        } else {
            try {
                int imm = stoi(words[2]);
                instructions.push_back(new OpClass(dst.value(), imm));
            } catch (const std::invalid_argument& e) {
                std::cerr << "Invalid immediate: " << words[2] << std::endl;
            }
        }
    }

    std::vector<Instruction*> parse(const std::string& program) {
        std::vector<Instruction*> instructions{};
        std::vector<std::string> lines = split(program, '\n');

        for (int i = 0; i < lines.size(); i++) {
            if (lines[i].empty()) continue;

            std::vector<std::string> words = split(lines[i], ' ');
            if (words.empty()) continue;

            const std::string& op = words[0];
            if (op == "Mov") parseTwoOperandInstruction<Mov>(words, instructions);
            else if (op == "Add") parseTwoOperandInstruction<Add>(words, instructions);
            else if (op == "Sub") parseTwoOperandInstruction<Sub>(words, instructions);
            else if (op == "Mul") parseTwoOperandInstruction<Mul>(words, instructions);
            else if (op == "Div") parseTwoOperandInstruction<Div>(words, instructions);
            else if (op == "Load") parseTwoOperandInstruction<Load>(words, instructions);
            else if (op == "Store") parseTwoOperandInstruction<Store>(words, instructions);
            else if (op == "Jmp") {
                try {
                    instructions.push_back(new Jmp(std::stoi(words[1])));
                } catch (const std::invalid_argument& e) {
                    std::cerr << "Invalid immediate: " << words[1] << std::endl;
                }
            }
            else if (op == "Jmpz") {
                try {
                    instructions.push_back(new Jmpz(std::stoi(words[1])));
                } catch (const std::invalid_argument& e) {
                    std::cerr << "Invalid immediate: " << words[1] << std::endl;
                }
            }
        }
        return instructions;
    }

    int emulate(const std::string& program_text) {
        std::vector<Instruction*> program = parse(program_text);
        EmulatorState state;

        while (state._pc < program.size()) {
            size_t current_pc = state._pc;
            program[current_pc]->eval(state);
            if (state._pc == current_pc) {
                state._pc++;
            }
        }

        for (size_t i = 0; i < program.size(); i++) {
            delete program[i];
        }

        return state._registers[R0];
    }
}