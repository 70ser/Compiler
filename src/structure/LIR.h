/* Traverse AST nodes to generate IR
 * Use the visitor to traverse the AST nodes to generate IR
 * @ref: https://gitlab.eduxiji.net/educg-group-12619-928705/penguincompiler
 */
#ifndef _LIR_H
#define _LIR_H
#include <string>
#include <vector>
#include <unordered_set>

#include "instruction.h"
// #include "ast.h"
#include "Type.h"
#include "../util/EntryTable.h"
// #include "../util/linearize.h"

namespace AST
{

    enum class rel_t;
    enum class equal_t;
    enum class mul_t;
    enum class add_t;
    enum class base_t;
    enum class unaryop_t;
    enum class const_op;

    class ConstVal;
    class CompRoot;
    class CompUnit;
    class BlockItem;
    class Decl;
    class InitVal;
    class Exp;
    class IDExp;
    class ArrayInitVals;
    class ConstDecl;
    class ConstDef;
    class VarDecl;
    class VarDef;
    class FuncDef;
    class Parameter;
    class Block;
    class Stmt;
    class AssignStmt;
    class ExpStmt;
    class IfStmt;
    class WhileStmt;
    class BreakStmt;
    class ContinueStmt;
    class ReturnStmt;
    class PrimaryExp;
    class LVal;
    class Number;
    class IntNumber;
    class FloatNumber;
    class UnaryExp;
    class CallExp;
    class MulExp;
    class AddExp;
    class RelExp;
    class EqualExp;
    class LAndExp;
    class LOrExp;

    class GetIntExp;
    class GetChExp;
    class GetFloatExp;
    class GetArrayExp;
    class GetFArrayExp;

    class PutIntStmt;
    class PutChStmt;
    class PutFloatStmt;
    class PutArrayStmt;
    class PutFArrayStmt;
    class PutFStmt;

    class StarttimeStmt;
    class StoptimeStmt;

}

namespace LIR
{

    class Stm;
    class StmList;
    class LabelStm;
    class JumpStm;
    class CJumpStm;
    class MoveStm;
    class ExpStm;

    class Exp;
    // class ExpList;
    class BinOpExp;
    class TempVarExp;
    class MemExp;
    class NameExp;
    class CallExp;
    class ConstExp;

    class StmWithRetExp;

    class Condition;

    class BaseUnit;

    class ASTVisitor;

    enum class STM_KIND;
    enum class RelOp;
    enum class EXP_KIND;
    enum class BIN_OP;
    enum class UnitKind;

    enum class STM_KIND
    {
        stm_list,
        label,
        jump,
        cjump,
        move,
        exp
    };

    enum class RelOp
    {
        T_eq,
        T_ne,
        T_lt,
        T_gt,
        T_le,
        T_ge,
        F_eq,
        F_ne,
        F_lt,
        F_gt,
        F_le,
        F_ge
    };

    enum class EXP_KIND
    {
        binop,
        temp,
        mem,
        name,
        call,
        constexp,
        stm_with_ret_exp
    };

    enum class BIN_OP
    {
        T_plus,
        T_minus,
        T_mul,
        T_div,
        T_mod,
        F_plus,
        F_minus,
        F_mul,
        F_div
    };

    enum class UnitKind
    {
        STM_U,
        EXP_U,
        CONDITION_U
    };

    // binExp, callExp, memExp(array) ....
    class Exp
    {
    public:
        EXP_KIND exp_kind;

        virtual void printIR() { assert(0); }

        virtual int ir2asm(std::vector<ASM::Instruction *> *l) = 0;

        virtual Exp *copy_inline(std::unordered_set<std::string> &global_var,
                                 std::unordered_map<int, int> &tmp_var_mp,
                                 std::unordered_map<std::string, std::string> &label_mp,
                                 std::vector<LIR::Stm *> &sl) = 0;
    };

    // IR is a list of Stm !!!
    class Stm
    {
    public:
        STM_KIND stm_kind;

        Stm() {}
        virtual void printIR() { assert(0); }

        virtual void ir2asm(std::vector<ASM::Instruction *> *l) = 0;

        virtual void copy_inline(std::unordered_set<std::string> &global_var,
                                 std::unordered_map<int, int> &tmp_var_mp,
                                 std::unordered_map<std::string, std::string> &label_mp,
                                 std::vector<LIR::Stm *> &sl) = 0;
    };

    class StmList : public Stm
    {
    public:
        std::vector<Stm *> stm_list;

        void add2StmList(Stm *s);

        void printIR();
        void ir2asm(std::vector<ASM::Instruction *> *l);
        void copy_inline(std::unordered_set<std::string> &global_var,
                         std::unordered_map<int, int> &tmp_var_mp,
                         std::unordered_map<std::string, std::string> &label_mp,
                         std::vector<LIR::Stm *> &sl);
        StmList() { stm_kind = STM_KIND::stm_list; }
    };

    class LabelStm : public Stm
    {
    public:
        std::string label_str;
        void printIR();
        void ir2asm(std::vector<ASM::Instruction *> *l);
        void copy_inline(std::unordered_set<std::string> &global_var,
                         std::unordered_map<int, int> &tmp_var_mp,
                         std::unordered_map<std::string, std::string> &label_mp,
                         std::vector<LIR::Stm *> &sl);
        LabelStm(std::string ls)
        {
            this->label_str = ls;
            stm_kind = STM_KIND::label;
        }
    };

    class JumpStm : public Stm
    {
    public:
        std::string jump_label_str;
        void printIR();
        void ir2asm(std::vector<ASM::Instruction *> *l);
        void copy_inline(std::unordered_set<std::string> &global_var,
                         std::unordered_map<int, int> &tmp_var_mp,
                         std::unordered_map<std::string, std::string> &label_mp,
                         std::vector<LIR::Stm *> &sl);
        JumpStm(std::string ls)
        {
            this->jump_label_str = ls;
            stm_kind = STM_KIND::jump;
        }
    };

    class CJumpStm : public Stm
    {
    public:
        RelOp op;
        LIR::Exp *l, *r;
        std::string trueLabel, falseLabel;
        void printIR();
        void ir2asm(std::vector<ASM::Instruction *> *l);
        void copy_inline(std::unordered_set<std::string> &global_var,
                         std::unordered_map<int, int> &tmp_var_mp,
                         std::unordered_map<std::string, std::string> &label_mp,
                         std::vector<LIR::Stm *> &sl);
        CJumpStm(RelOp op, LIR::Exp *l, LIR::Exp *f,
                 std::string trueLabel, std::string falseLabel)
        {
            this->op = op;
            stm_kind = STM_KIND::cjump;
            this->l = l;
            this->r = f;
            this->trueLabel = trueLabel;
            this->falseLabel = falseLabel;
        }
    };

    class MoveStm : public Stm
    {
    public:
        LIR::Exp *dst, *src;
        void printIR();
        void ir2asm(std::vector<ASM::Instruction *> *l);
        void copy_inline(std::unordered_set<std::string> &global_var,
                         std::unordered_map<int, int> &tmp_var_mp,
                         std::unordered_map<std::string, std::string> &label_mp,
                         std::vector<LIR::Stm *> &sl);
        MoveStm(LIR::Exp *_dst, LIR::Exp *_src)
        {
            stm_kind = STM_KIND::move;
            this->dst = _dst;
            this->src = _src;
        }
    };

    class ExpStm : public Stm
    {
    public:
        LIR::Exp *exp;
        void printIR();
        void ir2asm(std::vector<ASM::Instruction *> *l);
        void copy_inline(std::unordered_set<std::string> &global_var,
                         std::unordered_map<int, int> &tmp_var_mp,
                         std::unordered_map<std::string, std::string> &label_mp,
                         std::vector<LIR::Stm *> &sl);
        ExpStm(LIR::Exp *e)
        {
            this->exp = e;
            stm_kind = STM_KIND::exp;
        }
    };

    // class ExpList : public LIR::Exp
    // {
    // public:
    //     std::vector<LIR::Exp> exp_list;
    //     void printIR();
    //     void add2ExpList(LIR::Exp s) { this->exp_list.push_back(s); }
    //     int ir2asm(std::vector<ASM::Instruction *> *l) { return 0; }
    //     ExpList(std::vector<LIR::Exp> _exp_list)
    //     {
    //         this->exp_list = _exp_list;
    //         exp_kind = EXP_KIND::exp_list;
    //     }
    // };

    class BinOpExp : public LIR::Exp
    {
    public:
        LIR::Exp *l, *r;

        BIN_OP binop;
        int ir2asm(std::vector<ASM::Instruction *> *l);
        void printIR();
        Exp *copy_inline(std::unordered_set<std::string> &global_var,
                         std::unordered_map<int, int> &tmp_var_mp,
                         std::unordered_map<std::string, std::string> &label_mp,
                         std::vector<LIR::Stm *> &sl);
        BinOpExp(BIN_OP _binop, LIR::Exp *l, LIR::Exp *r)
        {
            this->binop = _binop;
            this->l = l;
            this->r = r;
            exp_kind = EXP_KIND::binop;
        }
    };

    class TempVarExp : public LIR::Exp
    {
    public:
        int temp_label_id_int;

        void printIR();
        int ir2asm(std::vector<ASM::Instruction *> *l);
        Exp *copy_inline(std::unordered_set<std::string> &global_var,
                         std::unordered_map<int, int> &tmp_var_mp,
                         std::unordered_map<std::string, std::string> &label_mp,
                         std::vector<LIR::Stm *> &sl);
        TempVarExp(int _temp_label_id)
        {
            this->temp_label_id_int = _temp_label_id;
            exp_kind = EXP_KIND::temp;
        }
    };

    class MemExp : public LIR::Exp
    {
    public:
        LIR::Exp *mem_addr;
        void printIR();
        int ir2asm(std::vector<ASM::Instruction *> *l);
        Exp *copy_inline(std::unordered_set<std::string> &global_var,
                         std::unordered_map<int, int> &tmp_var_mp,
                         std::unordered_map<std::string, std::string> &label_mp,
                         std::vector<LIR::Stm *> &sl);
        MemExp(LIR::Exp *_mem_addr)
        {
            this->mem_addr = _mem_addr;
            exp_kind = EXP_KIND::mem;
        }
    };

    class NameExp : public LIR::Exp
    {
    public:
        std::string name;
        void printIR();
        int ir2asm(std::vector<ASM::Instruction *> *l);
        Exp *copy_inline(std::unordered_set<std::string> &global_var,
                         std::unordered_map<int, int> &tmp_var_mp,
                         std::unordered_map<std::string, std::string> &label_mp,
                         std::vector<LIR::Stm *> &sl);
        NameExp(std::string _name)
        {
            this->name = _name;
            exp_kind = EXP_KIND::name;
        }
    };

    class CallExp : public LIR::Exp
    {
    public:
        std::string func_name_str;
        std::vector<LIR::Exp *> args;
        void printIR();
        int ir2asm(std::vector<ASM::Instruction *> *l);
        Exp *copy_inline(std::unordered_set<std::string> &global_var,
                         std::unordered_map<int, int> &tmp_var_mp,
                         std::unordered_map<std::string, std::string> &label_mp,
                         std::vector<LIR::Stm *> &sl);
        CallExp(std::string _func_name_str,
                std::vector<LIR::Exp *> _args)
        {
            this->func_name_str = _func_name_str;
            this->args = _args;
            exp_kind = EXP_KIND::call;
        }
    };

    class ConstExp : public LIR::Exp
    {
    public:
        int val;

        bool is_int_constexp;
        int ir2asm(std::vector<ASM::Instruction *> *l);
        Exp *copy_inline(std::unordered_set<std::string> &global_var,
                         std::unordered_map<int, int> &tmp_var_mp,
                         std::unordered_map<std::string, std::string> &label_mp,
                         std::vector<LIR::Stm *> &sl);
        void printIR();
        ConstExp(int i)
        {
            this->val = i;
            exp_kind = EXP_KIND::constexp;
            is_int_constexp = true;
        }
    };

    class StmWithRetExp : public LIR::Exp
    {
    public:
        LIR::Exp *ret_exp;
        LIR::Stm *stm;
        void printIR();
        int ir2asm(std::vector<ASM::Instruction *> *l);
        Exp *copy_inline(std::unordered_set<std::string> &global_var,
                         std::unordered_map<int, int> &tmp_var_mp,
                         std::unordered_map<std::string, std::string> &label_mp,
                         std::vector<LIR::Stm *> &sl);
        StmWithRetExp(LIR::Exp *_ret_exp, LIR::Stm *_stm)
        {
            this->ret_exp = _ret_exp;
            this->stm = _stm;
            this->exp_kind = EXP_KIND::stm_with_ret_exp;
        }
    };

    class StmLinkedList
    {
    public:
        LIR::Stm *stm;
        StmLinkedList *next;
        StmLinkedList(LIR::Stm *_stm, StmLinkedList *_next) : stm(_stm), next(_next) {}

        StmLinkedList *copy_inline(std::unordered_set<std::string> &global_var,
                                   int fp_offset,
                                   std::unordered_map<int, int> &tmp_var_mp,
                                   std::unordered_map<std::string, std::string> &label_mp)
        {
            int tmp_fp_id = newLocalTempVarID();
            tmp_var_mp[11] = tmp_fp_id; // r11 as fp
            std::vector<LIR::Stm *> sl;
            StmLinkedList *ans = nullptr;
            bool flag = false;
            for (auto sll = this; sll; sll = sll->next)
            {
                sll->stm->copy_inline(global_var, tmp_var_mp, label_mp, sl);
                if (!flag)
                {
                    flag = true;
                    sl.push_back(new LIR::MoveStm(new LIR::TempVarExp(tmp_fp_id),
                                                  new LIR::BinOpExp(LIR::BIN_OP::T_plus,
                                                                    new LIR::TempVarExp(11),
                                                                    new LIR::ConstExp(fp_offset))));
                }
            }
            for (int i = sl.size() - 1; i >= 0; i--)
                ans = new StmLinkedList(sl[i], ans);
            return ans;
        }
    };

    class PatchList
    {
    public:
        std::string *head;
        PatchList *tail;
        PatchList(std::string *_head, PatchList *_tail)
            : head(_head), tail(_tail) {}
    };

    // trues{ label1, label2 ... } , Smt
    class Condition
    {
    public:
        PatchList *trues; // true labels collection
        PatchList *falses;
        Stm *condition_stm;

        Condition() {}

        Condition(PatchList *_trues,
                  PatchList *_falses,
                  Stm *_condition_stm)
            : trues(_trues), falses(_falses), condition_stm(_condition_stm) {}
    };

    // BaseUnit -> {Stm, LIR::Exp, Condition}
    // can be decoded to {Stm, LIR::Exp, Condition}
    class BaseUnit
    {
    public:
        UnitKind unit_kind;
        Type *unit_res_type;

        // if needed
        Stm *stm;
        LIR::Exp *exp;
        Condition condtion_s;

        BaseUnit() {}

        // as Stm
        BaseUnit(Type *_res_t, Stm *_stm)
            : unit_kind(UnitKind::STM_U), unit_res_type(_res_t), stm(_stm) {}

        // as LIR::Exp
        BaseUnit(Type *_res_t, LIR::Exp *_exp)
            : unit_kind(UnitKind::EXP_U), unit_res_type(_res_t), exp(_exp) {}

        // as Condtion
        BaseUnit(Type *_res_t, Condition _condtion_s)
            : unit_kind(UnitKind::CONDITION_U), unit_res_type(_res_t), condtion_s(_condtion_s) {}

        Stm *decode2Stm();

        LIR::Exp *decode2Exp();

        Condition decode2Condtion();
    };

    class ASTVisitor
    {
    public:
        EntryTable<FuncEntry *> *funcEntryTable;

        EntryTable<Entry *> *varEntryTable;

        ASTVisitor(EntryTable<FuncEntry *> *_funcEntryTable,
                   EntryTable<Entry *> *_varEntryTable) : funcEntryTable(_funcEntryTable), varEntryTable(_varEntryTable)
        {
        }

        Type prev_defined_type;

        AST::base_t decl_def_type;

        std::string prev_name = "";

        std::string brkLabel;
        std::string conLabel;

        Type *arr_head;
        LIR::Exp *arr_base = nullptr;
        int arr_off = 0;

        Stm *visitConstDeclNode(AST::ConstDecl &node);
        Stm *visitConstDefNode(AST::ConstDef &node);
        Stm *visitVarDeclNode(AST::VarDecl &node);
        Stm *visitVarDefNode(AST::VarDef &node);
        Stm *visitFuncDefNode(AST::FuncDef &node);
        Stm *visitBlockNode(AST::Block &node);
        Stm *visitAssignStmtNode(AST::AssignStmt &node);
        Stm *visitExpStmtNode(AST::ExpStmt &node);
        Stm *visitIfStmtNode(AST::IfStmt &node);
        Stm *visitWhileStmtNode(AST::WhileStmt &node);
        Stm *visitBreakStmtNode(AST::BreakStmt &node);
        Stm *visitContinueStmtNode(AST::ContinueStmt &node);
        Stm *visitReturnStmtNode(AST::ReturnStmt &node);
        BaseUnit visitLValStmtNode(AST::LVal &node);
        BaseUnit visitIntNumberStmtNode(AST::IntNumber &node);
        BaseUnit visitFloatNumberNode(AST::FloatNumber &node);
        BaseUnit visitUnaryExpNode(AST::UnaryExp &node);
        BaseUnit visitCallExpNode(AST::CallExp &node);
        BaseUnit visitMulExpNode(AST::MulExp &node);
        BaseUnit visitAddExpExpNode(AST::AddExp &node);
        BaseUnit visitRelExpNode(AST::RelExp &node);
        BaseUnit visitEqExpNode(AST::EqualExp &node);
        BaseUnit visitLAndNode(AST::LAndExp &node);
        BaseUnit visitLOrExpNode(AST::LOrExp &node);
        BaseUnit visitIDExpNode(AST::IDExp &node);
        // BaseUnit visitArrayInitValsNode(AST::ArrayInitVals &node);
        BaseUnit calArrayInitValsFromExp(AST::Exp &node);
        BaseUnit calArrayInitValsFromArrayInitExp(AST::ArrayInitVals &node);

        BaseUnit visitGetIntExpNode(AST::GetIntExp &node);
        BaseUnit visitGetChExpNode(AST::GetChExp &node);
        BaseUnit visitGetFloatExpNode(AST::GetFloatExp &node);
        BaseUnit visitGetArrayExpNode(AST::GetArrayExp &node);
        BaseUnit visitGetFArrayExpNode(AST::GetFArrayExp &node);

        Stm *visitPutIntStmtNode(AST::PutIntStmt &node);
        Stm *visitPutChStmtNode(AST::PutChStmt &node);
        Stm *visitPutFloatStmtNode(AST::PutFloatStmt &node);
        Stm *visitPutArrayStmtNode(AST::PutArrayStmt &node);
        Stm *visitPutFArrayStmtNode(AST::PutFArrayStmt &node);
        Stm *visitPutFStmtNode(AST::PutFStmt &node);

        Stm *visitStarttimStmt(AST::StarttimeStmt &node);
        Stm *visitStoptimStmt(AST::StoptimeStmt &node);
    };

    ASM::Proc *ir2asm(StmLinkedList *ir);

    RelOp notRel(RelOp op);
}

#endif