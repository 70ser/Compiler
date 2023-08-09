#pragma once
#include <iostream>
#include <unordered_map>
#include <vector>
#include <assert.h>

#include "../util/label.h"

namespace ASM
{
    static bool imm8m(int n)
    {
        for (int i = 0; i <= 24; i += 2)
        {
            int mask = ~(0xff << i);
            if ((mask & n) == 0)
                return true;
        }
        return false;
    }

    enum class InstrType
    {
        operand,
        move,
        label
    };

    class Instruction
    {
    public:
        virtual void print(std::unordered_map<int, int> *tmp_replace = nullptr) = 0;
        InstrType kind;
        Instruction() {}
    };

    class Operand : public Instruction
    {
    public:
        std::string asm_stm;
        std::vector<int> dst, src;
        std::vector<std::string> jumps;

        void print(std::unordered_map<int, int> *tmp_replace = nullptr)
        {
            std::string out = asm_stm;
            std::string target;
            int i = 0;
            for (auto d : dst)
            {
                target = "$d" + std::to_string(i++);
                if (out.find(target) == std::string::npos)
                    break;
                out = out.replace(out.find(target), target.size(), reg_temp_name(d));
            }
            i = 0;
            for (auto d : src)
            {
                target = "$s" + std::to_string(i++);
                if (out.find(target) == std::string::npos)
                    break;
                out = out.replace(out.find(target), target.size(), reg_temp_name(d));
            }
            // opt for mov
            if (out.size() > 4 && out.substr(0, 4) == "$mov")
            {
                int len = out.size();
                int p = -1;
                for (int i = 0; i < out.size(); i++)
                {
                    if (out[i] == '#')
                    { // instant number
                        p = i + 1;
                        break;
                    }
                    else if (out[i] == '@')
                    { // name
                        p = i + 1;
                        break;
                    }
                }
                assert(p != -1);
                std::string target = "$mov";
                if (out[p - 1] == '#')
                {
                    int x = std::stoi(out.substr(p, len - p));
                    if (imm8m(x))
                    {
                        out = out.replace(out.find(target), target.size(), "mov");
                        std::cout << out << std::endl;
                    }
                    else if (x < 0 && x >= -257)
                    {
                        out = out.substr(0, p);
                        out = out.replace(out.find(target), target.size(), "mvn") + std::to_string(-x - 1);
                        std::cout << out << std::endl;
                    }
                    else
                    {
                        auto out1 = out.substr(0, p), out2 = out.substr(0, p);
                        out1 = out1.replace(out1.find(target), target.size(), "movw") + ":lower16:" + std::to_string(x);
                        std::cout << out1 << std::endl;
                        if (!(x >= 0 && x <= 65535))
                        {
                            out2 = out2.replace(out2.find(target), target.size(), "movt") + ":upper16:" + std::to_string(x);
                            std::cout << out2 << std::endl;
                        }
                    }
                }
                else
                {
                    out[p - 1] = '#';
                    auto out1 = out.substr(0, p), out2 = out.substr(0, p);
                    out1 = out1.replace(out1.find(target), target.size(), "movw") +
                           ":lower16:" + out.substr(p, len - p);
                    out2 = out2.replace(out2.find(target), target.size(), "movt") +
                           ":upper16:" + out.substr(p, len - p);
                    std::cout << out1 << std::endl;
                    std::cout << out2 << std::endl;
                }
                return;
            }
            std::cout << out << std::endl;
        }

        Operand(std::string _asm_stm,
                std::vector<int> _dst, std::vector<int> _src,
                std::vector<std::string> _jumps)
            : asm_stm(_asm_stm), dst(_dst), src(_src), jumps(_jumps)
        {
            this->kind = InstrType::operand;
        }
    };

    class Move : public Instruction
    {
    public:
        std::string asm_stm;
        std::vector<int> dst, src;

        void print(std::unordered_map<int, int> *tmp_replace = nullptr)
        {
            std::string out = asm_stm;
            out = out.replace(out.find("$d0"), 3, reg_temp_name(dst[0]));
            out = out.replace(out.find("$s0"), 3, reg_temp_name(src[0]));
            std::cout << out << std::endl;
        }

        Move(std::string _asm_stm,
             std::vector<int> _dst, std::vector<int> _src) : asm_stm(_asm_stm), dst(_dst), src(_src)
        {
            assert(this->dst.size() == 1 && this->src.size() == 1);
            this->kind = InstrType::move;
        }
    };

    class Label : public Instruction
    {
    public:
        std::string label;

        void print(std::unordered_map<int, int> *tmp_replace = nullptr)
        {
            std::cout << this->label << ":" << std::endl;
        }

        Label(std::string _label) : label(_label) { this->kind = InstrType::label; }
    };

    class Proc
    {
    public:
        std::vector<Instruction *> i_list;
        void print(std::unordered_map<int, int> *tmp_replace = nullptr)
        {
            for (auto it : i_list)
                it->print(tmp_replace);
        }
    };

};