#pragma once

#include "SSAIR.h"
#include "controldep.h"
#include <queue>
#include <functional>

static bool is_nessary(LIR::Stm *s, SSA::SSAIR *ssa_ir)
{
    switch (s->stm_kind)
    {
    case LIR::STM_KIND::move:
    {
        auto mv = static_cast<LIR::MoveStm *>(s);
        if (mv->dst->exp_kind == LIR::EXP_KIND::mem)
            return true;
        if (mv->dst->exp_kind == LIR::EXP_KIND::temp && static_cast<LIR::TempVarExp *>(mv->dst)->temp_label_id_int < 1000)
            return true;
        if (mv->src->exp_kind == LIR::EXP_KIND::call)
        {
            auto cll = static_cast<LIR::CallExp *>(mv->src);
            if (cll->func_name_str[0] == '$' ||
                cll->func_name_str == "f2i" ||
                cll->func_name_str == "i2f")
                return false;
            return true;
        }
        return false;
    }
    break;
    case LIR::STM_KIND::exp:
    { // side-effect? all func?
        return static_cast<LIR::ExpStm *>(s)->exp->exp_kind == LIR::EXP_KIND::call;
    }
    break;
    case LIR::STM_KIND::jump:
    {
        return false;
    }
    break;
    case LIR::STM_KIND::cjump:
    {
        return false;
    }
    break;
    case LIR::STM_KIND::label:
    {
        return static_cast<LIR::LabelStm *>(s)->label_str == ssa_ir->end_label;
    }
    break;
    case LIR::STM_KIND::stm_list:
    {
        assert(0);
    }
    break;
    default:
        assert(0);
    }
}

static bool is_phi(LIR::Stm *stm)
{
    return stm->stm_kind == LIR::STM_KIND::move &&
           static_cast<LIR::MoveStm *>(stm)->src->exp_kind == LIR::EXP_KIND::call &&
           static_cast<LIR::CallExp *>(static_cast<LIR::MoveStm *>(stm)->src)->func_name_str[0] == '$';
}

class DCE
{
private:
    SSA::SSAIR *ir;

    int node_sz;
    int old_parent_id;
    CDG::CDGraph *cdg;

    std::unordered_map<LIR::Stm *, int> stm_i_block;
    std::unordered_map<int, LIR::Stm *> temp_i_def;
    std::unordered_set<LIR::Stm *> active_stm;
    std::unordered_set<int> active_block;

    std::queue<LIR::Stm *> worklist;

    std::vector<std::set<int>> newpreds, newsuccs;

    void prepare()
    {
        for (const auto &it : ir->in_nodes)
        {
            auto sll = static_cast<LIR::StmLinkedList *>(it->info);
            for (; sll; sll = sll->next)
            {
                auto stm = sll->stm;
                stm_i_block[stm] = it->uid;
                auto i_def = getDefExp(stm);
                if (i_def)
                    temp_i_def[static_cast<LIR::TempVarExp *>(*i_def)->temp_label_id_int] = stm;
                if (is_nessary(stm, ir))
                {
                    active_stm.insert(stm);
                    active_block.insert(it->uid);
                    this->worklist.push(stm);
                }
            }
        }
    }

    void bfs_mark()
    {
        while (!this->worklist.empty())
        {
            auto stm = worklist.front();
            worklist.pop();
            auto uses = findTempUse(stm);
            for (auto it : uses)
            {
                auto def_stm = temp_i_def[static_cast<LIR::TempVarExp *>(*it)->temp_label_id_int];
                if (!def_stm)
                    continue;
                if (!active_stm.count(def_stm))
                {
                    active_stm.insert(def_stm);
                    active_block.insert(stm_i_block[def_stm]);
                    this->worklist.push(def_stm);
                }
            }
            auto block = ir->in_nodes[stm_i_block[stm]];
            auto label_stm = ((LIR::StmLinkedList *)(block->info))->stm;
            if (!active_stm.count(label_stm))
            {
                active_stm.insert(label_stm);
                for (auto it : cdg->cd_node[block->uid])
                {
                    auto jmp = ir->blockJump[it]->stm;
                    if (jmp->stm_kind == LIR::STM_KIND::cjump)
                    {
                        if (!active_stm.count(jmp))
                        {
                            active_stm.insert(jmp);
                            active_block.insert(it);
                            worklist.push(jmp);
                        }
                    }
                }
            }

            auto def = getDefExp(stm);
            if (def &&
                ir->Phi_mp[block->uid].count(static_cast<LIR::TempVarExp *>(*def)->temp_label_id_int))
            {
                for (auto it : block->pres)
                {
                    auto pre_label = static_cast<LIR::LabelStm *>(((LIR::StmLinkedList *)(ir->in_nodes[it]->info))->stm);
                    if (!active_stm.count(pre_label))
                    {
                        active_stm.insert(pre_label);
                        for (auto jp : cdg->cd_node[it])
                        {
                            auto jmp = ir->blockJump[jp]->stm;
                            if (jmp->stm_kind == LIR::STM_KIND::cjump)
                            {
                                if (!active_stm.count(jmp))
                                {
                                    active_stm.insert(jmp);
                                    active_block.insert(jp);
                                    worklist.push(jmp);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    GRAPH::Node *find_next_active_node(GRAPH::Node *node)
    {
        if (active_block.count(node->uid))
            return node;
        std::set<GRAPH::Node *> vis;
        vis.insert(node);
        GRAPH::Node *ans = 0;
        std::function<void(GRAPH::Node *)> dfs = [&](GRAPH::Node *cur)
        {
            if (ans)
                return;
            if (active_block.count(cur->uid))
            {
                ans = cur;
                return;
            }
            auto succs = cur->succ();
            if (!ans)
                this->old_parent_id = cur->uid;
            for (auto it = succs.begin(); it != succs.end(); ++it)
            {
                if (!vis.count(ir->in_nodes[*it]))
                {
                    vis.insert(ir->in_nodes[*it]);
                    dfs(ir->in_nodes[*it]);
                }
            }
        };
        dfs(node);
        return ans;
    }

    void do_elimilation()
    {
        std::queue<GRAPH::Node *> wkl;
        auto nodes = ir->in_nodes;
        wkl.push(nodes[0]);
        auto vis = std::set<int>();
        while (!wkl.empty())
        {
            auto cur = wkl.front();
            wkl.pop();
            if (vis.count(cur->uid))
                continue;
            vis.insert(cur->uid);
            auto sll = (LIR::StmLinkedList *)(cur->info);
            auto head = sll, tail = sll->next;
            while (tail && tail->next)
            { // erase phi
                if (!active_stm.count(tail->stm))
                {
                    if (is_phi(tail->stm))
                    {
                        ir->Phi_mp[cur->uid].erase(static_cast<LIR::TempVarExp *>(
                                                       static_cast<LIR::MoveStm *>(tail->stm)->dst)
                                                       ->temp_label_id_int);
                    }
                    auto nt = tail->next;
                    tail->next = nullptr;
                    delete tail;
                    tail = nt;
                    head->next = nt;
                }
                else
                {
                    head = head->next;
                    tail = tail->next;
                }
            }
            if (!tail)
            {
                assert(cur->uid == ir->block_done_id);
                continue;
            }
            if (active_stm.count(tail->stm))
            {
                auto cj = static_cast<LIR::CJumpStm *>(tail->stm);
                GRAPH::Node *t_node = 0, *f_node = 0;
                int t_old_id = 0, f_old_id = 0;
                assert(cur->out_degree() == 2);
                for (auto &it : cur->succs)
                {
                    auto lb = static_cast<LIR::LabelStm *>(((LIR::StmLinkedList *)(ir->in_nodes[it]->info))->stm)->label_str;
                    if (lb == cj->trueLabel)
                    {
                        this->old_parent_id = cur->uid;
                        t_node = find_next_active_node(ir->in_nodes[it]);
                        t_old_id = this->old_parent_id;
                        if (t_old_id != cur->uid)
                        {
                            cj->trueLabel = static_cast<LIR::LabelStm *>(((LIR::StmLinkedList *)(ir->in_nodes[t_old_id]->info))->stm)->label_str;
                            active_stm.insert(static_cast<LIR::LabelStm *>(((LIR::StmLinkedList *)(ir->in_nodes[t_old_id]->info))->stm));
                            newpreds[t_old_id].insert(cur->uid);
                            newsuccs[cur->uid].insert(t_old_id);
                        }
                        newpreds[t_node->uid].insert(t_old_id);
                        newsuccs[t_old_id].insert(t_node->uid);
                        wkl.push(t_node);
                    }
                    else if (lb == cj->falseLabel)
                    {
                        this->old_parent_id = cur->uid;
                        f_node = find_next_active_node(ir->in_nodes[it]);
                        f_old_id = this->old_parent_id;
                        if (f_old_id != cur->uid)
                        {
                            cj->falseLabel = static_cast<LIR::LabelStm *>(((LIR::StmLinkedList *)(ir->in_nodes[f_old_id]->info))->stm)->label_str;
                            active_stm.insert(static_cast<LIR::LabelStm *>(((LIR::StmLinkedList *)(ir->in_nodes[f_old_id]->info))->stm));
                            newpreds[f_old_id].insert(cur->uid);
                            newsuccs[cur->uid].insert(f_old_id);
                        }
                        newpreds[f_node->uid].insert(f_old_id);
                        newsuccs[f_old_id].insert(f_node->uid);
                        wkl.push(f_node);
                    }
                    else
                        assert(0);
                }
            }
            else
            {
                int old_id = 0;
                GRAPH::Node *nxt_node;
                for (auto &it : cur->succs)
                {
                    this->old_parent_id = cur->uid;
                    nxt_node = find_next_active_node(ir->in_nodes[it]);
                    old_id = this->old_parent_id;
                    if (nxt_node)
                        break;
                }
                std::string nxt_label = static_cast<LIR::LabelStm *>(((LIR::StmLinkedList *)(nxt_node->info))->stm)->label_str;
                if (old_id != cur->uid)
                {
                    nxt_label = static_cast<LIR::LabelStm *>(((LIR::StmLinkedList *)(ir->in_nodes[old_id]->info))->stm)->label_str;
                    active_stm.insert(static_cast<LIR::LabelStm *>(((LIR::StmLinkedList *)(ir->in_nodes[old_id]->info))->stm));
                    newpreds[old_id].insert(cur->uid);
                    newsuccs[cur->uid].insert(old_id);
                }
                newpreds[nxt_node->uid].insert(old_id);
                newsuccs[old_id].insert(nxt_node->uid);
                wkl.push(nxt_node);
            }
        }

        for (int i = 0; i < this->node_sz; i++)
        {
            if (newsuccs[i].size() == 1)
            {
                ir->blockJump[i]->stm = new LIR::JumpStm(static_cast<LIR::LabelStm *>(((LIR::StmLinkedList *)(ir->in_nodes[*newsuccs[i].begin()]->info))->stm)->label_str);
            }
            ir->in_nodes[i]->pres = move(newpreds[i]);
            ir->in_nodes[i]->succs = move(newsuccs[i]);
        }
    }

    void clean_inactive()
    {
        auto nodes = ir->in_nodes;
        for (const auto &it : nodes)
        {
            if (it->in_degree() == 1)
            {
                for (auto jt : ir->Phi_mp[it->uid])
                {
                    auto mv = static_cast<LIR::MoveStm *>(jt.second->stm);
                    auto cl = static_cast<LIR::CallExp *>(mv->src);
                    mv->src = allocate_temp_var_for_exp(cl->args[0]);
                }
                ir->Phi_mp[it->uid].clear();
            }
        }
        for (int i = 0; i < nodes.size(); i++)
        {
            if (!active_block.count(i))
            {
                ir->Phi_mp[i].clear();
                auto stml = static_cast<LIR::StmLinkedList *>(nodes[i]->info);
                stml->next = ir->blockJump[i];
                ir->pre_nodes[i].clear();
                for (auto j : nodes[i]->pres)
                {
                    ir->pre_nodes[i].push_back(j);
                }
            }
        }
    }

    void showIR_active(LIR::StmLinkedList *sll){
    #ifdef DEBUG
        for (; sll; sll = sll->next)
        {
            auto stm = sll->stm;
            if (active_stm.count(stm))
                std::cerr << "***";
            stm->printIR();
        }
    #endif
    }

    void show_mark()
    {
        std::cerr << std::endl
                  << "after mark for active node"
                  << std::endl
                  << std::endl;
        for (const auto &it : ir->in_nodes)
        {
            auto sll = static_cast<LIR::StmLinkedList *>(it->info);
            showIR_active(sll);
            // for (; sll; sll = sll->next)
            // {
            //     auto stm = sll->stm;
            //     if (active_stm.count(stm))
            //         std::cerr << "***";
            //     stm->printIR();
            // }
        }
    }

public:
    DCE(SSA::SSAIR *_ir)
        : ir(_ir),
          node_sz(_ir->in_nodes.size()),
          cdg(new CDG::CDGraph(_ir))
    {
        this->newpreds.resize(node_sz);
        this->newsuccs.resize(node_sz);
        prepare();
        bfs_mark();
        show_mark();
        do_elimilation();
        clean_inactive();
    }
};