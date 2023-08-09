#ifndef _LINEARIZE_H
#define _LINEARIZE_H
#include <iostream>
#include <algorithm>
#include <sstream>

#include "../structure/LIR.h"
// #include "cast.h"
#include "label.h"

// unwrapp all stm from stm (directly) or by stmList (recursion)
// static void unwrap(std::vector<LIR::Stm *> *_this, LIR::Stm *stm)
// {
//     if (stm->stm_kind == LIR::STM_KIND::stm_list)
//     {
//         for (LIR::Stm *s : static_cast<LIR::StmList *>(stm)->stm_list)
//             unwrap(_this, s);
//     }
//     else
//     {
//         _this->push_back(stm);
//     }
// }

class ExpRefList
{
public:
    LIR::Exp **head;
    ExpRefList *tail;

    ExpRefList(LIR::Exp **_head, ExpRefList *_tail)
        : head(_head), tail(_tail) {}
};

class BlockLinkedList
{
public:
    LIR::StmLinkedList *stm_l_head;
    BlockLinkedList *next;
    BlockLinkedList(LIR::StmLinkedList *_stm_l_head, BlockLinkedList *_next) : stm_l_head(_stm_l_head), next(_next) {}
};


namespace IRBlock
{

    /**
     * Block control, for func/loop/if, at least one 'out_label' should be added
     */
    class BlockController
    {
    public:
        BlockLinkedList *b_list;
        std::string done_label;

        BlockController(BlockLinkedList *_list, std::string _label) : b_list(_list), done_label(_label) {}

        BlockController() {}
    };

    BlockController *genBlock(std::vector<LIR::Stm *> *ir, std::string func_name);

    BlockController *genBlock(LIR::StmLinkedList *ir, std::string func_name);

    LIR::StmLinkedList *reorder(IRBlock::BlockController *b);
};

static LIR::Stm *insertTempVar(ExpRefList *l);

static LIR::Stm *pass(LIR::Stm *stm);

static std::vector<LIR::Stm *> *handleGlobalVar(std::ostringstream *globalVarOut, std::ostringstream *globalArrOut, EntryTable<Entry *> *vt);

static bool is_nop(LIR::Stm *s)
{
    return s->stm_kind == LIR::STM_KIND::exp &&
           (static_cast<LIR::ExpStm *>(s)->exp->exp_kind == LIR::EXP_KIND::constexp);
}

static inline bool is_ret_temp_var(LIR::Stm *s, LIR::Exp *e)
{
    if (is_nop(s))
        return true; // void
    if (e->exp_kind == LIR::EXP_KIND::name || e->exp_kind == LIR::EXP_KIND::constexp)
        return true;
    return false;
}

static ExpRefList *FuncCallExpList(LIR::Exp *exp)
{
    ExpRefList *rlist, *curr;
    curr = rlist = new ExpRefList(NULL, NULL);
    std::vector<LIR::Exp *> &args = static_cast<LIR::CallExp *>(exp)->args;
    for (auto &arg : args)
    {
        curr = curr->tail = new ExpRefList(&arg, NULL);
    }
    return rlist->tail;
}

// associate exp with a ret Stm
static LIR::StmWithRetExp assExpRetWithStm(LIR::Exp *exp)
{
    switch (exp->exp_kind)
    {
    case LIR::EXP_KIND::stm_with_ret_exp:
    {
        LIR::StmWithRetExp abab =
            assExpRetWithStm(static_cast<LIR::StmWithRetExp *>(exp)->ret_exp);
        LIR::StmList *sl = new LIR::StmList();
        sl->add2StmList(pass(static_cast<LIR::StmWithRetExp *>(exp)->stm));
        sl->add2StmList(abab.stm);
        return LIR::StmWithRetExp(abab.ret_exp, sl);
    }
    break;
    case LIR::EXP_KIND::binop:
    {
        return LIR::StmWithRetExp(exp, insertTempVar(
                                           new ExpRefList(&static_cast<LIR::BinOpExp *>(exp)->l,
                                                          new ExpRefList(&static_cast<LIR::BinOpExp *>(exp)->r,
                                                                         NULL))));
    }
    break;
    case LIR::EXP_KIND::mem:
    {
        return LIR::StmWithRetExp(exp, insertTempVar(
                                           new ExpRefList(&static_cast<LIR::MemExp *>(exp)->mem_addr, NULL)));
    }
    case LIR::EXP_KIND::call:
    {
        return LIR::StmWithRetExp(exp, insertTempVar(FuncCallExpList(exp)));
    }
    default:
        return LIR::StmWithRetExp(exp, insertTempVar(NULL));
    }
}

// insert tmp var in stm to store Exp
static LIR::Stm *insertTempVar(ExpRefList *l)
{
    if (!l)
        return new LIR::ExpStm(new LIR::ConstExp(0));
    else if ((*l->head)->exp_kind == LIR::EXP_KIND::call)
    {
        // create local tmp car to store the ret value
        int tmpID = newLocalTempVarID();
        *l->head =
            new LIR::StmWithRetExp(new LIR::TempVarExp(tmpID),
                                   new LIR::MoveStm(new LIR::TempVarExp(tmpID), *l->head));
        return insertTempVar(l);
    }
    else
    {
        LIR::StmWithRetExp hd = assExpRetWithStm(*l->head);
        LIR::Stm *s = insertTempVar(l->tail);
        if (is_ret_temp_var(s, hd.ret_exp))
        {
            *l->head = hd.ret_exp;
            if (is_nop(hd.stm))
                return s;
            if (is_nop(s))
                return hd.stm;
            // never reach here
            // spent a night to figure !!! (6.27.2023)
            LIR::StmList *ret = new LIR::StmList();
            ret->add2StmList(hd.stm);
            ret->add2StmList(s);
            return ret;
        }
        else
        {
            int tmpID = newLocalTempVarID();
            *l->head = new LIR::TempVarExp(tmpID);

            LIR::StmList *ret = new LIR::StmList();
            ret->add2StmList(hd.stm);
            ret->add2StmList(new LIR::MoveStm(new LIR::TempVarExp(tmpID), hd.ret_exp));
            ret->add2StmList(s);
            return ret;
        }
    }
    // return NOPStm();
}

// return stm / stmList with no inner 'stm_list'
static LIR::Stm *pass(LIR::Stm *stm)
{
    switch (stm->stm_kind)
    {
    case LIR::STM_KIND::stm_list:
    {
        LIR::StmList *sl = static_cast<LIR::StmList *>(stm);
        // std::vector<LIR::Stm *> *raw = new std::vector<LIR::Stm *>();
        // unwrap(raw, sl);
        LIR::StmList *s_ret = new LIR::StmList();
        for (LIR::Stm *it : sl->stm_list)
            s_ret->add2StmList(pass(it));
        return s_ret;
    }
    break;
    case LIR::STM_KIND::jump:
    {
        LIR::JumpStm *jp = static_cast<LIR::JumpStm *>(stm);
        return jp;
    }
    break;
    case LIR::STM_KIND::cjump:
    {
        LIR::CJumpStm *cjp = static_cast<LIR::CJumpStm *>(stm);
        LIR::StmList *ans = new LIR::StmList();
        LIR::Stm *ord = insertTempVar(new ExpRefList(&cjp->l, new ExpRefList(&cjp->r, NULL)));
        ans->add2StmList(ord);
        ans->add2StmList(stm);
        return ans;
    }
    break;
    case LIR::STM_KIND::exp:
    {
        if (static_cast<LIR::ExpStm *>(stm)->exp->exp_kind == LIR::EXP_KIND::call)
        {
            LIR::Stm *para_in_temp = insertTempVar(FuncCallExpList(static_cast<LIR::ExpStm *>(stm)->exp));
            LIR::StmList *ret = new LIR::StmList();
            ret->add2StmList(para_in_temp);
            ret->add2StmList(stm);
            return ret;
        }
        else
        {
            LIR::Stm *temp = insertTempVar(new ExpRefList(&static_cast<LIR::ExpStm *>(stm)->exp, NULL));
            LIR::StmList *ret = new LIR::StmList();
            ret->add2StmList(temp);
            ret->add2StmList(stm);
            return ret;
        }
    }
    case LIR::STM_KIND::move:
    {
        LIR::MoveStm *s = static_cast<LIR::MoveStm *>(stm);
        if ((s->dst->exp_kind == LIR::EXP_KIND::temp || s->dst->exp_kind == LIR::EXP_KIND::name) && s->src->exp_kind == LIR::EXP_KIND::call)
        {
            LIR::Stm *ss = insertTempVar(FuncCallExpList(s->src));
            LIR::StmList *ret = new LIR::StmList();
            ret->add2StmList(ss);
            ret->add2StmList(stm);
            return ret;
        }
        else if (s->dst->exp_kind == LIR::EXP_KIND::temp || s->dst->exp_kind == LIR::EXP_KIND::name)
        {
            LIR::Stm *ss = insertTempVar(new ExpRefList(&s->src, NULL));
            LIR::StmList *ret = new LIR::StmList();
            ret->add2StmList(ss);
            ret->add2StmList(stm);
            return ret;
        }
        else if (s->dst->exp_kind == LIR::EXP_KIND::mem)
        {
            LIR::Stm *ss = insertTempVar(new ExpRefList(&static_cast<LIR::MemExp *>(s->dst)->mem_addr,
                                                        new ExpRefList(&s->src, NULL)));
            LIR::StmList *ret = new LIR::StmList();
            ret->add2StmList(ss);
            ret->add2StmList(stm);
            return ret;
        }
        else if (s->dst->exp_kind == LIR::EXP_KIND::stm_with_ret_exp)
        {
            LIR::Stm *ss = static_cast<LIR::StmWithRetExp *>(s->dst)->stm;
            s->dst = static_cast<LIR::StmWithRetExp *>(s->dst)->ret_exp;
            LIR::StmList *ret = new LIR::StmList();
            ret->add2StmList(ss);
            ret->add2StmList(stm);
            return pass(ret);
        }
        else
            assert(0);
    }
    default:
        break;
    }
    return stm;
}

/**
 * @param: si
 * @note: 'si' must be LIR::StmLis, for function's accept() return
 *       after it, no StmList or StmWithRetExp(LIR::Exp)
 * */
static std::vector<LIR::Stm *> *Linearize(LIR::Stm *si)
{
    std::vector<LIR::Stm *> *ans = new std::vector<LIR::Stm *>();
    std::stack<LIR::Stm *> st;
    st.push(pass(si));
    while (!st.empty())
    {
        LIR::Stm *tp = st.top();
        st.pop();
        if (tp->stm_kind == LIR::STM_KIND::stm_list)
        {
            for (LIR::Stm *it : static_cast<LIR::StmList *>(tp)->stm_list)
            {
                st.push(it);
            }
        }
        else
        {
            ans->push_back(tp);
        }
    }
    std::reverse(ans->begin(), ans->end());
    return ans;
}

static LIR::Exp *allocate_temp_var_for_exp(LIR::Exp *e)
{
    switch (e->exp_kind)
    {
    case LIR::EXP_KIND::constexp:
    {
        // LIR::ConstExp *exp = static_cast<LIR::ConstExp *>(e);
        // if (static_cast<LIR::ConstExp *>(e)->is_int_constexp)
        return new LIR::ConstExp(static_cast<LIR::ConstExp *>(e)->val);
        // else
        // return new LIR::ConstExp(static_cast<LIR::ConstExp *>(e)->value.f_val);
    }
    break;
    case LIR::EXP_KIND::binop:
    {
        // LIR::BinOpExp *exp = static_cast<LIR::BinOpExp *>(e);
        int tid = newLocalTempVarID();
        LIR::MoveStm *mv1 = new LIR::MoveStm(new LIR::TempVarExp(tid),
                                             new LIR::BinOpExp(static_cast<LIR::BinOpExp *>(e)->binop,
                                                               allocate_temp_var_for_exp(static_cast<LIR::BinOpExp *>(e)->l),
                                                               allocate_temp_var_for_exp(static_cast<LIR::BinOpExp *>(e)->r)));
        return new LIR::StmWithRetExp(new LIR::TempVarExp(tid), mv1);
    }
    break;
    case LIR::EXP_KIND::temp:
    {
        return new LIR::TempVarExp(static_cast<LIR::TempVarExp *>(e)->temp_label_id_int);
    }
    break;
    case LIR::EXP_KIND::mem:
    {
        // LIR::MemExp *exp = static_cast<LIR::MemExp *>(e);
        int tid = newLocalTempVarID();
        LIR::MoveStm *mv1 = new LIR::MoveStm(new LIR::TempVarExp(tid),
                                             new LIR::MemExp(
                                                 allocate_temp_var_for_exp(static_cast<LIR::MemExp *>(e)->mem_addr)));
        return new LIR::StmWithRetExp(new LIR::TempVarExp(tid), mv1);
    }
    break;
    case LIR::EXP_KIND::name:
    {
        return new LIR::NameExp(static_cast<LIR::NameExp *>(e)->name);
    }
    break;
    case LIR::EXP_KIND::call:
    {
        // LIR::CallExp *exp = static_cast<LIR::CallExp *>(e);
        std::vector<LIR::Exp *> func_args;
        for (LIR::Exp *pi : static_cast<LIR::CallExp *>(e)->args)
            func_args.push_back(allocate_temp_var_for_exp(pi));
        int tid = newLocalTempVarID();
        LIR::MoveStm *mv1 = new LIR::MoveStm(new LIR::TempVarExp(tid),
                                             new LIR::CallExp(static_cast<LIR::CallExp *>(e)->func_name_str, func_args));
        return new LIR::StmWithRetExp(new LIR::TempVarExp(tid), mv1);
    }
    break;
    case LIR::EXP_KIND::stm_with_ret_exp:
        assert(0); // panic
    }
    return e;
}

static LIR::Stm *allocate_temp_var(LIR::Stm *s)
{
    switch (s->stm_kind)
    {
    case LIR::STM_KIND::move:
    {
        /**
         * temp1    <-  src
         * dst      <-  temp1
         *
         * hope it will not crash 🙏
         */
        // LIR::MoveStm *stm = static_cast<LIR::MoveStm *>(s);
        if (static_cast<LIR::MoveStm *>(s)->src->exp_kind == LIR::EXP_KIND::mem &&
            static_cast<LIR::MoveStm *>(s)->dst->exp_kind == LIR::EXP_KIND::mem)
        {
            int tid = newLocalTempVarID();
            LIR::MoveStm *mv1 =
                new LIR::MoveStm(new LIR::TempVarExp(tid),
                                 new LIR::MemExp(
                                     allocate_temp_var_for_exp(
                                         static_cast<LIR::MemExp *>(static_cast<LIR::MoveStm *>(s)->src)->mem_addr)));
            LIR::MoveStm *mv2 =
                new LIR::MoveStm(new LIR::MemExp(
                                     allocate_temp_var_for_exp(
                                         static_cast<LIR::MemExp *>(static_cast<LIR::MoveStm *>(s)->dst)->mem_addr)),
                                 new LIR::TempVarExp(tid));
            LIR::StmList *ans = new LIR::StmList();
            ans->add2StmList(mv1);
            ans->add2StmList(mv2);
            return ans;
        }
        else if (static_cast<LIR::MoveStm *>(s)->dst->exp_kind == LIR::EXP_KIND::mem)
        {
            return new LIR::MoveStm(new LIR::MemExp(
                                        allocate_temp_var_for_exp(
                                            static_cast<LIR::MemExp *>(static_cast<LIR::MoveStm *>(s)->dst)->mem_addr)),
                                    allocate_temp_var_for_exp(static_cast<LIR::MoveStm *>(s)->src));
        }
        else if (static_cast<LIR::MoveStm *>(s)->src->exp_kind == LIR::EXP_KIND::mem)
        {
            return new LIR::MoveStm(allocate_temp_var_for_exp(static_cast<LIR::MoveStm *>(s)->dst),
                                    new LIR::MemExp(
                                        allocate_temp_var_for_exp(
                                            static_cast<LIR::MemExp *>(static_cast<LIR::MoveStm *>(s)->src)->mem_addr)));
        }
        return new LIR::MoveStm(allocate_temp_var_for_exp(static_cast<LIR::MoveStm *>(s)->dst),
                                allocate_temp_var_for_exp(static_cast<LIR::MoveStm *>(s)->src));
    }
    break;
    case LIR::STM_KIND::label:
    {
        return new LIR::LabelStm(static_cast<LIR::LabelStm *>(s)->label_str);
    }
    break;
    case LIR::STM_KIND::jump:
    {
        return new LIR::JumpStm(static_cast<LIR::JumpStm *>(s)->jump_label_str);
    }
    break;
    case LIR::STM_KIND::cjump:
    {
        // LIR::CJumpStm *stm = static_cast<LIR::CJumpStm *>(s);
        return new LIR::CJumpStm(static_cast<LIR::CJumpStm *>(s)->op,
                                 allocate_temp_var_for_exp(static_cast<LIR::CJumpStm *>(s)->l),
                                 allocate_temp_var_for_exp(static_cast<LIR::CJumpStm *>(s)->r),
                                 static_cast<LIR::CJumpStm *>(s)->trueLabel,
                                 static_cast<LIR::CJumpStm *>(s)->falseLabel);
    }
    break;
    case LIR::STM_KIND::exp:
    {
        return new LIR::ExpStm(allocate_temp_var_for_exp(static_cast<LIR::ExpStm *>(s)->exp));
    }
    break;
    default:
        assert(0);
    }
}

static LIR::Stm *r_allocate_temp_var(std::vector<LIR::Stm *> *s_v)
{
    LIR::StmList *ret = new LIR::StmList();
    for (LIR::Stm *s : *s_v)
    {
        ret->add2StmList(allocate_temp_var(s));
    }
    return ret;
}

// static std::vector<LIR::Stm *> *d_linearize(LIR::Stm *si)
// { // insane !!!
//     std::vector<LIR::Stm *> *r1 = Linearize(si);
//     LIR::Stm *inner = r_allocate_temp_var(r1);
//     return Linearize(inner);
//     // return Linearize(si);
// }

static void showIR(std::vector<LIR::Stm *> *u)
{
#ifdef DEBUG
    for (const auto &ui : *u)
        ui->printIR();
#endif
}

static void showIR(LIR::StmLinkedList *sll)
{
#ifdef DEBUG
    for (auto l = sll; l; l = l->next)
        l->stm->printIR();
#endif
}

static void showIR(IRBlock::BlockController *b){
#ifdef DEBUG
    for (BlockLinkedList *bb = b->b_list; bb; bb = bb->next)
    {
        for (LIR::StmLinkedList *l = bb->stm_l_head; l; l = l->next)
        {
            l->stm->printIR();
        }
    }
#endif
}


static std::vector<LIR::Stm *> *handleGlobalVar(std::ostringstream *globalVarOut,
                                                std::ostringstream *globalArrOut,
                                                EntryTable<Entry *> *vt)
{
    *globalVarOut << ".data\n";
    *globalArrOut << ".bss\n";
    LIR::StmList *sl = new LIR::StmList();
    for (const auto &it : vt->entryMap)
    {
        Entry *var = vt->findEntry(it.first);
        assert(var && var->kind == EntryKind::GLOBAL);
        if (var->type->is_const)
        {
            assert(var->type->t_kind != T_KIND::ARRAY);
            continue;
        }
        std::string varname = static_cast<GlobalvarEntry *>(var)->label;
        switch (var->type->t_kind)
        {
        case T_KIND::INT:
        {
            *globalVarOut
                << varname + ":\n" + ".word " + std::to_string(*var->type->var_val)
                << std::endl;
            sl->add2StmList(new LIR::MoveStm(new LIR::MemExp(new LIR::NameExp(varname)), new LIR::ConstExp(*var->type->var_val)));
        }
        break;
        case T_KIND::FLOAT:
        {
            // *globalVarOut
            //     << varname + ":\n" + ".word " + std::to_string(var->type->var_val.f_val)
            //     << std::endl;
            // sl->add2StmList(new LIR::MoveStm(new LIR::MemExp(new LIR::NameExp(varname)), new LIR::ConstExp(var->type->var_val.f_val)));
            *globalVarOut
                << varname + ":\n" + ".word " + std::to_string(*var->type->var_val)
                << std::endl;
            sl->add2StmList(new LIR::MoveStm(new LIR::MemExp(new LIR::NameExp(varname)), new LIR::ConstExp(*var->type->var_val)));
        }
        break;
        case T_KIND::ARRAY:
        {
            *globalArrOut << varname + ":\n";
            *globalArrOut << ".space " + std::to_string(var->type->array_size * 4) << std::endl;
            // memset as n-0 arr
            /**
             * example: int a[3];
             *
             *  MOVESTM:        t1004    2
             *  LABELSTM:       AaBbcCL4
             *  MOVESTM:        t1006    ( 4 mul t1004 )
             *  MOVESTM:        t1005    ( AaBbcCL3 add t1006 )
             *  MOVESTM:        Mem( t1005 )    0
             *  MOVESTM:        t1007    ( t1004 sub 1 )
             *  MOVESTM:        t1004    t1007
             *  CJUMPSTM:       t1004 ge 0
             *  true:           AaBbcCL4
             *  false:          AaBbcCL5
             *  LABELSTM:       AaBbcCL5
             */

            std::string tl = newLabel(), fl = newLabel();
            int index = newLocalTempVarID();
            sl->add2StmList(new LIR::MoveStm(new LIR::TempVarExp(index),
                                             new LIR::ConstExp(var->type->array_size - 1)));
            sl->add2StmList(new LIR::LabelStm(tl));
            sl->add2StmList(new LIR::MoveStm(new LIR::MemExp(new LIR::BinOpExp(LIR::BIN_OP::T_plus,
                                                                               new LIR::NameExp(varname),
                                                                               new LIR::BinOpExp(LIR::BIN_OP::T_mul, new LIR::TempVarExp(index), new LIR::ConstExp(4)))),
                                             new LIR::ConstExp(0)));
            sl->add2StmList(new LIR::MoveStm(new LIR::TempVarExp(index),
                                             new LIR::BinOpExp(LIR::BIN_OP::T_minus, new LIR::TempVarExp(index), new LIR::ConstExp(1))));
            sl->add2StmList(new LIR::CJumpStm(LIR::RelOp::T_ge,
                                              new LIR::TempVarExp(index), new LIR::ConstExp(0), tl, fl));
            sl->add2StmList(new LIR::LabelStm(fl));

            // init arr with val
            for (int i = 0; i < var->type->array_size; i++)
            {
                if (*(var->type->var_val + i) == 0)
                    continue;
                sl->add2StmList(new LIR::MoveStm(new LIR::MemExp(new LIR::BinOpExp(
                                                     LIR::BIN_OP::T_plus, new LIR::NameExp(varname), new LIR::ConstExp(4 * i))),
                                                 new LIR::ConstExp(*(var->type->var_val + i))));
            }
        }
        break;
        // case T_KIND::FLOAT_ARRAY:
        // {
        //     *globalArrOut << varname + ":\n";
        //     *globalArrOut << ".space " + std::to_string(var->type->array_size * 4) << std::endl;
        //     // memset as n-0 arr

        //     std::string tl = newLabel(), fl = newLabel();
        //     int index = newLocalTempVarID();
        //     sl->add2StmList(new LIR::MoveStm(new LIR::TempVarExp(index),
        //                                      new LIR::ConstExp(var->type->array_size - 1)));
        //     sl->add2StmList(new LIR::LabelStm(tl));
        //     sl->add2StmList(new LIR::MoveStm(new LIR::MemExp(new LIR::BinOpExp(LIR::BIN_OP::T_plus,
        //                                                                        new LIR::NameExp(varname),
        //                                                                        new LIR::BinOpExp(LIR::BIN_OP::T_mul, new LIR::TempVarExp(index), new LIR::ConstExp(4)))),
        //                                      new LIR::ConstExp(0)));
        //     sl->add2StmList(new LIR::MoveStm(new LIR::TempVarExp(index),
        //                                      new LIR::BinOpExp(LIR::BIN_OP::T_minus, new LIR::TempVarExp(index), new LIR::ConstExp(1))));
        //     sl->add2StmList(new LIR::CJumpStm(LIR::RelOp::T_ge,
        //                                       new LIR::TempVarExp(index), new LIR::ConstExp(0), tl, fl));
        //     sl->add2StmList(new LIR::LabelStm(fl));

        //     // init arr with val
        //     for (int i = 0; i < var->type->array_size; i++)
        //     {
        //         if (*(var->type->arr_val.f_arr_p + i) == 0)
        //             continue;
        //         sl->add2StmList(new LIR::MoveStm(new LIR::MemExp(new LIR::BinOpExp(
        //                                              LIR::BIN_OP::T_plus, new LIR::NameExp(varname), new LIR::ConstExp(4 * i))),
        //                                          new LIR::ConstExp(*(var->type->arr_val.f_arr_p + i))));
        //     }
        // }
        // break;
        default:
            assert(0);
        }
    }
    return &sl->stm_list;
}

std::vector<LIR::Exp **> findTempUse(LIR::Stm *stm);

static LIR::Exp **getDefExp(LIR::Stm *stm)
{
    switch (stm->stm_kind)
    {
    case LIR::STM_KIND::move:
    {
        LIR::MoveStm *mv = static_cast<LIR::MoveStm *>(stm);
        if (mv->dst->exp_kind == LIR::EXP_KIND::temp)
        {
            LIR::TempVarExp *d = static_cast<LIR::TempVarExp *>(mv->dst);
            if (d->temp_label_id_int >= 1000)
                return &(mv->dst);
            else
                return 0;
        }
        else
            return 0;
    }
    break;
    case LIR::STM_KIND::stm_list:
        assert(0);
    case LIR::STM_KIND::exp:
    case LIR::STM_KIND::jump:
    case LIR::STM_KIND::cjump:
    case LIR::STM_KIND::label:
        return 0;
    }
    return 0;
}

static LIR::StmLinkedList *toStmLinkedList(std::vector<LIR::Stm *> *ir)
{
    LIR::StmLinkedList *ans = new LIR::StmLinkedList(nullptr, nullptr);
    LIR::StmLinkedList *ans_h = ans;
    for (LIR::Stm *s : *ir)
    {
        ans = ans->next = new LIR::StmLinkedList(s, nullptr);
    }
    return ans_h->next;
}

static std::vector<LIR::Stm *> *toStmList(LIR::StmLinkedList *slk)
{
    std::vector<LIR::Stm *> *ans = new std::vector<LIR::Stm *>();
    LIR::StmLinkedList *tmp = slk;
    while (tmp)
    {
        ans->push_back(tmp->stm);
        tmp = tmp->next;
    }
    return ans;
}

static LIR::StmLinkedList *lastOfABlcok(LIR::StmLinkedList *list)
{
    LIR::StmLinkedList *last = list;
    assert(last->next);
    while (last->next->next)
        last = last->next;
    return last;
}

static LIR::StmLinkedList *endOfABlcok(LIR::StmLinkedList *list)
{
    LIR::StmLinkedList *last = list;
    assert(last);
    while (last->next)
        last = last->next;
    return last;
}
#endif