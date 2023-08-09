#include "controlflow.h"
#include "../util/cast.h"

CFG::ControlFlowGraph::ControlFlowGraph(IRBlock::BlockController bc)
{
    // add all block items to graph
    this->lable_node_table = Lable_Node_Table();
    for (auto bll = bc.b_list; bll; bll = bll->next)
    {
        auto sll = bll->stm_l_head;
        auto stm = sll->stm;
        assert(stm->stm_kind == LIR::STM_KIND::label);

        GRAPH::Node *node = this->add_node(sll);
        // this->node_i_origins[this->node_count - 1] = std::set<int>();
        blockLabel.push_back(sll);
        std::string label = static_cast<LIR::LabelStm *>(stm)->label_str;
        lable_node_table.insert({label, node});
    }
    // plus an end node
    std::string b_done_label = bc.done_label;
    auto end_stm = new LIR::StmLinkedList(new LIR::LabelStm(b_done_label), nullptr);
    auto end_node = this->add_node(end_stm);
    this->block_done_id = end_node->uid;
    // this->node_i_origins[this->node_count - 1] = std::set<int>();
    blockLabel.push_back(end_stm);
    lable_node_table.insert({b_done_label, end_node});

    for (auto node : this->in_nodes)
    {
        if (node == end_node)
            continue;
        auto sll = (LIR::StmLinkedList *)node->nodeInfo();
        auto _at_sll = sll;
        for (; _at_sll->next; _at_sll = _at_sll->next)
        {
            auto def = getDefExp(_at_sll->stm);
            if (def)
                node_i_origins[node->uid]
                    .insert(static_cast<LIR::TempVarExp *>(*def)->temp_label_id_int);
        }
        blockJump.push_back(_at_sll);
        switch (_at_sll->stm->stm_kind)
        {
        case LIR::STM_KIND::jump:
        {
            auto j_label = static_cast<LIR::JumpStm *>(_at_sll->stm)->jump_label_str;
            node->belong_graph->add_path(node, lable_node_table.at(j_label));
        }
        break;
        case LIR::STM_KIND::cjump:
        {
            auto t_label = static_cast<LIR::CJumpStm *>(_at_sll->stm)->trueLabel;
            auto f_label = static_cast<LIR::CJumpStm *>(_at_sll->stm)->falseLabel;
            node->belong_graph->add_path(node, lable_node_table.at(t_label));
            node->belong_graph->add_path(node, lable_node_table.at(f_label));
        }
        break;
        default:
            assert(0);
        }
    }
    blockJump.push_back(nullptr); // cut
    this->exists = std::vector<bool>(this->node_count, false);
    this->pre_nodes = std::unordered_map<int, std::vector<int>>();
    wake(0);
    // rm unreachable nodes
    for (int i = 0; i < this->node_count; i++)
    {
        if (exists[i])
            continue;
        while (!this->in_nodes[i]->pres.empty())
            this->rm_path(in_nodes[*this->in_nodes[i]->pres.begin()], in_nodes[i]);
        while (!this->in_nodes[i]->succs.empty())
            this->rm_path(in_nodes[i], in_nodes[*this->in_nodes[i]->succs.begin()]);
        node_i_origins[i].clear();
    }
    for (int i = 0; i < this->node_count; i++)
    {
        if (exists[i])
        {
            for (auto it : in_nodes[i]->pres)
                pre_nodes[i].push_back(it);
        }
    }
    cut_edge();
    combine_node();

    // std::cerr << std::endl
    //           << "after controlflow ... " << std::endl;
    // for (int i = 0; i < this->node_count; i++)
    // {
    //     auto sll = (LIR::StmLinkedList *)(this->in_nodes[i]->info);
    //     std::cerr << "Block: " << i << std::endl;
    //     while (sll)
    //     {
    //         sll->stm->printIR();
    //         sll = sll->next;
    //     }
    //     std::cerr << std::endl;
    // }
}

/**
 * cut between cjump -> stm by adding a direct jump
 */
void CFG::ControlFlowGraph::cut_edge()
{
    int old_node_cnt = this->node_count;
    for (int i = 0; i < old_node_cnt; i++)
    {
        if (in_nodes[i]->in_degree() <= 1)
            continue;
        std::string _this_label = static_cast<LIR::LabelStm *>(blockLabel[i]->stm)->label_str;
        for (auto &pre_id : pre_nodes[i])
        {
            // con cjump with JumpStm
            if (blockJump[pre_id]->stm->stm_kind == LIR::STM_KIND::cjump)
            {
                int _this_pre_id = pre_id;
                LIR::Stm *cjump_stm = blockJump[pre_id]->stm;
                LIR::StmLinkedList *prelist;

                std::string tmp_label;
                // ((pre)trueLabel => (pre)tmp_label) -> jump -> _this_label
                if (_this_label == static_cast<LIR::CJumpStm *>(cjump_stm)->trueLabel)
                {
                    tmp_label = newLabel();
                    static_cast<LIR::CJumpStm *>(cjump_stm)->trueLabel = tmp_label;
                    prelist = new LIR::StmLinkedList(new LIR::LabelStm(tmp_label),
                                                     new LIR::StmLinkedList(new LIR::JumpStm(_this_label), nullptr));
                    _this_pre_id = this->add_node(prelist)->uid;
                }
                else
                {
                    tmp_label = newLabel();
                    static_cast<LIR::CJumpStm *>(cjump_stm)->falseLabel = tmp_label;
                    prelist = new LIR::StmLinkedList(new LIR::LabelStm(tmp_label),
                                                     new LIR::StmLinkedList(new LIR::JumpStm(_this_label), nullptr));
                    _this_pre_id = this->add_node(prelist)->uid;
                }
                this->rm_path(in_nodes[pre_id], in_nodes[i]);

                this->add_path(in_nodes[pre_id], in_nodes[_this_pre_id]);
                this->add_path(in_nodes[_this_pre_id], in_nodes[i]);

                this->blockLabel.push_back(prelist);
                this->blockJump.push_back(prelist->next);

                // pre_nodes[this->node_count - 1] = std::vector<int>{pre_id};
                pre_nodes[this->node_count - 1].push_back(pre_id);
                pre_id = _this_pre_id;
            }
        }
    }
}

/**
 * merge continued nodes
 */
void CFG::ControlFlowGraph::combine_node()
{
    int old_node_cnt = this->node_count;
    bool flag = true;
    while (flag)
    {
        flag = false;
        for (int i = 0; i < old_node_cnt; i++)
        {
            if (i != 0 && in_nodes[i]->in_degree() == 0)
                continue;
            if (in_nodes[i]->out_degree() != 1)
                continue;
            int succ_id = *in_nodes[i]->succs.begin();
            if (in_nodes[succ_id]->in_degree() != 1 || succ_id == block_done_id)
                continue;

            in_nodes[i]->succs.clear();
            in_nodes[succ_id]->pres.clear();
            pre_nodes[succ_id].clear();

            blockJump[i]->next = blockLabel[succ_id]->next;
            blockJump[i]->stm = NOPStm();
            blockJump[i] = blockJump[succ_id];

            for (auto it : node_i_origins[succ_id])
                node_i_origins[i].insert(it);

            LIR::StmLinkedList *stml =
                new LIR::StmLinkedList(new LIR::LabelStm("NOWHERE"),
                                       new LIR::StmLinkedList(new LIR::JumpStm("ERROR"), nullptr));
            blockLabel[succ_id] = stml;
            blockJump[succ_id] = stml->next;

            in_nodes[succ_id]->info = (void *)stml;
            std::vector<int> succ_succs;
            while (!in_nodes[succ_id]->succs.empty())
            {
                int n = *in_nodes[succ_id]->succs.begin();
                succ_succs.push_back(n);
                rm_path(in_nodes[succ_id], in_nodes[n]);
            }
            for (auto succ_succ : succ_succs)
            {
                add_path(in_nodes[i], in_nodes[succ_succ]);
                for (auto &pred : pre_nodes[succ_succ])
                    if (pred == succ_id)
                        pred = i;
            }
            flag = true;
            break;
        }
    }
}

void CFG::ControlFlowGraph::wake(int from)
{
    this->exists[from] = true;
    for (auto it : this->in_nodes[from]->succ())
    {
        if (!exists[it])
            wake(it);
    }
}