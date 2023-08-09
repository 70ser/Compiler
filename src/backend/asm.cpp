#include "../structure/LIR.h"
#include "../structure/instruction.h"
#include "../util/linearize.h"
#include "../util/cast.h"

#include <unordered_map>
#include <set>

void LIR::StmList::ir2asm(std::vector<ASM::Instruction *> *l)
{
    // no stmlist
    assert(0);
}

void LIR::LabelStm::ir2asm(std::vector<ASM::Instruction *> *l)
{
    l->push_back(new ASM::Label(this->label_str));
}

void LIR::JumpStm::ir2asm(std::vector<ASM::Instruction *> *l)
{
    l->push_back(new ASM::Operand(std::string("b ") + this->jump_label_str, {}, {}, {this->jump_label_str}));
}

void LIR::CJumpStm::ir2asm(std::vector<ASM::Instruction *> *l)
{
    if (op == LIR::RelOp::F_eq || op == LIR::RelOp::F_ge || op == LIR::RelOp::F_gt ||
        op == LIR::RelOp::F_le || op == LIR::RelOp::F_lt || op == LIR::RelOp::F_ne)
    {
        int lf = this->l->ir2asm(l);
        int rt = this->r->ir2asm(l);
        if (lf > 0)
        {
            int newtp = ~newLocalTempVarID();
            l->push_back(new ASM::Operand("vmov $d0, $s0", {newtp}, {lf}, {}));
            lf = newtp;
        }
        if (rt > 0)
        {
            int newtp = ~newLocalTempVarID();
            l->push_back(new ASM::Operand("vmov $d0, $s0", {newtp}, {rt}, {}));
            rt = newtp;
        }
        l->push_back(new ASM::Operand("vcmp.f32 $s0, $s1", {}, {lf, rt}, {}));
        l->push_back(new ASM::Operand("vmrs APSR_nzcv, fpscr", {}, {}, {}));
    }
    else
    {
        auto lf = this->l->ir2asm(l);
        auto rt = this->r->ir2asm(l);
        l->push_back(new ASM::Operand(std::string("cmp $s0, $s1"), {}, {lf, rt}, {}));
    }
    std::string branch_type = "b" + relop2string(this->op);
    l->push_back(
        new ASM::Operand(branch_type + " " + this->trueLabel, {}, {}, {trueLabel, falseLabel}));
}

void LIR::MoveStm::ir2asm(std::vector<ASM::Instruction *> *l)
{
    if (this->dst->exp_kind == EXP_KIND::temp)
    {
        if (this->src->exp_kind == EXP_KIND::binop)
        { // a <- b op c
            LIR::BinOpExp *r = static_cast<LIR::BinOpExp *>(this->src);
            int l_label = static_cast<LIR::TempVarExp *>(this->dst)->temp_label_id_int;
            auto ltmp = r->l->ir2asm(l), rtmp = r->r->ir2asm(l);
            switch (r->binop)
            {
            case LIR::BIN_OP::T_plus:
            case LIR::BIN_OP::T_minus:
            case LIR::BIN_OP::T_mul:
            case LIR::BIN_OP::T_div:
            {
                l->push_back(new ASM::Operand(binop2string(r->binop) + " $d0, $s0, $s1",
                                              {l_label},
                                              {ltmp, rtmp}, {}));
            }
            break;
            case LIR::BIN_OP::T_mod:
            {
                int temp = newLocalTempVarID();
                // tmp = ltmp / rtmp
                // l_label = ltmp - (ltmp / rtmp) * rtmp
                l->push_back(
                    new ASM::Operand(std::string("sdiv $d0, $s0, $s1"), {temp}, {ltmp, rtmp}, {}));
                l->push_back(new ASM::Operand(std::string("mls $d0, $s0, $s1, $s2"),
                                              {l_label},
                                              {temp, rtmp, ltmp}, {}));
                break;
            }
            case LIR::BIN_OP::F_plus:
            case LIR::BIN_OP::F_minus:
            case LIR::BIN_OP::F_mul:
            case LIR::BIN_OP::F_div:
            {
                assert(l_label < 0);
                if (ltmp > 0)
                {
                    int newtp = ~newLocalTempVarID();
                    l->push_back(new ASM::Operand("vmov $d0, $s0", {newtp}, {ltmp}, {}));
                    ltmp = newtp;
                }
                if (rtmp > 0)
                {
                    int newtp = ~newLocalTempVarID();
                    l->push_back(new ASM::Operand("vmov $d0, $s0", {newtp}, {rtmp}, {}));
                    rtmp = newtp;
                }
                l->push_back(new ASM::Operand(binop2string(r->binop) + " $d0, $s0, $s1",
                                              {l_label},
                                              {ltmp, rtmp}, {}));
            }
            break;
            default:
                assert(0);
                break;
            }
        }
        else if (this->src->exp_kind == EXP_KIND::constexp)
        {
            int val = static_cast<LIR::ConstExp *>(this->src)->val;
            // int val = ce->val;
            auto d0 = this->dst->ir2asm(l);
            if (d0 < 0)
            {
                int tmp = newLocalTempVarID();
                l->push_back(
                    new ASM::Operand(std::string("$mov $d0, #") + std::to_string(val), {tmp}, {}, {}));
                l->push_back(new ASM::Operand(std::string("vmov $d0, $s0"), {d0}, {tmp}, {}));
            }
            else
                l->push_back(
                    new ASM::Operand(std::string("$mov $d0, #") + std::to_string(val), {d0}, {}, {}));
        }
        else if (this->src->exp_kind == EXP_KIND::name)
        {
            auto d0 = static_cast<LIR::TempVarExp *>(this->dst)->temp_label_id_int;
            assert(d0 >= 0);
            l->push_back(new ASM::Operand(std::string("$mov $d0,@") + static_cast<LIR::NameExp *>(this->src)->name, {d0}, {}, {}));
        }
        else if (this->src->exp_kind == EXP_KIND::mem)
        {
            auto d0 = (static_cast<LIR::TempVarExp *>(this->dst)->temp_label_id_int);
            assert(static_cast<LIR::MemExp *>(this->src)->mem_addr->exp_kind == EXP_KIND::temp ||
                   static_cast<LIR::MemExp *>(this->src)->mem_addr->exp_kind == EXP_KIND::name);

            auto mempos = static_cast<LIR::MemExp *>(this->src)->mem_addr->ir2asm(l);

            assert(mempos >= 0);
            if (d0 < 0)
                l->push_back(new ASM::Operand(std::string("vldr $d0, [$s0]"), {d0}, {mempos}, {}));
            else
                l->push_back(new ASM::Operand(std::string("ldr $d0, [$s0]"), {d0}, {mempos}, {}));
        }
        else if (this->src->exp_kind == EXP_KIND::temp)
        {
            auto d0 = static_cast<LIR::TempVarExp *>(this->dst)->temp_label_id_int;
            auto s0 = static_cast<LIR::TempVarExp *>(this->src)->temp_label_id_int;

            if (d0 < 0 && s0 < 0)
                l->push_back(new ASM::Move("vmov $d0, $s0", {d0}, {s0}));
            else if (d0 >= 0 && s0 >= 0)
                l->push_back(new ASM::Move("mov $d0, $s0", {d0}, {s0}));
            else
                l->push_back(new ASM::Operand("vmov $d0, $s0", {d0}, {s0}, {}));
        }
        else if (this->src->exp_kind == EXP_KIND::call)
        {
            std::string func_name = getCallName(this);
            auto d0 = static_cast<LIR::TempVarExp *>(this->dst)->temp_label_id_int;
            if (func_name == "i2f")
            {
                assert(d0 < 0);
                LIR::Exp *e0 = getCallParam(this)[0];
                if (e0->exp_kind == EXP_KIND::temp)
                {
                    int src = static_cast<LIR::TempVarExp *>(e0)->temp_label_id_int;
                    assert(src >= 0);
                    l->push_back(new ASM::Operand("vmov $d0, $s0", {d0}, {src}, {}));
                    l->push_back(new ASM::Operand("vcvt.f32.s32 $d0, $s0", {d0}, {d0}, {}));
                }
                else if (e0->exp_kind == EXP_KIND::constexp)
                {
                    // assert(static_cast<LIR::ConstExp *>(e0)->is_int_constexp);
                    int i_val = static_cast<LIR::ConstExp *>(e0)->val;
                    i_val = digit_i2f(i_val);
                    int tmp = newLocalTempVarID();
                    l->push_back(new ASM::Operand("$mov $d0,#" + std::to_string(i_val), {tmp}, {}, {}));
                    l->push_back(new ASM::Operand("vmov $d0,$s0", {d0}, {tmp}, {}));
                }
                else
                    assert(0);
            }
            else if (func_name == "f2i")
            {
                assert(d0 >= 0);
                LIR::Exp *e0 = getCallParam(this)[0];
                if (e0->exp_kind == EXP_KIND::temp)
                {
                    int src = static_cast<LIR::TempVarExp *>(e0)->temp_label_id_int;
                    assert(src < 0);
                    int tmp = ~newLocalTempVarID();
                    l->push_back(new ASM::Operand("vcvt.s32.f32 $d0, $s0", {tmp}, {src}, {}));
                    l->push_back(new ASM::Operand("vmov $d0, $s0", {d0}, {tmp}, {}));
                }
                else if (e0->exp_kind == EXP_KIND::constexp)
                {
                    // assert(!static_cast<LIR::ConstExp *>(e0)->is_int_constexp);
                    int i_val = static_cast<LIR::ConstExp *>(e0)->val;
                    i_val = digit_f2i(i_val);
                    l->push_back(new ASM::Operand("$mov $d0, #" + std::to_string(i_val), {d0}, {}, {}));
                }
                else
                    assert(0);
            }
            else
            {
                auto s0 = this->src->ir2asm(l);
                assert(s0 == 0);
                if (d0 < 0)
                    l->push_back(new ASM::Operand(std::string("vmov $d0, $s0"), {d0}, {s0}, {}));
                else
                    l->push_back(new ASM::Move(std::string("mov $d0, $s0"), {d0}, {s0}));
            }
        }
    }
    else if (this->dst->exp_kind == EXP_KIND::mem)
    { // move mem[], const/name/tmp
        auto s1 = this->src->ir2asm(l);
        auto s2 = static_cast<LIR::MemExp *>(this->dst)->mem_addr->ir2asm(l);
        if (s1 < 0) // float store
            l->push_back(new ASM::Operand(std::string("vstr $s0, [$s1]"), {}, {s1, s2}, {}));
        else
            l->push_back(new ASM::Operand(std::string("str $s0, [$s1]"), {}, {s1, s2}, {}));
    }
    else
        assert(0);
}

void LIR::ExpStm::ir2asm(std::vector<ASM::Instruction *> *l)
{
    if (this->exp->exp_kind == EXP_KIND::call)
        this->exp->ir2asm(l);
}

int LIR::ConstExp::ir2asm(std::vector<ASM::Instruction *> *l)
{
    int dst = newLocalTempVarID();
    // if (this->is_int_constexp)
    l->push_back(new ASM::Operand(std::string("$mov $d0, #") + std::to_string(this->val), {dst}, {}, {}));
    // else
    //     // armv7 不支持浮点立即数 122行也是
    //     l->push_back(new ASM::Operand(std::string("$mov $d0, #") + std::to_string(int(this->val)), {dst}, {}, {}));
    return dst;
}

int LIR::BinOpExp::ir2asm(std::vector<ASM::Instruction *> *l)
{
    assert(0);
    return 0;
}

int LIR::TempVarExp::ir2asm(std::vector<ASM::Instruction *> *l)
{
    return this->temp_label_id_int;
}

int LIR::MemExp::ir2asm(std::vector<ASM::Instruction *> *l)
{
    assert(0);
    return 0;
}

int LIR::StmWithRetExp::ir2asm(std::vector<ASM::Instruction *> *l)
{
    assert(0);
    return 0;
}

int LIR::NameExp::ir2asm(std::vector<ASM::Instruction *> *l)
{
    auto d0 = newLocalTempVarID();
    l->push_back(new ASM::Operand(std::string("$mov $d0,@") + this->name, {d0}, {}, {}));
    return d0;
}

int LIR::CallExp::ir2asm(std::vector<ASM::Instruction *> *l)
{
    int stksize = 0, cnt = 0;
    ;
    assert(this->func_name_str != "f2i" && this->func_name_str != "i2f");
    LIR::StmList *sl = new LIR::StmList();
    for (auto it : this->args)
    {
        if (cnt < 4)
        {
            sl->add2StmList(new LIR::MoveStm(new LIR::TempVarExp(cnt), it));
            cnt++;
        }
        else
        {
            int tmp_id = newLocalTempVarID();
            // pass by sp(R13)
            sl->add2StmList(new LIR::MoveStm(new LIR::TempVarExp(tmp_id),
                                             new LIR::BinOpExp(LIR::BIN_OP::T_plus, new LIR::TempVarExp(13), new LIR::ConstExp(stksize))));
            sl->add2StmList(new LIR::MoveStm(new LIR::MemExp(new LIR::TempVarExp(tmp_id)), it));
            stksize += 4;
        }
    }

    if (stksize)
        (new LIR::MoveStm(new LIR::TempVarExp(13),
                          new LIR::BinOpExp(LIR::BIN_OP::T_plus, new LIR::TempVarExp(13), new LIR::ConstExp(-stksize))))
            ->ir2asm(l);

    for (auto it : sl->stm_list)
        it->ir2asm(l);

    // r0 - r3
    std::vector<int> func_use_var;
    for (int i = 0; i < cnt; i++)
        func_use_var.push_back(i);

    std::vector<int> func_def_var;
    for (int i = 0; i < 32; i++)
        func_def_var.push_back(~i);
    func_def_var.push_back(0);
    func_def_var.push_back(1);
    func_def_var.push_back(2);
    func_def_var.push_back(3);
    func_def_var.push_back(14); //
    func_def_var.push_back(12);

    int f_tmp = newLocalTempVarID();
    if (this->func_name_str == "putfloat")
    {
        l->push_back(new ASM::Operand(std::string("vmov $d0, $s0"), {~0}, {0}, {}));
        func_use_var.push_back(~0);
    }
    if (this->func_name_str == "putfloat" || this->func_name_str == "putfarray")
    {
        l->push_back(new ASM::Operand(std::string("mov $d0, sp"), std::vector<int>(1, f_tmp), {}, {}));
        l->push_back(new ASM::Operand(std::string("lsr sp, sp, #4"), {}, {}, {}));
        l->push_back(new ASM::Operand(std::string("lsl sp, sp, #4"), {}, {}, {}));
    }

    l->push_back(new ASM::Operand(std::string("bl ") + this->func_name_str, func_def_var, func_use_var, {}));

    if (this->func_name_str == "putfloat" || this->func_name_str == "putfarray")
    {
        l->push_back(new ASM::Operand(std::string("mov sp, $s0"), {}, {f_tmp}, {}));
    }
    if (this->func_name_str == "getfloat")
    {
        l->push_back(new ASM::Operand(std::string("vmov $d0, $s0"), {0}, {~0}, {}));
    }

    if (stksize)
    {
        // sp(R13)
        (new LIR::MoveStm(new LIR::TempVarExp(13),
                          new LIR::BinOpExp(LIR::BIN_OP::T_plus, new LIR::TempVarExp(13), new LIR::ConstExp(stksize))))
            ->ir2asm(l);
    }
    return 0; // r0
}

// diff float tmep var with id<0
LIR::StmLinkedList *MarkFTemp(LIR::StmLinkedList *ir)
{
    std::set<int> fid_set;
    std::unordered_map<int, std::set<int>> mp;
    // build trace
    for (auto sll = ir; sll; sll = sll->next)
    {
        auto it = sll->stm;
        if (it->stm_kind == LIR::STM_KIND::move &&
            static_cast<LIR::MoveStm *>(it)->src->exp_kind == LIR::EXP_KIND::temp &&
            static_cast<LIR::MoveStm *>(it)->dst->exp_kind == LIR::EXP_KIND::temp)
        {
            int src_id = static_cast<LIR::TempVarExp *>(static_cast<LIR::MoveStm *>(it)->src)->temp_label_id_int;
            int dst_id = static_cast<LIR::TempVarExp *>(static_cast<LIR::MoveStm *>(it)->dst)->temp_label_id_int;
            if (src_id >= 1000 && dst_id >= 1000)
            { // temp var def
                mp[src_id].insert(dst_id);
                mp[dst_id].insert(src_id);
            }
        }
    }
    for (auto sll = ir; sll; sll = sll->next)
    {
        auto it = sll->stm;
        if (isCallStm(it) && getCallName(it) == "i2f")
        { // tmp(float) <- int
            auto dst = getDefExp(it);
            if (dst)
                fid_set.insert(static_cast<LIR::TempVarExp *>(*dst)->temp_label_id_int);
        }
        else if (isCallStm(it) && getCallName(it) == "f2i")
        { // tmp(int) <- float
            auto dst = findTempUse(it);
            for (auto it : dst)
                fid_set.insert(static_cast<LIR::TempVarExp *>(*it)->temp_label_id_int);
        }
        else if (ismovebi(it))
        {
            LIR::BIN_OP op = static_cast<LIR::BinOpExp *>(static_cast<LIR::MoveStm *>(it)->src)->binop;
            if (op == LIR::BIN_OP::F_plus || op == LIR::BIN_OP::F_minus ||
                op == LIR::BIN_OP::F_mul || op == LIR::BIN_OP::F_div)
            {
                auto dstp = getDefExp(it);
                if (dstp)
                    fid_set.insert(static_cast<LIR::TempVarExp *>(*dstp)->temp_label_id_int);
                auto dst = findTempUse(it);
                for (auto it : dst)
                    fid_set.insert(static_cast<LIR::TempVarExp *>(*it)->temp_label_id_int);
            }
        }
        else if (it->stm_kind == LIR::STM_KIND::jump)
        {
            LIR::RelOp op = static_cast<LIR::CJumpStm *>(it)->op;
            if (op == LIR::RelOp::F_eq || op == LIR::RelOp::F_ge ||
                op == LIR::RelOp::F_gt || op == LIR::RelOp::F_le ||
                op == LIR::RelOp::F_lt || op == LIR::RelOp::F_ne)
            {
                auto dst = findTempUse(it);
                for (auto it : dst)
                    fid_set.insert(static_cast<LIR::TempVarExp *>(*it)->temp_label_id_int);
            }
        }
    }

    // bfs
    std::stack<int> st;
    for (int i : fid_set)
        st.push(i);
    while (!st.empty())
    {
        int tp = st.top();
        st.pop();
        if (mp.count(tp))
        {
            for (int i : mp[tp])
            {
                if (!fid_set.count(i))
                {
                    fid_set.insert(i);
                    st.push(i);
                }
            }
        }
    }
    for (auto sll = ir; sll; sll = sll->next)
    {
        auto it = sll->stm;
        auto d = getDefExp(it);
        if (d)
        {
            int &dst = static_cast<LIR::TempVarExp *>(*d)->temp_label_id_int;
            if (fid_set.count(dst))
                dst = ~dst; // flaot as neg
        }
        auto v = findTempUse(it);
        for (auto it : v)
        {
            int &src = static_cast<LIR::TempVarExp *>(*it)->temp_label_id_int;
            if (fid_set.count(src))
                src = ~src;
        }
    }
    return ir;
}

int getSize(LIR::StmLinkedList *ir)
{
    int ans = 0;
    LIR::StmLinkedList *tmp = ir;
    while (tmp)
    {
        ans++;
        tmp = tmp->next;
    }
    return ans;
}

ASM::Proc *LIR::ir2asm(LIR::StmLinkedList *ir)
{
    ASM::Proc *ans = new ASM::Proc();

    int l_size = getSize(ir);

    ir = MarkFTemp(ir);

    int r_size = getSize(ir);

    // showIR(ir);
    std::unordered_map<int, int> temp_cnt;
    for (auto it = ir; it; it = it->next)
    {
        auto s = it->stm;
        auto tmps = findTempUse(s);
        for (auto tmp : tmps)
            temp_cnt[static_cast<LIR::TempVarExp *>(*tmp)->temp_label_id_int]++;
    }
    LIR::StmLinkedList *s1 = ir, *s2, *s3;

    while (s1)
    {
        s2 = s1->next;
        if (s2)
            s3 = s2->next;
        else
            s3 = 0;
        if (s2)
        {
            auto stm1 = s1->stm, stm2 = s2->stm;
            if ((stm1)->stm_kind == STM_KIND::move && (stm2)->stm_kind == STM_KIND::move)
            {
                // 1. [mem] <- tmp1  2. tmp2 <- [mem]     ---->>>>    tmp2 <- tmp1
                auto m1 = static_cast<LIR::MoveStm *>(stm1);
                auto m2 = static_cast<LIR::MoveStm *>(stm2);
                if (m1->dst->exp_kind == LIR::EXP_KIND::mem &&
                    m2->src->exp_kind == LIR::EXP_KIND::mem &&
                    expEquals(m1->dst, m2->src))
                {
                    // delete s2->stm;
                    s2->stm = new LIR::MoveStm(allocate_temp_var_for_exp(m2->dst),
                                               allocate_temp_var_for_exp(m1->src));
                }

                // 1. a <- b + c    2. d <- [a]             ---->>>>    d <- [b + c]
                if (m1->src->exp_kind == LIR::EXP_KIND::binop && m1->dst->exp_kind == LIR::EXP_KIND::temp &&
                    m2->src->exp_kind == LIR::EXP_KIND::mem && m2->dst->exp_kind == LIR::EXP_KIND::temp)
                {
                    auto mem = static_cast<LIR::MemExp *>(m2->src);
                    if (mem->mem_addr->exp_kind == EXP_KIND::temp)
                    {
                        auto dst_id2 = static_cast<LIR::TempVarExp *>(m2->dst)->temp_label_id_int;

                        auto src_id2 = static_cast<LIR::TempVarExp *>(mem->mem_addr)->temp_label_id_int;
                        auto dst_id1 = static_cast<LIR::TempVarExp *>(m1->dst)->temp_label_id_int;
                        if (dst_id1 == src_id2 && temp_cnt[src_id2] == 1 && dst_id2 >= 0)
                        {
                            auto bop = static_cast<LIR::BinOpExp *>(m1->src);
                            auto l_id = (bop->l)->ir2asm(&ans->i_list);
                            auto r_id = (bop->r)->ir2asm(&ans->i_list);
                            if (bop->binop == LIR::BIN_OP::T_plus)
                                ans->i_list.push_back(new ASM::Operand(std::string("ldr $d0, [$s0, $s1]"), {dst_id2},
                                                                       {l_id, r_id}, {}));
                            else if (bop->binop == LIR::BIN_OP::T_minus)
                                ans->i_list.push_back(new ASM::Operand(std::string("ldr $d0, [$s0, -$s1]"), {dst_id2},
                                                                       {l_id, r_id}, {}));
                            s1->stm = NOPStm();
                            s2->stm = NOPStm();
                            s1 = s3;
                            continue;
                        }
                    }
                }

                //  1. a <- b + c    2. [a] <- d             ---->>>>    [b + c]  <- d
                if (m1->src->exp_kind == LIR::EXP_KIND::binop && m1->dst->exp_kind == LIR::EXP_KIND::temp &&
                    m2->dst->exp_kind == LIR::EXP_KIND::mem)
                {
                    auto mem = static_cast<LIR::MemExp *>(m2->dst);
                    if (mem->mem_addr->exp_kind == EXP_KIND::temp)
                    {
                        auto src_id2 = (m2->src)->ir2asm(&ans->i_list);

                        auto dst_id2 = static_cast<LIR::TempVarExp *>(mem->mem_addr)->temp_label_id_int;
                        auto dst_id1 = static_cast<LIR::TempVarExp *>(m1->dst)->temp_label_id_int;
                        if (dst_id1 == dst_id2 && temp_cnt[dst_id1] == 1 && src_id2 >= 0)
                        {
                            auto bop = static_cast<LIR::BinOpExp *>(m1->src);
                            auto l_id = (bop->l)->ir2asm(&ans->i_list);
                            auto r_id = (bop->r)->ir2asm(&ans->i_list);
                            if (bop->binop == LIR::BIN_OP::T_plus)
                                ans->i_list.push_back(new ASM::Operand(std::string("str $s0, [$s1, $s2]"), {},
                                                                       {src_id2, l_id, r_id}, {}));
                            else if (bop->binop == LIR::BIN_OP::T_minus)
                                ans->i_list.push_back(new ASM::Operand(std::string("str $s0, [$s1, -$s2]"), {},
                                                                       {src_id2, l_id, r_id}, {}));
                            s1->stm = NOPStm();
                            s2->stm = NOPStm();
                            s1 = s3;
                            continue;
                        }
                    }
                }

                // mla  mls
                // 1. a <- b * c    2. d <- e '+-' a        ---->>>> mla/mls    d,  b, c, e
                if (m1->src->exp_kind == LIR::EXP_KIND::binop && m1->dst->exp_kind == LIR::EXP_KIND::temp &&
                    m2->src->exp_kind == LIR::EXP_KIND::binop && m2->dst->exp_kind == LIR::EXP_KIND::temp)
                {
                    auto bi1 = static_cast<LIR::BinOpExp *>(m1->src),
                         bi2 = static_cast<LIR::BinOpExp *>(m2->src);

                    auto tid1 = static_cast<LIR::TempVarExp *>(m1->dst)->temp_label_id_int;

                    if (bi1->binop == BIN_OP::T_mul &&
                        (bi2->binop == BIN_OP::T_plus || bi2->binop == LIR::BIN_OP::T_minus) &&
                        temp_cnt[tid1] == 1)
                    {
                        int flag = 0;
                        auto uses = findTempUse(m2);
                        for (auto it : uses)
                        {
                            if (static_cast<LIR::TempVarExp *>(*it)->temp_label_id_int == tid1)
                            {
                                flag = 1;
                                break;
                            }
                        }
                        if (flag)
                        {
                            auto tid2 = exp2tempid(bi2->l), tid3 = exp2tempid(bi2->r);
                            auto r1 = static_cast<LIR::TempVarExp *>(m2->dst)->temp_label_id_int;
                            auto r2 = bi1->l->ir2asm(&ans->i_list);
                            auto r3 = bi1->r->ir2asm(&ans->i_list);
                            int r4;
                            if (tid2 && tid2 == tid1) // 1. a <- b * c    2. d <- a '+-' e
                                r4 = bi2->r->ir2asm(&ans->i_list);
                            else if (tid3 && tid3 == tid1) // 1. a <- b * c    2. d <- e '+-' a
                                r4 = bi2->l->ir2asm(&ans->i_list);
                            if (bi2->binop == LIR::BIN_OP::T_plus)
                                ans->i_list.push_back(
                                    new ASM::Operand(std::string("mla $d0, $s0, $s1, $s2"), {r1},
                                                     {r2, r3, r4}, {}));
                            else if (bi2->binop == LIR::BIN_OP::T_minus)
                                ans->i_list.push_back(
                                    new ASM::Operand(std::string("mls $d0, $s0, $s1, $s2"), {r1},
                                                     {r2, r3, r4}, {}));
                            s1->stm = NOPStm();
                            s2->stm = NOPStm();
                            s1 = s3;
                            continue;
                        }
                    }
                }
            }
        }

        s1->stm->ir2asm(&ans->i_list);
        s1 = s1->next;
    }
    // for (auto it : ans->i_list)
    //     it->print();
    return ans;
}