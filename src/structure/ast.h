#ifndef _AST_H
#define _AST_H

#include <string>
#include <vector>
#include <assert.h>

// #include "LIR.h"

namespace LIR
{
    // LIR.h
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
}

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

    enum class rel_t
    {
        LT,
        GT,
        LE,
        GE
    };

    enum class equal_t
    {
        EQ,
        NE
    };

    enum class mul_t
    {
        MULT,
        DIV,
        REM
    };

    enum class add_t
    {
        ADD,
        SUB
    };

    enum class base_t
    {
        INT,
        FLOAT,
        VOID
    };

    enum class unaryop_t
    {
        ADD,
        SUB,
        NOT
    };

    enum class const_op
    {
        ADD,
        SUB,
        MUL,
        REM,
        DIV,
        NOT
    };

    class ConstVal
    {
    public:
        bool can_be_cal;

        bool is_const;
        bool is_int;

        union
        {
            int i;
            float f;
        } val;

        ConstVal() : is_const(false) {}
        ConstVal(void *x) : is_const(true), can_be_cal(false) {}
        ConstVal(int i) : is_const(true), is_int(true), can_be_cal(true)
        {
            this->val.i = i;
        }
        ConstVal(float f) : is_const(true), is_int(false), can_be_cal(true)
        {
            this->val.f = f;
        }
    };

    static ConstVal calConst(const ConstVal &lhs, const const_op op, const ConstVal &rhs)
    {
        if (!lhs.is_const || !rhs.is_const)
        {
            return ConstVal();
        }

        if (!lhs.can_be_cal || !rhs.can_be_cal)
        {
            return ConstVal(nullptr);
        }

        switch (op)
        {
        case const_op::ADD:
            if (lhs.is_int && rhs.is_int)
                return ConstVal(lhs.val.i + rhs.val.i);
            if (lhs.is_int && !rhs.is_int)
                return ConstVal(lhs.val.i + rhs.val.f);
            if (!lhs.is_int && rhs.is_int)
                return ConstVal(lhs.val.f + rhs.val.i);
            if (!lhs.is_int && !rhs.is_int)
                return ConstVal(lhs.val.f + rhs.val.f);
            break;
        case const_op::SUB:
            if (lhs.is_int && rhs.is_int)
                return ConstVal(lhs.val.i - rhs.val.i);
            if (lhs.is_int && !rhs.is_int)
                return ConstVal(lhs.val.i - rhs.val.f);
            if (!lhs.is_int && rhs.is_int)
                return ConstVal(lhs.val.f - rhs.val.i);
            if (!lhs.is_int && !rhs.is_int)
                return ConstVal(lhs.val.f - rhs.val.f);
            break;
        case const_op::MUL:
            if (lhs.is_int && rhs.is_int)
                return ConstVal(lhs.val.i * rhs.val.i);
            if (lhs.is_int && !rhs.is_int)
                return ConstVal(lhs.val.i * rhs.val.f);
            if (!lhs.is_int && rhs.is_int)
                return ConstVal(lhs.val.f * rhs.val.i);
            if (!lhs.is_int && !rhs.is_int)
                return ConstVal(lhs.val.f * rhs.val.f);
            break;
        case const_op::DIV:
            if (lhs.is_int && rhs.is_int)
                return ConstVal(lhs.val.i / rhs.val.i);
            if (lhs.is_int && !rhs.is_int)
                return ConstVal(lhs.val.i / rhs.val.f);
            if (!lhs.is_int && rhs.is_int)
                return ConstVal(lhs.val.f / rhs.val.i);
            if (!lhs.is_int && !rhs.is_int)
                return ConstVal(lhs.val.f / rhs.val.f);
            break;
        case const_op::REM:
            if (lhs.is_int && rhs.is_int)
                return ConstVal(lhs.val.i % rhs.val.i);
            break;
        case const_op::NOT:
            if (rhs.is_int)
                return ConstVal(int(!lhs.val.i));
            break;
        default:;
        }
        return ConstVal();
    }

    class CompRoot
    {
    public:
        std::vector<CompUnit *> comp_list;
        int lineno;
        CompRoot(int lineno) : lineno(lineno) { this->comp_list.reserve(1); }
    };

    /* compunit -> decl | funcdef */
    class CompUnit
    {
    public:
        virtual LIR::Stm *accept(LIR::ASTVisitor &visitor) = 0;
    };

    /* BlockItem → Decl | Stmt */
    class BlockItem
    {
    public:
        virtual LIR::Stm *accept(LIR::ASTVisitor &visitor) = 0;
    };

    /* ========================= lir::BaseUnitDECL ========================= */

    /* decl -> constdecl | vardecl */
    class Decl : public CompUnit, public BlockItem
    {
    public:
        virtual LIR::Stm *accept(LIR::ASTVisitor &visitor) = 0;
    };

    class InitVal
    {
    public:
        bool is_const;
        InitVal() : is_const(false) {}
        virtual LIR::BaseUnit accept(LIR::ASTVisitor &visitor) = 0;
        virtual LIR::BaseUnit calArray(LIR::ASTVisitor &visitor) = 0;
    };

    class Exp : public InitVal
    {
    public:
        ConstVal constval;
        bool ast_isconst() { return this->constval.is_const; }
        Exp() {}
        virtual LIR::BaseUnit accept(LIR::ASTVisitor &visitor) = 0;
        LIR::BaseUnit calArray(LIR::ASTVisitor &visitor);
    };

    class IDExp : public Exp
    {

    public:
        std::string *str;
        int lineno;
        IDExp(std::string *str, int lineno) : str(str), lineno(lineno) {}
        LIR::BaseUnit accept(LIR::ASTVisitor &visitor);
    };

    /* eg: {1, 2, 3, ...} */
    class ArrayInitVals : public InitVal
    {
    public:
        int lineno;
        std::vector<InitVal *> *val_list;
        ArrayInitVals(int lineno, std::vector<InitVal *> *val_list)
            : lineno(lineno), val_list(val_list) {}
        LIR::BaseUnit calArray(LIR::ASTVisitor &visitor);
        LIR::BaseUnit accept(LIR::ASTVisitor &visitor);
    };

    /* constdecl -> 'const' BASE_T [constdef, ...] ';'*/
    class ConstDecl : public Decl
    {

    public:
        base_t b_type;
        std::vector<ConstDef *> *defs;
        int lineno;
        ConstDecl(base_t b_type, std::vector<ConstDef *> *defs, int lineno)
            : b_type(b_type), defs(defs), lineno(lineno) {}
        LIR::Stm *accept(LIR::ASTVisitor &visitor);
    };

    /* 1. constdef -> ident "[exp(CONST)][..][...]" = constval
     * 2. (const array def)         -> ident     = {constval1, cv2, ...}
     * NOTE: array index must be CONST val
     */
    class ConstDef
    {

    public:
        IDExp *id;
        std::vector<Exp *> *array_index_list;
        InitVal *const_init_val;
        int lineno;
        ConstDef(IDExp *id, std::vector<Exp *> *array_index_list,
                 InitVal *const_init_val, int lineno)
            : id(id), array_index_list(array_index_list),
              const_init_val(const_init_val), lineno(lineno) {}
        LIR::Stm *accept(LIR::ASTVisitor &visitor);
    };

    /* vardecl ->  BASE_T [vardef, ...] ';'*/
    class VarDecl : public Decl
    {

    public:
        base_t b_type;
        std::vector<VarDef *> *defs;
        int lineno;
        VarDecl(base_t b_type, std::vector<VarDef *> *defs, int lineno)
            : b_type(b_type), defs(defs), lineno(lineno) {}
        LIR::Stm *accept(LIR::ASTVisitor &visitor);
    };

    /* 1. vardef -> ident "[exp(CONST)][..][...]" = val | (default 0??)
     * 2. (var array def)         -> ident     = {constval1, cv2, ...}
     * NOTE: array index must be CONST val
     */
    class VarDef
    {

    public:
        IDExp *id;
        std::vector<Exp *> *array_index_list;
        InitVal *init_val;
        int lineno;
        VarDef(IDExp *id, std::vector<Exp *> *array_index_list, InitVal *init_val, int lineno)
            : id(id), array_index_list(array_index_list), init_val(init_val), lineno(lineno) {}
        LIR::Stm *accept(LIR::ASTVisitor &visitor);
    };

    /* ========================= FUNC DEF ========================= */

    /* FuncDef → FuncType Ident '(' [FuncFParams] ')' Block */
    class FuncDef : public CompUnit
    {

    public:
        base_t f_type;
        IDExp *func_id;
        std::vector<Parameter *> *parameters;
        Block *block;
        int lineno;
        FuncDef(base_t f_type, IDExp *func_id,
                std::vector<Parameter *> *parameters, Block *block, int lineno)
            : f_type(f_type), func_id(func_id), parameters(parameters), block(block),
              lineno(lineno) {}
        LIR::Stm *accept(LIR::ASTVisitor &visitor);
    };

    /* BType Ident ['[' ']' { '[' Exp ']' }] */
    class Parameter
    {

    public:
        base_t b_type;
        IDExp *id;
        std::vector<Exp *> *array_index_list;
        int lineno;
        Parameter(base_t b_type, IDExp *id, std::vector<Exp *> *array_index_list,
                  int lineno)
            : b_type(b_type), id(id), array_index_list(array_index_list),
              lineno(lineno) {}
    };

    /* Stmt → LVal '=' Exp ';'(AssignStmt) | [Exp] ';' (ExpStmt)| Block
    | 'if' '( Cond ')' Stmt [ 'else' Stmt ]
    | 'while' '(' Cond ')' Stmt
    | 'break' ';' | 'continue' ';'
    | 'return' [Exp] ';'
    */
    class Stmt : public BlockItem
    {
    public:
        Stmt() {}
        virtual LIR::Stm *accept(LIR::ASTVisitor &visitor) = 0;
    };

    class Block : public Stmt
    {

    public:
        std::vector<BlockItem *> *items;
        int lineno;
        Block(std::vector<BlockItem *> *items, int lineno)
            : items(items), lineno(lineno) {}
        LIR::Stm *accept(LIR::ASTVisitor &visitor);
    };

    class AssignStmt : public Stmt
    {

    public:
        LVal *l_val;
        Exp *exp;
        int lineno;
        AssignStmt(LVal *l_val, Exp *exp, int lineno)
            : l_val(l_val), exp(exp), lineno(lineno) {}
        LIR::Stm *accept(LIR::ASTVisitor &visitor);
    };

    class ExpStmt : public Stmt
    {

    public:
        Exp *exp;
        int lineno;
        ExpStmt(Exp *exp, int lineno) : exp(exp), lineno(lineno) {}
        LIR::Stm *accept(LIR::ASTVisitor &visitor);
    };

    class IfStmt : public Stmt
    {

    public:
        Exp *conditon;
        Stmt *if_part = nullptr;
        Stmt *else_part = nullptr;
        int lineno;
        IfStmt(Exp *conditon, Stmt *if_part, Stmt *else_part, int lineno)
            : conditon(conditon), if_part(if_part), else_part(else_part),
              lineno(lineno) {}
        LIR::Stm *accept(LIR::ASTVisitor &visitor);
    };

    class WhileStmt : public Stmt
    {

    public:
        Exp *condition;
        Stmt *loop;
        int lineno;
        WhileStmt(Exp *condition, Stmt *loop, int lineno)
            : condition(condition), loop(loop), lineno(lineno) {}
        LIR::Stm *accept(LIR::ASTVisitor &visitor);
    };

    class BreakStmt : public Stmt
    {

    public:
        int lineno;
        BreakStmt(int lineno) : lineno(lineno) {}
        LIR::Stm *accept(LIR::ASTVisitor &visitor);
    };

    class ContinueStmt : public Stmt
    {

    public:
        int lineno;
        ContinueStmt(int lineno) : lineno(lineno) {}
        LIR::Stm *accept(LIR::ASTVisitor &visitor);
    };

    class ReturnStmt : public Stmt
    {

    public:
        Exp *exp;
        int lineno;
        ReturnStmt(Exp *exp, int lineno) : exp(exp), lineno(lineno) {}
        LIR::Stm *accept(LIR::ASTVisitor &visitor);
    };

    /* PrimaryExp → '(' Exp ')' | LVal | Number */
    class PrimaryExp : public Exp
    {
    public:
        PrimaryExp() {}
        virtual LIR::BaseUnit accept(LIR::ASTVisitor &visitor) = 0;
    };

    /* LVal → Ident {'[' Exp ']'} */
    class LVal : public PrimaryExp
    {

    public:
        IDExp *id;
        std::vector<Exp *> *array_index_list;
        int lineno;
        LVal(IDExp *id, std::vector<Exp *> *array_index_list, int lineno)
            : id(id), array_index_list(array_index_list), lineno(lineno)
        {
            constval = ConstVal(nullptr);
        }
        LIR::BaseUnit accept(LIR::ASTVisitor &visitor);
    };

    /* Number → IntConst | floatConst */
    class Number : public PrimaryExp
    {
    public:
        Number() {}
        virtual LIR::BaseUnit accept(LIR::ASTVisitor &visitor) = 0;
    };

    class IntNumber : public Number
    {

    public:
        int value;
        int lineno;
        IntNumber(int value, int lineno) : value(value), lineno(lineno)
        {
            constval = ConstVal(value);
        }
        LIR::BaseUnit accept(LIR::ASTVisitor &visitor);
    };

    class FloatNumber : public Number
    {

    public:
        std::string *float_str;
        int lineno;
        FloatNumber(std::string *f_str, int lineno)
            : float_str(f_str), lineno(lineno)
        {
            constval = ConstVal(std::stof(*f_str));
        }
        LIR::BaseUnit accept(LIR::ASTVisitor &visitor);
    };

    /* UnaryExp - >  UnaryOp UnaryExp */
    class UnaryExp : public Exp
    {

    public:
        unaryop_t op;
        Exp *exp;
        int lineno;
        UnaryExp(unaryop_t op, Exp *exp, int lineno)
            : op(op), exp(exp), lineno(lineno)
        {
            if (exp->ast_isconst())
                constval = calConst(ConstVal(0),
                                    static_cast<const_op>(op),
                                    exp->constval);
        }
        LIR::BaseUnit accept(LIR::ASTVisitor &visitor);
    };

    /* CallExp -> Ident '(' [FuncRParams] ')'  */
    class CallExp : public Exp
    {

    public:
        IDExp *func_id;
        std::vector<Exp *> *params;
        int lineno;
        CallExp(IDExp *func_id, std::vector<Exp *> *params, int lineno)
            : func_id(func_id), params(params), lineno(lineno) {}
        LIR::BaseUnit accept(LIR::ASTVisitor &visitor);
    };

    class MulExp : public Exp
    {

    public:
        Exp *lhs;
        mul_t op;
        Exp *rhs;
        int lineno;
        MulExp(Exp *lhs, mul_t op, Exp *rhs, int lineno)
            : lhs(lhs), op(op), rhs(rhs), lineno(lineno)
        {
            if (lhs->ast_isconst() && rhs->ast_isconst())
                constval = calConst(lhs,
                                    static_cast<const_op>(op),
                                    rhs);
        }
        LIR::BaseUnit accept(LIR::ASTVisitor &visitor);
    };

    class AddExp : public Exp
    {

    public:
        Exp *lhs;
        add_t op;
        Exp *rhs;
        int lineno;
        AddExp(Exp *lhs, add_t op, Exp *rhs, int lineno)
            : lhs(lhs), op(op), rhs(rhs), lineno(lineno)
        {
            if (lhs->ast_isconst() && rhs->ast_isconst())
                constval = calConst(lhs,
                                    static_cast<const_op>(op),
                                    rhs);
        }
        LIR::BaseUnit accept(LIR::ASTVisitor &visitor);
    };

    class RelExp : public Exp
    {

    public:
        Exp *lhs;
        rel_t op;
        Exp *rhs;
        int lineno;
        RelExp(Exp *lhs, rel_t op, Exp *rhs, int lineno)
            : lhs(lhs), op(op), rhs(rhs), lineno(lineno) {}
        LIR::BaseUnit accept(LIR::ASTVisitor &visitor);
    };

    class EqualExp : public Exp
    {

    public:
        Exp *lhs;
        equal_t op;
        Exp *rhs;
        int lineno;
        EqualExp(Exp *lhs, equal_t op, Exp *rhs, int lineno)
            : lhs(lhs), op(op), rhs(rhs), lineno(lineno) {}
        LIR::BaseUnit accept(LIR::ASTVisitor &visitor);
    };

    class LAndExp : public Exp
    {

    public:
        Exp *lhs;
        Exp *rhs;
        int lineno;
        LAndExp(Exp *lhs, Exp *rhs, int lineno)
            : lhs(lhs), rhs(rhs), lineno(lineno) {}
        LIR::BaseUnit accept(LIR::ASTVisitor &visitor);
    };

    class LOrExp : public Exp
    {

    public:
        Exp *lhs;
        Exp *rhs;
        int lineno;
        LOrExp(Exp *lhs, Exp *rhs, int lineno) : lhs(lhs), rhs(rhs), lineno(lineno) {}
        LIR::BaseUnit accept(LIR::ASTVisitor &visitor);
    };

    class GetIntExp : public Exp
    {
    public:
        int lineno;
        GetIntExp(int l) : lineno(l) {}
        LIR::BaseUnit accept(LIR::ASTVisitor &visitor);
    };

    class GetChExp : public Exp
    {
    public:
        int lineno;
        GetChExp(int l) : lineno(l) {}
        LIR::BaseUnit accept(LIR::ASTVisitor &visitor);
    };

    class GetFloatExp : public Exp
    {
    public:
        int lineno;
        GetFloatExp(int l) : lineno(l) {}
        LIR::BaseUnit accept(LIR::ASTVisitor &visitor);
    };

    class GetArrayExp : public Exp
    {
    public:
        int lineno;
        AST::Exp *arr;
        GetArrayExp(AST::Exp *_arr, int l) : arr(_arr), lineno(l) {}
        LIR::BaseUnit accept(LIR::ASTVisitor &visitor);
    };

    class GetFArrayExp : public Exp
    {
    public:
        int lineno;
        AST::Exp *arr;
        GetFArrayExp(AST::Exp *_arr, int l) : arr(_arr), lineno(l) {}
        LIR::BaseUnit accept(LIR::ASTVisitor &visitor);
    };

    class PutIntStmt : public Stmt
    {
    public:
        Exp *exp;
        int lineno;
        PutIntStmt(Exp *_exp, int _lineno) : exp(_exp), lineno(_lineno) {}
        LIR::Stm *accept(LIR::ASTVisitor &visitor);
    };

    class PutChStmt : public Stmt
    {
    public:
        Exp *exp;
        int lineno;
        PutChStmt(Exp *_exp, int _lineno) : exp(_exp), lineno(_lineno) {}
        LIR::Stm *accept(LIR::ASTVisitor &visitor);
    };

    class PutFloatStmt : public Stmt
    {
    public:
        Exp *exp;
        int lineno;
        PutFloatStmt(Exp *_exp, int _lineno) : exp(_exp), lineno(_lineno) {}
        LIR::Stm *accept(LIR::ASTVisitor &visitor);
    };

    // void putarray(int , int[])
    class PutArrayStmt : public Stmt
    {
    public:
        Exp *len;
        Exp *arr;
        int lineno;
        PutArrayStmt(Exp *_len, Exp *_arr, int _lineno) : len(_len), arr(_arr), lineno(_lineno) {}
        LIR::Stm *accept(LIR::ASTVisitor &visitor);
    };

    class PutFArrayStmt : public Stmt
    {
    public:
        Exp *len;
        Exp *arr;
        int lineno;
        PutFArrayStmt(Exp *_len, Exp *_arr, int _lineno) : len(_len), arr(_arr), lineno(_lineno) {}
        LIR::Stm *accept(LIR::ASTVisitor &visitor);
    };

    class PutFStmt : public Stmt
    {
    public:
        std::vector<Exp *> *args;
        int lineno;
        PutFStmt(std::vector<Exp *> *_args, int _lineno) : args(_args), lineno(_lineno) {}
        LIR::Stm *accept(LIR::ASTVisitor &visitor);
    };

    class StarttimeStmt : public Stmt
    {
    public:
        int lineno;
        StarttimeStmt(int _lineno) : lineno(_lineno) {}
        LIR::Stm *accept(LIR::ASTVisitor &visitor);
    };

    class StoptimeStmt : public Stmt
    {
    public:
        int lineno;
        StoptimeStmt(int _lineno) : lineno(_lineno) {}
        LIR::Stm *accept(LIR::ASTVisitor &visitor);
    };

}; // namespace AST
using namespace AST;

#endif