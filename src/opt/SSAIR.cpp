#include "SSAIR.h"
#include "../util/cast.h"
#include <queue>

void SSA::SSAIR::gen_phi()
{
    for (int i = 0; i < this->node_count; i++)
    {
        for (auto def : node_i_origins[i])
            this->def_pos[def].push_back(i);
    }
    std::unordered_set<int> wk;
    for (auto def : this->def_pos)
    {
        for (auto node : def.second)
            wk.insert(node);
        while (!wk.empty())
        {
            int node = *wk.begin();
            wk.erase(node);
            for (auto y : dtree->DF[node])
            {
                if (this->Phi_mp[y].count(def.first) ||
                    y == this->block_done_id)
                    continue;
                std::vector<LIR::Exp *> params;
                for (int i = 0; i < in_nodes[y]->pres.size(); i++)
                {
                    params.push_back(new LIR::TempVarExp(def.first));
                }
                auto phi_func = new LIR::MoveStm(new LIR::TempVarExp(def.first),
                                                 new LIR::CallExp("$", params));
                auto sll =
                    (LIR::StmLinkedList *)this->in_nodes[y]->nodeInfo();
                sll->next = new LIR::StmLinkedList(phi_func, sll->next);
                this->Phi_mp[y].insert({def.first, sll->next});
                if (!node_i_origins[y].count(def.first))
                {
                    wk.insert(y);
                    node_i_origins[y].insert(def.first);
                }
            }
        }
    }
}

void SSA::SSAIR::rename()
{
    this->rename_i(0);
    std::vector<std::pair<int, LIR::StmLinkedList *>> phi_i;
    for (int i = 0; i < this->node_count; i++)
    {
        phi_i.clear();
        for (auto it : this->Phi_mp[i])
            phi_i.push_back(it);
        this->Phi_mp[i].clear();
        for (auto &it : phi_i)
        {
            it.first = static_cast<LIR::TempVarExp *>(*getDefExp(it.second->stm))->temp_label_id_int;
            this->Phi_mp[i].insert(it);
        }
    }
}

bool SSA::SSAIR::is_phi(LIR::Stm *stm)
{
    return stm->stm_kind == LIR::STM_KIND::move &&
           static_cast<LIR::MoveStm *>(stm)->src->exp_kind == LIR::EXP_KIND::call &&
           static_cast<LIR::CallExp *>(static_cast<LIR::MoveStm *>(stm)->src)->func_name_str[0] == '$';
}

/**
 * rename phi's params
 */
void SSA::SSAIR::rename_i(int node)
{
    auto sll = (LIR::StmLinkedList *)in_nodes[node]->nodeInfo();
    std::vector<int> rev;
    for (; sll; sll = sll->next)
    {
        auto stm = sll->stm;
        if (!is_phi(stm))
        {
            for (auto exp : findTempUse(stm))
            {
                int tid = static_cast<LIR::TempVarExp *>(*exp)->temp_label_id_int;
                if (this->tmp_var_stk[tid].empty())
                {
                    static_cast<LIR::TempVarExp *>(*exp)->temp_label_id_int = newLocalTempVarID();
                }
                else
                {
                    static_cast<LIR::TempVarExp *>(*exp)->temp_label_id_int = this->tmp_var_stk[tid].top();
                }
            }
        }
        auto dst = getDefExp(stm);
        if (dst)
        { // identify dst
            int tid = newLocalTempVarID();
            int did = static_cast<LIR::TempVarExp *>(*dst)->temp_label_id_int;
            this->tmp_var_stk[did].push(tid);
            rev.push_back(did);
            static_cast<LIR::TempVarExp *>(*dst)->temp_label_id_int = tid;
        }
    }

    for (auto succ : this->in_nodes[node]->succs)
    {
        for (auto it : this->Phi_mp[succ])
        {
            auto mv = static_cast<LIR::MoveStm *>(it.second->stm);
            auto phi_call = static_cast<LIR::CallExp *>(mv->src);

            int cnt = 0;
            for (auto pred : pre_nodes[succ])
            {
                if (pred == node)
                { // identify params
                    if (this->tmp_var_stk[it.first].empty())
                        static_cast<LIR::TempVarExp *>(phi_call->args[cnt])->temp_label_id_int = newLocalTempVarID();
                    else
                        static_cast<LIR::TempVarExp *>(phi_call->args[cnt])->temp_label_id_int = this->tmp_var_stk[it.first].top();
                    break;
                }
                cnt++;
            }
        }
    }
    for (auto succ : dtree->CH[node])
        rename_i(succ);
    for (auto var : rev)
        this->tmp_var_stk[var].pop();
}

SSA::SSAIR::SSAIR(IRBlock::BlockController bc)
    : ControlFlowGraph(bc),
      end_label(bc.done_label)
{
    this->dtree = new DT::DT(this, 0);
    this->dtree->gen_DF();

    std::cerr << std::endl
              << "before insert Phi ... " << std::endl;
    for (int i = 0; i < this->node_count; i++)
    {
        auto sll = (LIR::StmLinkedList *)(this->in_nodes[i]->info);
        std::cerr << "Block: " << i << std::endl;
        showIR(sll);
        std::cerr << std::endl;
    }
    gen_phi();

    std::cerr << std::endl
              << "after insert Phi ... " << std::endl;
    for (int i = 0; i < this->node_count; i++)
    {
        auto sll = (LIR::StmLinkedList *)(this->in_nodes[i]->info);
        std::cerr << "Block: " << i << std::endl;
        showIR(sll);
        std::cerr << std::endl;
    }
    rename();
}

IRBlock::BlockController *SSA::SSAIR::SSA2blks()
{
    for (int i = 0; i < this->node_count; i++)
    {
        if (in_nodes[i]->in_degree() <= 1)
            continue;
        if (this->Phi_mp[i].size() == 0)
            continue;
        assert(blockLabel[i]->stm->stm_kind == LIR::STM_KIND::label);

        auto label = static_cast<LIR::LabelStm *>(blockLabel[i]->stm)->label_str;
        int cnt = 0;

        for (auto &pre : this->pre_nodes[i])
        {
            assert(blockJump[pre]->stm->stm_kind != LIR::STM_KIND::cjump);

            std::unordered_map<int, int> indg;
            std::unordered_map<int, LIR::MoveStm *> mvstms;

            for (auto it : Phi_mp[i])
            {
                auto movephi = static_cast<LIR::MoveStm *>(it.second->stm);
                auto callphi = static_cast<LIR::CallExp *>(movephi->src);

                LIR::Exp *paramexp = callphi->args[cnt];
                assert(static_cast<LIR::TempVarExp *>(movephi->dst)->temp_label_id_int == it.first);

                int dsttemp = it.first;
                if (paramexp->exp_kind == LIR::EXP_KIND::temp)
                {
                    int srctemp = static_cast<LIR::TempVarExp *>(paramexp)->temp_label_id_int;
                    if (srctemp == dsttemp)
                        continue;
                    indg[srctemp] += 1;
                }
                auto *mv = new LIR::MoveStm(new LIR::TempVarExp(dsttemp), paramexp);
                mvstms.insert({dsttemp, mv});
            }
            std::queue<int> mvq;
            std::unordered_set<int> addtmp;
            for (auto it : mvstms)
            {
                if (indg[it.first] == 0)
                    mvq.push(it.first);
            }
            while (!mvstms.empty())
            {
                while (!mvq.empty())
                {
                    int dstmp = mvq.front();
                    mvq.pop();
                    auto *mvn = mvstms.at(dstmp);
                    blockJump[pre]->next = new LIR::StmLinkedList(blockJump[pre]->stm, nullptr);
                    blockJump[pre]->stm = mvn;
                    blockJump[pre] = blockJump[pre]->next;
                    if (mvn->src->exp_kind == LIR::EXP_KIND::temp)
                    {
                        int srctmp = static_cast<LIR::TempVarExp *>(mvn->src)->temp_label_id_int;
                        if (!addtmp.count(srctmp))
                        {
                            indg.at(srctmp) -= 1;
                            assert(indg.at(srctmp) >= 0);
                            if (indg.at(srctmp) == 0)
                            {
                                if (mvstms.count(srctmp))
                                {
                                    mvq.push(srctmp);
                                }
                            }
                        }
                    }
                    mvstms.erase(dstmp);
                }
                if (!mvstms.empty())
                { // has circle
                    int dsttmp = mvstms.begin()->first;
                    auto *mvn = mvstms.begin()->second;
                    assert(mvn->src->exp_kind == LIR::EXP_KIND::temp);
                    int srctmp = static_cast<LIR::TempVarExp *>(mvn->src)->temp_label_id_int;
                    assert(indg[dsttmp] == 1);
                    assert(indg[srctmp] == 1);
                    assert(mvstms.count(srctmp));
                    int temp = newLocalTempVarID();
                    addtmp.insert(temp);
                    static_cast<LIR::TempVarExp *>(mvn->src)->temp_label_id_int = temp;
                    auto *insmv = new LIR::MoveStm(new LIR::TempVarExp(temp), new LIR::TempVarExp(srctmp));
                    blockJump[pre]->next = new LIR::StmLinkedList(blockJump[pre]->stm, nullptr);
                    blockJump[pre]->stm = insmv;
                    blockJump[pre] = blockJump[pre]->next;
                    indg[srctmp] = 0;
                    mvq.push(srctmp);
                }
            }
            cnt++;
        }
        for (auto it : this->Phi_mp[i])
            it.second->stm = NOPStm();
    }

    IRBlock::BlockController *b = new IRBlock::BlockController(nullptr, this->end_label);
    BlockLinkedList *head = new BlockLinkedList(nullptr, nullptr), *tail;
    tail = head;
    for (int i = 0; i < this->node_count; i++)
    {
        if (in_nodes[i]->in_degree() == 0 && i != 0)
            continue;
        if (i == block_done_id)
            continue;
        LIR::StmLinkedList *stmlist = (LIR::StmLinkedList *)in_nodes[i]->nodeInfo();

        LIR::StmLinkedList *last = nullptr;
        for (auto list = stmlist; list; list = list->next)
        {
            if (list->stm->stm_kind == LIR::STM_KIND::exp &&
                static_cast<LIR::ExpStm *>(list->stm)->exp->exp_kind == LIR::EXP_KIND::constexp)
            {
                last->next = list->next;
                continue;
            }
            last = list;
        }

        tail = tail->next = new BlockLinkedList(stmlist, nullptr);
    }
    b->b_list = head->next;
    std::cerr << std::endl << "After SSA .." << std::endl;
    showIR(b);
    return b;
}