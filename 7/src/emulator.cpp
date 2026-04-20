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
    Reg strtoreg(const std::string& str) {
        if (str == "R0") return R0;
        if (str == "R1") return R1;
        if (str == "R2") return R2;
        if (str == "R3") return R3;
        throw std::invalid_argument("invalid register");
    }

    std::vector<Instruction*> parse(const std::string& program) {
        std::vector<Instruction*> instructions{};
        std::vector<std::string> lines = split(program, '\n');

        for (int i = 0; i < lines.size(); i++) {
            if (lines[i].empty()) continue;

            std::vector<std::string> words = split(lines[i], ' ');
            if (words.empty()) continue;

            const std::string& op = words[0];
            if (op == "Mov") {
                Reg dst = strtoreg(words[1]);
                try {
                    Reg src = strtoreg(words[2]);
                    instructions.push_back(new Mov(dst, src));
                } catch (const std::invalid_argument& e) {
                    instructions.push_back(new Mov(dst, std::stoi(words[2])));
                }
            }
            else if (op == "Add") {
                Reg dst = strtoreg(words[1]);
                try {
                    Reg src = strtoreg(words[2]);
                    instructions.push_back(new Add(dst, src));
                } catch (const std::invalid_argument& e) {
                    instructions.push_back(new Add(dst, std::stoi(words[2])));
                }
            }
            else if (op == "Sub") {
                Reg dst = strtoreg(words[1]);
                try {
                    Reg src = strtoreg(words[2]);
                    instructions.push_back(new Sub(dst, src));
                } catch (const std::invalid_argument& e) {
                    instructions.push_back(new Sub(dst, std::stoi(words[2])));
                }
            }
            else if (op == "Mul") {
                Reg dst = strtoreg(words[1]);
                try {
                    Reg src = strtoreg(words[2]);
                    instructions.push_back(new Mul(dst, src));
                } catch (const std::invalid_argument& e) {
                    instructions.push_back(new Mul(dst, std::stoi(words[2])));
                }
            }
            else if (op == "Div") {
                Reg dst = strtoreg(words[1]);
                try {
                    Reg src = strtoreg(words[2]);
                    instructions.push_back(new Div(dst, src));
                } catch (const std::invalid_argument& e) {
                    instructions.push_back(new Div(dst, std::stoi(words[2])));
                }
            }
            else if (op == "Load") {
                instructions.push_back(new Load(strtoreg(words[1]), std::stoi(words[2])));
            }
            else if (op == "Store") {
                instructions.push_back(new Store(std::stoi(words[2]), strtoreg(words[1])));
            }
            else if (op == "Jmp") {
                instructions.push_back(new Jmp(std::stoi(words[1])));
            }
            else if (op == "Jmpz") {
                instructions.push_back(new Jmpz(std::stoi(words[1])));
            }
        }
        return instructions;
    }

    int emulate(const std::string& program_text) {
        std::vector<Instruction*> program = parse(program_text);
        EmulatorState state;

        while (state._pc < program.size()) {
            size_t current_pc = state._pc;
            program[state._pc]->eval(state);
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