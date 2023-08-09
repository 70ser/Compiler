#ifndef _CAST_H
#define _CAST_H

#include <cmath>
#include "../structure/ast.h"

static std::string binop2string(LIR::BIN_OP op)
{
    switch (op)
    {
    case LIR::BIN_OP::T_div:
        return "sdiv";
    case LIR::BIN_OP::T_minus:
        return "sub";
    case LIR::BIN_OP::T_mod:
        return "mod";
    case LIR::BIN_OP::T_mul:
        return "mul";
    case LIR::BIN_OP::T_plus:
        return "add";
    case LIR::BIN_OP::F_div:
        return "vdiv.f32";
    case LIR::BIN_OP::F_minus:
        return "vsub.f32";
    case LIR::BIN_OP::F_mul:
        return "vmul.f32";
    case LIR::BIN_OP::F_plus:
        return "vadd.f32";
    default:
        assert(0);
    }
}

static std::string relop2string(LIR::RelOp op)
{
    switch (op)
    {
    case LIR::RelOp::T_eq:
        return "eq";
    case LIR::RelOp::T_ne:
        return "ne";
    case LIR::RelOp::T_lt:
        return "lt";
    case LIR::RelOp::T_ge:
        return "ge";
    case LIR::RelOp::T_gt:
        return "gt";
    case LIR::RelOp::T_le:
        return "le";
    case LIR::RelOp::F_eq:
        return "eq";
    case LIR::RelOp::F_ne:
        return "ne";
    case LIR::RelOp::F_lt:
        return "lt";
    case LIR::RelOp::F_ge:
        return "pl";
    case LIR::RelOp::F_gt:
        return "hi";
    case LIR::RelOp::F_le:
        return "le";
    default:
        return "unknown";
    }
}

static bool expEquals(LIR::Exp *a, LIR::Exp *b)
{
    if (a->exp_kind == LIR::EXP_KIND::constexp && b->exp_kind == LIR::EXP_KIND::constexp)
    {
        auto aa = static_cast<LIR::ConstExp *>(a), bb = static_cast<LIR::ConstExp *>(b);
        // if (aa->is_int_constexp && bb->is_int_constexp)
        //     return aa->value.i_val == bb->value.i_val;
        // else if (!aa->is_int_constexp && !bb->is_int_constexp)
        //     return aa->value.f_val == bb->value.f_val;
        // else
        //     return false;
        return aa->val == bb->val;
    }
    if (a->exp_kind == LIR::EXP_KIND::temp && b->exp_kind == LIR::EXP_KIND::temp)
    {
        auto aa = static_cast<LIR::TempVarExp *>(a), bb = static_cast<LIR::TempVarExp *>(b);
        return aa->temp_label_id_int == bb->temp_label_id_int;
    }
    if (a->exp_kind == LIR::EXP_KIND::mem && b->exp_kind == LIR::EXP_KIND::mem)
    {
        auto aa = static_cast<LIR::MemExp *>(a), bb = static_cast<LIR::MemExp *>(b);
        return expEquals(aa->mem_addr, bb->mem_addr);
    }
    if (a->exp_kind == LIR::EXP_KIND::name && b->exp_kind == LIR::EXP_KIND::name)
    {
        auto aa = static_cast<LIR::NameExp *>(a), bb = static_cast<LIR::NameExp *>(b);
        return aa->name == bb->name;
    }
    if (a->exp_kind == LIR::EXP_KIND::binop && b->exp_kind == LIR::EXP_KIND::binop)
    {
        auto aa = static_cast<LIR::BinOpExp *>(a), bb = static_cast<LIR::BinOpExp *>(b);
        if (aa->binop != bb->binop)
            return false;
        if ((aa->binop == LIR::BIN_OP::T_minus || aa->binop == LIR::BIN_OP::T_plus) &&
            (expEquals(aa->l, bb->r) && expEquals(aa->r, bb->r)))
        {
            return true;
        }
        return (expEquals(aa->l, bb->l) && expEquals(aa->r, bb->r));
    }
    // auto t1 = exp2int(a), t2 = exp2int(b);
    // if (t1.first && t2.first)
    //     return t1.second == t2.second;
    // if (a->exp_kind == EXP_KIND::temp && b->exp_kind == EXP_KIND::temp)
    // {
    //     return static_cast<LIR::TempVarExp *>(a)->temp_label_id_int == static_cast<LIR::TempVarExp *>(b)->temp_label_id_int;
    // }
    return false;
}

static int exp2tempid(LIR::Exp *e)
{
    if (e->exp_kind == LIR::EXP_KIND::temp)
        return static_cast<LIR::TempVarExp *>(e)->temp_label_id_int;
    return 0;
}

// {const? , value}
// static std::pair<int, int> exp2int(LIR::Exp *x)
// {
//     if (x->exp_kind == LIR::EXP_KIND::constexp)
//         return {1, static_cast<LIR::ConstExp *>(x)->value.i_val};
//     if (x->exp_kind == LIR::EXP_KIND::binop)
//     {
//         auto bi = static_cast<LIR::BinOpExp *>(x);
//         auto t1 = exp2int(bi->l), t2 = exp2int(bi->r);
//         if (t1.first && t2.first)
//         {
//             switch (bi->binop)
//             {
//             case LIR::BIN_OP::T_plus:
//                 return {1, t1.second + t2.second};
//             case LIR::BIN_OP::T_mul:
//                 return {1, t1.second * t2.second};
//             case LIR::BIN_OP::T_mod:
//                 return {1, t1.second % t2.second};
//             case LIR::BIN_OP::T_minus:
//                 return {1, t1.second - t2.second};
//             case LIR::BIN_OP::T_div:
//                 return {1, t1.second / t2.second};
//             default:
//                 return {0, 0};
//             }
//         }
//     }
//     return {0, 0};
// }

static float decode(int i)
{
    union
    {
        int i;
        float f;
    } ret;
    ret.i = i;
    return ret.f;
}
static int encode(float f)
{
    union
    {
        int i;
        float f;
    } ret;
    ret.f = f;
    return ret.i;
}
static int digit_i2f(int i) { return encode((float)i); }
static int digit_f2i(int f) { return (int)decode(f); }

static LIR::Exp *iExp2fExp(LIR::Exp *exp)
{
    if (exp->exp_kind == LIR::EXP_KIND::constexp)
        return new LIR::ConstExp(digit_i2f(static_cast<LIR::ConstExp *>(exp)->val));
    else
        return new LIR::CallExp("i2f", std::vector<LIR::Exp *>(1, exp));
}

static LIR::Exp *fExp2iExp(LIR::Exp *exp)
{
    if (exp->exp_kind == LIR::EXP_KIND::constexp)
        return new LIR::ConstExp(digit_f2i(static_cast<LIR::ConstExp *>(exp)->val));
    else
        return new LIR::CallExp("f2i", std::vector<LIR::Exp *>(1, exp));
}

static LIR::Exp *align2LetfExp(LIR::Exp *rexp, T_KIND l, T_KIND r)
{
    if (l != r)
    {
        if (l == T_KIND::INT && r == T_KIND::FLOAT)
            return fExp2iExp(rexp);
        else if ((l == T_KIND::FLOAT && r == T_KIND::INT))
            return iExp2fExp(rexp);
    }
    return rexp;
}

template <typename T>
static T cal(LIR::BIN_OP op, T l, T r)
{
    switch (op)
    {
    case LIR::BIN_OP::T_plus:
        return l + r;
    case LIR::BIN_OP::T_minus:
        return l - r;
    case LIR::BIN_OP::T_mul:
        return l * r;
    case LIR::BIN_OP::T_div:
        return l / (r != (T)0 ? r : (T)1);
    default:
        assert(0);
    }
}

// Type should contains res val
static Type *BinOpResType(Type *l_type, Type *r_type, LIR::BIN_OP op)
{
    if (l_type->t_kind == T_KIND::INT && r_type->t_kind == T_KIND::INT)
    {
        int l = *l_type->var_val, r = *r_type->var_val;
        if (op == LIR::BIN_OP::T_mod)
            return newIntType(new int(l % (r != 0 ? r : 1)), false);
        else
            return newIntType(new int(cal(op, l, r)), false);
    }
    else if (l_type->t_kind == T_KIND::INT && r_type->t_kind == T_KIND::FLOAT)
    {
        float l = (float)*l_type->var_val;
        float r = decode(*r_type->var_val);
        return newFloatType(new int(encode(cal(op, l, r))), false);
    }
    else if (l_type->t_kind == T_KIND::FLOAT && r_type->t_kind == T_KIND::INT)
    {
        float l = decode(*l_type->var_val);
        float r = (float)*r_type->var_val;
        return newFloatType(new int(encode(cal(op, l, r))), false);
    }
    else if (l_type->t_kind == T_KIND::FLOAT && r_type->t_kind == T_KIND::FLOAT)
    {
        float l = decode(*l_type->var_val);
        float r = decode(*r_type->var_val);
        return newFloatType(new int(encode(cal(op, l, r))), false);
    }
    else if (l_type->t_kind == T_KIND::ARRAY && r_type->t_kind == T_KIND::INT)
    {
        return l_type;
    }
    else if (r_type->t_kind == T_KIND::ARRAY && l_type->t_kind == T_KIND::INT)
    {
        return r_type;
    }
    else
        assert(0);
}

static LIR::Exp *calLIRFloatExp(LIR::BIN_OP op, LIR::Exp *l_exp, LIR::Exp *r_exp)
{
    switch (op)
    {
    case LIR::BIN_OP::T_plus:
        return new LIR::BinOpExp(LIR::BIN_OP::F_plus, l_exp, r_exp);
    case LIR::BIN_OP::T_minus:
        return new LIR::BinOpExp(LIR::BIN_OP::F_minus, l_exp, r_exp);
    case LIR::BIN_OP::T_mul:
        return new LIR::BinOpExp(LIR::BIN_OP::F_mul, l_exp, r_exp);
    case LIR::BIN_OP::T_div:
        return new LIR::BinOpExp(LIR::BIN_OP::F_div, l_exp, r_exp);
    default:
        assert(0);
    }
}

static LIR::Exp *BinOpResExp(LIR::BIN_OP op, Type *l_type, LIR::Exp *l_exp, Type *r_type, LIR::Exp *r_exp)
{
    if (l_type->t_kind == T_KIND::INT && r_type->t_kind == T_KIND::INT)
    {
        return new LIR::BinOpExp(op, l_exp, r_exp);
    }
    else if (l_type->t_kind == T_KIND::INT && r_type->t_kind == T_KIND::FLOAT)
    {
        l_exp = iExp2fExp(l_exp);
        return calLIRFloatExp(op, l_exp, r_exp);
    }
    else if (l_type->t_kind == T_KIND::FLOAT && r_type->t_kind == T_KIND::INT)
    {
        r_exp = iExp2fExp(r_exp);
        return calLIRFloatExp(op, l_exp, r_exp);
    }
    else if (l_type->t_kind == T_KIND::FLOAT && r_type->t_kind == T_KIND::FLOAT)
    {
        return calLIRFloatExp(op, l_exp, r_exp);
    }
    else if (l_type->t_kind == T_KIND::ARRAY && r_type->t_kind == T_KIND::INT)
    {
        // l -> arr r -> int
        assert(op == LIR::BIN_OP::T_plus || op == LIR::BIN_OP::T_minus);
        return new LIR::BinOpExp(op, l_exp,
                                 new LIR::BinOpExp(LIR::BIN_OP::T_mul, r_exp, new LIR::ConstExp(l_type->array_size * 4)));
    }
    else if (r_type->t_kind == T_KIND::ARRAY && l_type->t_kind == T_KIND::INT)
    {
        // l -> int r-> arr
        assert(op == LIR::BIN_OP::T_plus);
        return new LIR::BinOpExp(LIR::BIN_OP::T_plus, r_exp,
                                 new LIR::BinOpExp(LIR::BIN_OP::T_mul, l_exp, new LIR::ConstExp(r_type->array_size * 4)));
    }
    else
        assert(0);
}

static std::vector<std::string *> *JoinAfter(const std::vector<std::string *> *l, const std::vector<std::string *> *r)
{
    auto result = new std::vector<std::string *>();
    for (const auto &s : *l)
    {
        result->push_back(s);
    }
    for (const auto &s : *r)
    {
        result->push_back(s);
    }
    return result;
}

static Type *AST_T2IR_T(AST::base_t t)
{
    switch (t)
    {
    case AST::base_t::VOID:
        return newVoidType();
    case AST::base_t::INT:
        return newIntType(new int(0), false);
    case AST::base_t::FLOAT:
        return newFloatType(new int(0), false);
    default:
        assert(0);
    }
}

static LIR::Stm *NOPStm() { return new LIR::ExpStm(new LIR::ConstExp(0)); }

static bool isLdrStm(LIR::Stm *stm)
{
    return stm->stm_kind == LIR::STM_KIND::move &&
           static_cast<LIR::MoveStm *>(stm)->src->exp_kind == LIR::EXP_KIND::mem;
}
static bool isStrStm(LIR::Stm *stm)
{
    return stm->stm_kind == LIR::STM_KIND::move &&
           static_cast<LIR::MoveStm *>(stm)->dst->exp_kind == LIR::EXP_KIND::mem;
}

static bool isCallStm(LIR::Stm *s)
{
    return (s->stm_kind == LIR::STM_KIND::move &&
            static_cast<LIR::MoveStm *>(s)->src->exp_kind == LIR::EXP_KIND::call) ||
           (s->stm_kind == LIR::STM_KIND::exp &&
            static_cast<LIR::ExpStm *>(s)->exp->exp_kind == LIR::EXP_KIND::call);
}

static std::string getCallName(LIR::Exp *callexp)
{
    assert(callexp->exp_kind == LIR::EXP_KIND::call);
    return static_cast<LIR::CallExp *>(callexp)->func_name_str;
}
static std::string getCallName(LIR::Stm *callstm)
{
    LIR::Exp *exp = nullptr;
    if (callstm->stm_kind == LIR::STM_KIND::exp)
    {
        exp = static_cast<LIR::ExpStm *>(callstm)->exp;
    }
    else if (callstm->stm_kind == LIR::STM_KIND::move)
    {
        exp = static_cast<LIR::MoveStm *>(callstm)->src;
    }
    return getCallName(exp);
}

static std::vector<LIR::Exp *> getCallParam(LIR::Exp *exp)
{
    assert(exp->exp_kind == LIR::EXP_KIND::call);
    return static_cast<LIR::CallExp *>(exp)->args;
}

static std::vector<LIR::Exp *> getCallParam(LIR::Stm *stm)
{
    LIR::Exp *exp = nullptr;
    if (stm->stm_kind == LIR::STM_KIND::exp)
        exp = static_cast<LIR::ExpStm *>(stm)->exp;
    else if (stm->stm_kind == LIR::STM_KIND::move)
        exp = static_cast<LIR::MoveStm *>(stm)->src;
    else
        assert(0);
    return getCallParam(exp);
}

static bool ismovebi(LIR::Stm *stm)
{
    return stm->stm_kind == LIR::STM_KIND::move &&
           static_cast<LIR::MoveStm *>(stm)->src->exp_kind == LIR::EXP_KIND::binop;
}

static bool isReturn(LIR::Stm *stm)
{
    return stm->stm_kind == LIR::STM_KIND::jump &&
           static_cast<LIR::JumpStm *>(stm)->jump_label_str.find("_RETURN") != -1;
}

/**
 *
 * stm1:    mov     xxx, yyy
 * stm2:    mov     r0, xxx
 * stm3:    jump    xxx_RETURN
 */
static bool isReturn(LIR::Stm *stm1, LIR::Stm *stm2, LIR::Stm *stm3)
{
    if (stm2->stm_kind != LIR::STM_KIND::move)
        return false;
    LIR::MoveStm *mv = static_cast<LIR::MoveStm *>(stm2);
    if (mv->src->exp_kind != LIR::EXP_KIND::temp || mv->dst->exp_kind != LIR::EXP_KIND::temp)
        return false;
    int dst = static_cast<LIR::TempVarExp *>(mv->dst)->temp_label_id_int;
    int src = static_cast<LIR::TempVarExp *>(mv->src)->temp_label_id_int;
    if (dst != 0)
        return false;
    if (stm1->stm_kind != LIR::STM_KIND::move)
        return false;
    LIR::MoveStm *mv1 = static_cast<LIR::MoveStm *>(stm1);
    if (mv1->dst->exp_kind != LIR::EXP_KIND::temp ||
        static_cast<LIR::TempVarExp *>(mv1->dst)->temp_label_id_int != src)
        return false;
    return isReturn(stm3);
}

#endif