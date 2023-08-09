#include "funcinline.h"
#include "../util/cast.h"
#include "../util/linearize.h"

std::vector<std::string> FI::FuncInline::do_func_inline()
{
    auto ans = std::vector<std::string>();
    for (auto f : func_name)
        analyse_func(f);
    for (auto f_name : func_name)
    {
        LIR::StmLinkedList *caller_ir = func_info[f_name].ir_ll;
        LIR::StmLinkedList *head = new LIR::StmLinkedList(nullptr, nullptr);
        LIR::StmLinkedList *tail = head;
        std::vector<LIR::StmLinkedList *> stm_vec;
        for (auto l = caller_ir; l; l = l->next)
        { // maybe do in add2StmList()
            if (!is_nop(l->stm))
                stm_vec.push_back(l);
        }
        bool is_recu = false;

        for (int i = 0; i < stm_vec.size();)
        { // handle recursive
            LIR::Stm *stm = stm_vec[i]->stm;
            bool is_val_ret_stm = isCallStm(stm) &&
                                  getCallName(stm) == f_name &&
                                  i + 2 < stm_vec.size() &&
                                  isReturn(stm, stm_vec[i + 1]->stm, stm_vec[i + 2]->stm);
            bool is_void_ret_stm = isCallStm(stm) &&
                                   getCallName(stm) == f_name &&
                                   i + 1 < stm_vec.size() &&
                                   isReturn(stm_vec[i + 1]->stm);
            if (is_val_ret_stm || is_void_ret_stm)
            { // recursive return
                int cnt = 0;
                auto para_list = getCallParam(stm);
                for (auto ll = caller_ir; ll; ll = ll->next)
                {
                    auto _at_stm = ll->stm;
                    if (_at_stm->stm_kind == LIR::STM_KIND::label &&
                        static_cast<LIR::LabelStm *>(_at_stm)->label_str == FUNC_BEGIN + f_name)
                    { // finish all param insertion
                        break;
                    }
                    else if (_at_stm->stm_kind == LIR::STM_KIND::move)
                    {
                        auto pmv = static_cast<LIR::MoveStm *>(_at_stm);
                        if ((pmv->src->exp_kind == LIR::EXP_KIND::temp &&
                             static_cast<LIR::TempVarExp *>(pmv->src)->temp_label_id_int < 4) ||
                            pmv->src->exp_kind == LIR::EXP_KIND::mem)
                        { // do param insertion
                            int dst_id = static_cast<LIR::TempVarExp *>(pmv->dst)->temp_label_id_int;
                            tail = tail->next = new LIR::StmLinkedList(
                                new LIR::MoveStm(new LIR::TempVarExp(dst_id), para_list[cnt]),
                                nullptr);
                            cnt++;
                        }
                    }
                }
                assert(cnt == para_list.size());
                tail = tail->next = new LIR::StmLinkedList(new LIR::JumpStm(FUNC_BEGIN + f_name), nullptr);
            }
            else
            { // recursive call, maybe do in analyse()?
                if (isCallStm(stm) && getCallName(stm) == f_name)
                    is_recu = true;
                tail = tail->next = stm_vec[i];
            }

            if (is_val_ret_stm)
                i += 3;
            else if (is_void_ret_stm)
                i += 2;
            else
                i += 1;
        }

        func_info[f_name].ir_ll = head->next;
        func_info[f_name].is_recur = is_recu;
        auto fun_pos = get_call_pos(f_name);
        for (auto f_p : fun_pos)
        {
            // TODO: do inline
            std::string callee_name = f_p.first;
            auto ill = func_info[callee_name].ir_ll;
            for (; ill; ill = ill->next)
            {
                if (ill->stm->stm_kind == LIR::STM_KIND::label &&
                    static_cast<LIR::LabelStm *>(ill->stm)->label_str == FUNC_BEGIN + callee_name)
                    break;
            }
            std::unordered_map<int, int> tmp_var_mp;               // caller's tmp var -> callee's in-func tmp var
            std::unordered_map<std::string, std::string> label_mp; // caller's label -> callee's in-func label
            assert(ill);
            // do empty pram copy
            ill = ill->copy_inline(global_var,
                                   -func_info[f_name].stack_size + _BASE_STK_SIZE,
                                   tmp_var_mp,
                                   label_mp);
            LIR::StmLinkedList *in_call_param = new LIR::StmLinkedList(nullptr, nullptr), *tail;
            tail = in_call_param;
            int cnt = 0;
            auto func_call_stm = f_p.second->stm;
            auto para_list = getCallParam(func_call_stm);
            for (auto ll = func_info[callee_name].ir_ll; ll; ll = ll->next)
            {
                auto _at_stm = ll->stm;
                if (_at_stm->stm_kind == LIR::STM_KIND::label &&
                    static_cast<LIR::LabelStm *>(_at_stm)->label_str == FUNC_BEGIN + callee_name)
                { // finish all param insertion
                    break;
                }
                else if (_at_stm->stm_kind == LIR::STM_KIND::move)
                {
                    auto pmv = static_cast<LIR::MoveStm *>(_at_stm);
                    if ((pmv->src->exp_kind == LIR::EXP_KIND::temp &&
                         static_cast<LIR::TempVarExp *>(pmv->src)->temp_label_id_int < 4) ||
                        pmv->src->exp_kind == LIR::EXP_KIND::mem)
                    { // do param insertion
                        int dst_id = static_cast<LIR::TempVarExp *>(pmv->dst)->temp_label_id_int;
                        // count in tmp param map
                        if (tmp_var_mp.count(dst_id))
                        {
                            tail = tail->next = new LIR::StmLinkedList(
                                new LIR::MoveStm(new LIR::TempVarExp(tmp_var_mp.at(dst_id)), para_list[cnt]),
                                nullptr);
                        }
                        cnt++;
                    }
                }
            }
            assert(cnt == para_list.size());
            tail->next = ill;
            tail = endOfABlcok(ill);
            std::string exit_label = label_mp.count(callee_name + "_RETURN")
                                         ? label_mp.at(callee_name + "_RETURN")
                                         : newLabel();
            tail = tail->next = new LIR::StmLinkedList(new LIR::LabelStm(exit_label), nullptr);
            if (func_call_stm->stm_kind == LIR::STM_KIND::move)
            {
                int ret_tmp_var_label_int = tmp_var_mp.count(0)
                                                ? tmp_var_mp.at(0)
                                                : newLocalTempVarID();
                tail = tail->next = new LIR::StmLinkedList(new LIR::MoveStm(static_cast<LIR::MoveStm *>(func_call_stm)->dst,
                                                                            new LIR::TempVarExp(ret_tmp_var_label_int)),
                                                           nullptr);
            }
            tail->next = f_p.second->next;
            f_p.second->next = in_call_param->next;
            f_p.second->stm = NOPStm();
            func_info[f_name].stack_size += func_info[callee_name].stack_size - _BASE_STK_SIZE;
        }
    }

    if (global2local())
    { // just main !!!
        ans.push_back("main");
    }
    else
    {
        for (auto f_name : func_name)
            if (func_info[f_name].called_num != 0)
                ans.push_back(f_name);
    }
    return ans;
}

void FI::FuncInline::analyse_func(std::string func_name)
{
    assert(func_map.count(func_name));

    LIR::StmLinkedList *sll = func_map[func_name];

    func_info[func_name].is_void =
        func_table->findEntry(func_name)->type->t_kind == T_KIND::VOID;
    func_info[func_name].is_recur = false;

    func_info[func_name].stack_size = func_table->findEntry(func_name)->stack_size;
    func_info[func_name].ir_ll = sll;
    func_info[func_name].code_len = 0;
    if (func_name == "main")
        func_info[func_name].called_num = 1;

    for (; sll; sll = sll->next)
    {
        func_info[func_name].code_len += 1;
        LIR::Stm *stm = sll->stm;
        if (isCallStm(stm))
        {
            std::string callee_name = getCallName(stm);
            if (func_name == callee_name)
                func_info[func_name].is_recur = true;
            else if (func_info.count(callee_name))
            {
                func_info[func_name].call_num += 1;
                func_info[func_name].pos_called.push_back({callee_name, sll});
                func_info[callee_name].called_num += 1;
                for (std::string gv_in_callee : func_info[callee_name].global_val_uesd)
                    func_info[func_name].global_val_uesd.insert(gv_in_callee);
            }
        }
        else if (isStrStm(stm))
        {
            // mark global var
            auto exp = static_cast<LIR::MemExp *>(static_cast<LIR::MoveStm *>(stm)->dst)->mem_addr;
            if (exp->exp_kind == LIR::EXP_KIND::name)
                func_info[func_name].global_val_uesd.insert(static_cast<LIR::NameExp *>(exp)->name);
        }
        else if (isLdrStm(stm))
        {
            // mark global var
            auto exp = static_cast<LIR::MemExp *>(static_cast<LIR::MoveStm *>(stm)->src)->mem_addr;
            if (exp->exp_kind == LIR::EXP_KIND::name)
                func_info[func_name].global_val_uesd.insert(static_cast<LIR::NameExp *>(exp)->name);
        }
    }
}

std::vector<std::pair<std::string, LIR::StmLinkedList *>>
FI::FuncInline::get_call_pos(std::string func_name)
{
    auto ans = std::vector<std::pair<std::string, LIR::StmLinkedList *>>();
    int _stk_size_add = this->func_info[func_name].stack_size;
    for (auto it : this->func_info[func_name].pos_called)
    {
        std::string f_name = it.first;
        if (func_info[f_name].is_recur)
            continue;
        if (!func_info[func_name].is_recur &&
            _stk_size_add + (func_info[f_name].stack_size - _BASE_STK_SIZE) < _MAX_STK_SIZE)
        {
            ans.push_back(it);
            _stk_size_add += (func_info[f_name].stack_size - _BASE_STK_SIZE);
        }
        else if (func_info[func_name].is_recur &&
                 _stk_size_add + (func_info[f_name].stack_size - _BASE_STK_SIZE) < _MAX_STK_GROWTH_MULTI * func_info[func_name].stack_size &&
                 func_info[f_name].code_len < func_info[func_name].code_len)
        {
            ans.push_back(it);
            _stk_size_add += (func_info[f_name].stack_size - _BASE_STK_SIZE);
        }
    }
    return ans;
}

/**
 * considering main function:
 *      change global var(other funcs don't use / only use in main) to main's local var
 */
bool FI::FuncInline::global2local()
{
    std::unordered_map<std::string, int> nameMap;
    for (auto it = var_table->entryMap.begin(); it != var_table->entryMap.end(); it++)
    {
        Entry *entry = var_table->findEntry(it->first);
        assert(entry && entry->kind == EntryKind::GLOBAL);
        if (entry->type->is_const)
        {
            assert(entry->type->t_kind != T_KIND::ARRAY);
            continue; // const
        }
        if (entry->type->t_kind == T_KIND::ARRAY)
            continue;
        std::string name = static_cast<GlobalvarEntry *>(entry)->label;
        nameMap.insert({name, newLocalTempVarID()});
    }
    LIR::StmLinkedList *sll = func_info["main"].ir_ll;
    bool no_func_call = true;
    for (auto _at_stm = sll; _at_stm; _at_stm = _at_stm->next)
    {
        auto _this_stm = _at_stm->stm;
        if (isCallStm(_this_stm) && func_info.count(getCallName(_this_stm)))
        {
            no_func_call = false;
            std::string callee_name = getCallName(_this_stm);
            for (auto it : func_info[callee_name].global_val_uesd)
                nameMap.erase(it);
        }
    }
    for (auto _at_stm = sll; _at_stm; _at_stm = _at_stm->next)
    {
        auto _this_stm = _at_stm->stm;
        if (_this_stm->stm_kind != LIR::STM_KIND::move)
            continue;
        auto mv = static_cast<LIR::MoveStm *>(_this_stm);
        if (isLdrStm(_this_stm))
        {
            LIR::MemExp *mm = static_cast<LIR::MemExp *>(mv->src);
            if (mm->mem_addr->exp_kind == LIR::EXP_KIND::name)
            {
                std::string gv_name = static_cast<LIR::NameExp *>(mm->mem_addr)->name;
                if (nameMap.count(gv_name))
                {
                    mv->src = new LIR::TempVarExp(nameMap[gv_name]);
                }
            }
        }
        else if (isStrStm(_this_stm))
        {
            LIR::MemExp *mm = static_cast<LIR::MemExp *>(mv->dst);
            if (mm->mem_addr->exp_kind == LIR::EXP_KIND::name)
            {
                std::string gv_name = static_cast<LIR::NameExp *>(mm->mem_addr)->name;
                if (nameMap.count(gv_name))
                {
                    mv->dst = new LIR::TempVarExp(nameMap[gv_name]);
                }
            }
        }
    }
    return no_func_call;
}