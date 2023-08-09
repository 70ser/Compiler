#pragma once

#include <map>
#include "../structure/LIR.h"
#include "controlflow.h"
#include "domintree.h"

namespace SSA
{
    class ExpUnit
    {
    public:
        LIR::EXP_KIND e_kind;
        int val;
        std::string name;

        ExpUnit(LIR::Exp *e) : val(0), name("")
        {
            this->e_kind = e->exp_kind;
            switch (e->exp_kind)
            {
            case LIR::EXP_KIND::name:
            {
                name = static_cast<LIR::NameExp *>(e)->name;
            }
            break;
            case LIR::EXP_KIND::temp:
            {
                val = static_cast<LIR::TempVarExp *>(e)->temp_label_id_int;
            }
            break;
            case LIR::EXP_KIND::constexp:
            {
                val = static_cast<LIR::ConstExp *>(e)->val;
            }
            break;
            default:
                assert(0);
            }
        }

        LIR::Exp *toExp()
        {
            switch (e_kind)
            {
            case LIR::EXP_KIND::name:
            {
                return new LIR::NameExp(name);
            }
            break;
            case LIR::EXP_KIND::temp:
            {
                return new LIR::TempVarExp(val);
            }
            break;
            case LIR::EXP_KIND::constexp:
            {
                return new LIR::ConstExp(val);
            }
            break;
            default:
                assert(0);
            }
            return nullptr;
        }

        bool operator==(const ExpUnit &e) const
        {
            return this->e_kind == e.e_kind && this->val == e.val && this->name == e.name;
        }

        size_t _hash() const
        {
            return std::hash<int>()(static_cast<int>(this->e_kind)) ^
                   std::hash<std::string>()(this->name) ^
                   std::hash<int>()(this->val);
        }
    };

    class BinExp
    {
    public:
        LIR::BIN_OP op;
        ExpUnit *l, *r;

        BinExp(LIR::BIN_OP op, LIR::Exp *l, LIR::Exp *r)
            : op(op), l(new ExpUnit(l)), r(new ExpUnit(r)) {}
    };

    class SSAIR : public CFG::ControlFlowGraph
    {
    private:
        std::map<int, std::vector<int>> def_pos;
        std::map<int, std::stack<int>> tmp_var_stk;

        void gen_phi();
        void rename();
        void rename_i(int node);

    public:
        // <i, <temp_var(id) to deter, node[i]'s sll>>
        std::unordered_map<int, std::map<int, LIR::StmLinkedList *>> Phi_mp;
        std::string end_label;
        DT::DT *dtree;

        SSAIR(IRBlock::BlockController bc);

        IRBlock::BlockController *SSA2blks();

        bool is_phi(LIR::Stm *stm);
    };
}
