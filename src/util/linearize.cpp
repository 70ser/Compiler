#include "linearize.h"

/**
 * label0 --------------  | label1 -------------- | ...
 * ^                                 ^
 * |                                 |
 * BlockLinkedList                 'block'
 *
 * in/out block, a label will be created and a jump stm point to it
 *
 */
static BlockLinkedList *dispatch_block(LIR::StmLinkedList *sl, std::string done_label)
{
    assert(sl && sl->stm->stm_kind == LIR::STM_KIND::label);

    LIR::StmLinkedList *stm_head = new LIR::StmLinkedList(nullptr, nullptr);
    LIR::StmLinkedList *stm_next = stm_head;

    BlockLinkedList *label_haed = new BlockLinkedList(nullptr, nullptr);
    BlockLinkedList *label_next = label_haed;

    const int IN = 1, OUT = 2; // in/out block
    int STATE = OUT;
    while (sl)
    {
        LIR::Stm *stm = sl->stm;
        switch (STATE)
        {
        case OUT:
        {
            if (stm->stm_kind == LIR::STM_KIND::label)
            { // 1.外层
                STATE = IN;
                stm_head->next = nullptr;
                stm_next = stm_head;
                stm_next = stm_next->next = new LIR::StmLinkedList(stm, nullptr);

                sl = sl->next;
                // goto IN(3. 4.)
            }
            else
            { // 2.给下一个 block 零时的加一个 label
                STATE = IN;
                stm_head->next = nullptr;
                stm_next = stm_head;
                stm_next = stm_next->next = new LIR::StmLinkedList(new LIR::LabelStm(newLabel()), nullptr);
                // goto 4.
            }
        }
        break;
        case IN:
        {
            if (stm->stm_kind == LIR::STM_KIND::label)
            { // 3.遇到 donelabel/elselabel（下个 block 开始的 label）直接 jump
              // 转到 1 处理
                STATE = OUT;
                std::string lab = static_cast<LIR::LabelStm *>(stm)->label_str;
                // jump label1
                // label1
                stm_next = stm_next->next = new LIR::StmLinkedList(new LIR::JumpStm(lab), nullptr);
                label_next = label_next->next = new BlockLinkedList(stm_head->next, nullptr);
                // goto 1
            }
            else
            { // 4.在 block 中(最正常的情况)
                stm_next = stm_next->next = new LIR::StmLinkedList(stm, nullptr);
                sl = sl->next;
                // goto 3,4
                if (stm->stm_kind == LIR::STM_KIND::cjump || stm->stm_kind == LIR::STM_KIND::jump)
                {
                    STATE = OUT;
                    label_next = label_next->next = new BlockLinkedList(stm_head->next, nullptr);
                    // goto 1
                }
            }
        }
        break;
        }
    }
    if (STATE == IN)
    {
        // turn to OUT
        stm_next = stm_next->next = new LIR::StmLinkedList(new LIR::JumpStm(done_label), nullptr);
        label_next = label_next->next = new BlockLinkedList(stm_head->next, nullptr);
    }
    return label_haed->next;
}

/**
 * find exps use same tempid in stm
 */
std::vector<LIR::Exp **> findTempUse(LIR::Stm *stm)
{
    std::vector<LIR::Exp **> ans;

    auto ptmp = [&](LIR::Exp **exp)
    {
        if ((*exp)->exp_kind != LIR::EXP_KIND::temp)
            return;
        LIR::TempVarExp *e = static_cast<LIR::TempVarExp *>(*exp);
        if (e->temp_label_id_int < 1000)
            return; // real register
        ans.push_back(exp);
    };

    auto pmove = [&](LIR::Exp *&exp)
    {
        switch (exp->exp_kind)
        {
        case LIR::EXP_KIND::binop:
        {
            LIR::BinOpExp *e = static_cast<LIR::BinOpExp *>(exp);
            ptmp(&e->l);
            ptmp(&e->r);
            break;
        }
        case LIR::EXP_KIND::call:
        {
            LIR::CallExp *e = static_cast<LIR::CallExp *>(exp);
            for (auto &it : e->args)
                ptmp(&it);
            break;
        }
        case LIR::EXP_KIND::constexp:
        {
            break;
        }
        case LIR::EXP_KIND::mem:
        {
            LIR::MemExp *e = static_cast<LIR::MemExp *>(exp);
            ptmp(&e->mem_addr);
            break;
        }
        case LIR::EXP_KIND::name:
        {
            break;
        }
        case LIR::EXP_KIND::temp:
        {
            ptmp(&exp);
            break;
        }
        case LIR::EXP_KIND::stm_with_ret_exp:
            assert(0);
        }
    };

    switch (stm->stm_kind)
    {
    case LIR::STM_KIND::move:
    {
        LIR::MoveStm *mv = static_cast<LIR::MoveStm *>(stm);
        pmove(mv->src);
        if (mv->dst->exp_kind == LIR::EXP_KIND::mem)
        {
            ptmp(&static_cast<LIR::MemExp *>(mv->dst)->mem_addr);
        }
    }
    break;
    case LIR::STM_KIND::exp:
    {
        LIR::ExpStm *mv = static_cast<LIR::ExpStm *>(stm);
        if (mv->exp->exp_kind == LIR::EXP_KIND::call)
        {
            LIR::CallExp *call = static_cast<LIR::CallExp *>(mv->exp);
            for (auto &it : call->args)
                ptmp(&it);
        }
    }
    break;
    case LIR::STM_KIND::cjump:
    {
        LIR::CJumpStm *mv = static_cast<LIR::CJumpStm *>(stm);
        ptmp(&mv->l);
        ptmp(&mv->r);
    }
    break;
    case LIR::STM_KIND::label:
        break;
    case LIR::STM_KIND::jump:
        break;
    default:
        assert(0);
    }
    return ans;
}

IRBlock::BlockController *IRBlock::genBlock(std::vector<LIR::Stm *> *ir, std::string func_name)
{
    BlockController *b = new BlockController();
    b->done_label = func_name + "_RETURN";
    b->b_list = dispatch_block(toStmLinkedList(ir), b->done_label);
    return b;
}

IRBlock::BlockController *IRBlock::genBlock(LIR::StmLinkedList *ir, std::string func_name)
{
    BlockController *b = new BlockController();
    b->done_label = func_name + "_RETURN";
    b->b_list = dispatch_block(ir, b->done_label);
    return b;
}

/**
 *   condition (condition 中的条件满足的话会 jump 到 truelabel)
 *   trueLabel:  (如果条件是假的话还是会顺序往下执行)
 *     ----
 *   jump done
 *   falseLabel:
 *     ----
 *   doneLabel
 *
 *       ||
 *       ||    after reorder()
 *       ||
 *       \/
 *
 *   condition
 *   falseLabel:
 *       ----
 *   doneLabel
 *       ----
 *       ---- (stms after donelabel)
 *       ----
 *   trueLabel
 *       ----
 *   jump done
 *
 *  -------------*********--------------
 *
 *  @ref: an example could be:
 *
 *  LABELSTM:     main
 *  JUMPSTM:       _FUNC_BEGIN_main
 *
 *  LABELSTM:     _FUNC_BEGIN_main
 *  MOVESTM:      tmp1001      32
 *  CJUMPSTM:     tmp1001 lt 30
 *
 *  (TRUE:     __label__0
 *  FALSE:       __label__1)
 *
 *  LABELSTM:     __label__0
 *  MOVESTM:      tmp1002      ( tmp1001 add 10 )
 *  MOVESTM:      tmp1001      tmp1002
 *  JUMPSTM:       __label__2
 *
 *  LABELSTM:     __label__1
 *  MOVESTM:      tmp1003      ( tmp1001 sub 66 )
 *  MOVESTM:      tmp1001      tmp1003
 *  JUMPSTM:       __label__2
 *
 *  LABELSTM:     __label__2
 *  MOVESTM:      tmp0      111111
 *  JUMPSTM:       main_RETURN
 *
 *
 *
 *
 *
 *  LABELSTM:     main
 *
 *  LABELSTM:     _FUNC_BEGIN_main
 *  MOVESTM:      tmp1001      32
 *  CJUMPSTM:     tmp1001 lt 30
 *  (TRUE:     __label__0
 *  FALSE:       __label__1)
 *
 *  LABELSTM:     __label__1
 *  MOVESTM:      tmp1003      ( tmp1001 sub 66 )
 *  MOVESTM:      tmp1001      tmp1003
 *
 *  LABELSTM:     __label__2
 *  MOVESTM:      tmp0      111111
 *  JUMPSTM:       main_RETURN
 *
 *  LABELSTM:     __label__0
 *  MOVESTM:      tmp1002      ( tmp1001 add 10 )
 *  MOVESTM:      tmp1001      tmp1002
 *  JUMPSTM:       __label__2
 *
 *  LABELSTM:     main_RETURN
 */
LIR::StmLinkedList *IRBlock::reorder(IRBlock::BlockController *b)
{

    std::cerr << std::endl;
    std::cerr << std::endl;
    std::cerr << "after funcinline() and dispatch block !!! " << std::endl;
    std::cerr << std::endl;
    showIR(b);

    std::cerr << std::endl;
    std::cerr << std::endl;

    BlockLinkedList *b_l;
    std::unordered_map<std::string, LIR::StmLinkedList *> label_start;
    for (b_l = b->b_list; b_l; b_l = b_l->next)
    {
        std::string label_name = static_cast<LIR::LabelStm *>(b_l->stm_l_head->stm)->label_str;
        label_start[label_name] = b_l->stm_l_head;
    }
    LIR::StmLinkedList *head = new LIR::StmLinkedList(nullptr,
                                                      new LIR::StmLinkedList(nullptr, nullptr)),
                       *tail, *next;
    /**
     * @param: 'tail' points stm before jmp to next label (for jmp ignorence)
     * @param: 'next' points to jump to next label entry
     */
    tail = head;
    next = tail->next;

    for (b_l = b->b_list; b_l; b_l = b_l->next)
    {
        LIR::StmLinkedList *block = b_l->stm_l_head;
        LIR::Stm *label_stm = block->stm;
        assert(label_stm->stm_kind == LIR::STM_KIND::label);
        std::string label_name = static_cast<LIR::LabelStm *>(label_stm)->label_str;
        if (!label_start.count(label_name))
            continue; // already reordered
        // std::cerr << std::endl
        //           << label_name << std::endl;
        label_start.erase(label_name);

        next->next = block;
        tail = lastOfABlcok(next); // after jump to next label
        next = tail->next;         // jump to next block

        LIR::Stm *jp = next->stm;
        while (1)
        {
            // head     tail    next
            if (jp->stm_kind == LIR::STM_KIND::jump)
            {
                std::string next_label = static_cast<LIR::JumpStm *>(jp)->jump_label_str;
                // std::cerr << next_label << std::endl;
                if (!label_start.count(next_label))
                    break;
                block = label_start[next_label];
                label_start.erase(next_label);

                tail->next = block; // remove jump
                tail = lastOfABlcok(tail);
                next = tail->next;
                jp = next->stm;
            }
            else if (jp->stm_kind == LIR::STM_KIND::cjump)
            {
                std::string true_label = static_cast<LIR::CJumpStm *>(jp)->trueLabel;
                std::string false_label = static_cast<LIR::CJumpStm *>(jp)->falseLabel;

                if (label_start.count(false_label))
                {
                    block = label_start[false_label];
                    label_start.erase(false_label);
                    // previuos 'next' point to cjump stm, then it move to false block
                    // std::cerr << false_label << std::endl;
                    next->next = block;
                    tail = lastOfABlcok(tail);
                    next = tail->next; // next now point to 'jump to done stm'
                    jp = next->stm;
                }
                else if (label_start.count(true_label))
                {
                    static_cast<LIR::CJumpStm *>(jp)->trueLabel = false_label;
                    static_cast<LIR::CJumpStm *>(jp)->falseLabel = true_label;
                    LIR::RelOp op = static_cast<LIR::CJumpStm *>(jp)->op;
                    static_cast<LIR::CJumpStm *>(jp)->op = LIR::notRel(op);

                    block = label_start[true_label];
                    label_start.erase(true_label);

                    next->next = block;
                    tail = lastOfABlcok(tail);
                    next = tail->next;
                    jp = next->stm;
                }
                else
                {
                    std::string n_false_label = newLabel();
                    static_cast<LIR::CJumpStm *>(jp)->falseLabel = n_false_label;
                    next->next = new LIR::StmLinkedList(new LIR::LabelStm(n_false_label), nullptr);
                    tail = next;
                    next = tail->next;
                    next->next = new LIR::StmLinkedList(new LIR::JumpStm(false_label), nullptr);
                    tail = next;
                    next = tail->next;
                    break;
                }
            }
            else
                assert(0);
        }
    }
    next->next = new LIR::StmLinkedList(new LIR::LabelStm(b->done_label), nullptr);
    return head->next->next;
}
