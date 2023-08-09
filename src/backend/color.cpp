#include "color.h"

void COLOR::COLOR_Assign::try_assign()
{
    this->make_worklist();
    while (!this->simplify_worklist.empty() ||
           !this->combine_worklist.empty() ||
           !this->freeze_worklist.empty() ||
           !this->spill_worklist.empty())
    {
        if (!this->simplify_worklist.empty())
            this->simplify();
        else if (!this->combine_worklist.empty())
            this->combine();
        else if (!this->freeze_worklist.empty())
            this->freeze();
        else if (!this->spill_worklist.empty())
            this->spill();
    }
    this->assignColor();
    std::cerr << std::endl << "ALLOCATE for REG:" << std::endl;
#ifdef DEBUG
    for (auto it = tmp2color.begin(); it != tmp2color.end(); it++)
    {
        std::cerr << "++++++++++++" << std::endl;
        std::cerr << "tmpID: " << it->first << std::endl;
        std::cerr << "reg ASSign: " << it->second << std::endl;
    }
#endif
    return;
}

void COLOR::COLOR_Assign::make_worklist()
{
    for (auto &node : this->c_g->nodes)
    {
        if (node.is_reg())
            continue;
        else if (node.degree() >= REGNUM)
            this->spill_worklist.insert(node.uid);
        else if (!node.moveEdges.empty())
            this->combine_worklist.insert(node.uid);
        else
            this->simplify_worklist.insert(node.uid);
    }
}

void COLOR::COLOR_Assign::change_worklist(int node)
{
    if (this->c_g->nodes[node].is_reg())
        return;
    if (this->simplify_worklist.count(node))
    {
        if (this->c_g->nodes[node].degree() >= REGNUM)
        {
            this->simplify_worklist.erase(node);
            spill_worklist.insert(node);
        }
    }
    else if (this->combine_worklist.count(node))
    {
        if (this->c_g->nodes[node].degree() >= REGNUM)
        {
            this->combine_worklist.erase(node);
            spill_worklist.insert(node);
        }
        else if (this->c_g->nodes[node].moveEdges.empty())
        {
            this->combine_worklist.erase(node);
            simplify_worklist.insert(node);
        }
    }
    else if (this->freeze_worklist.count(node))
    {
        if (this->c_g->nodes[node].degree() >= REGNUM)
        {
            this->freeze_worklist.erase(node);
            spill_worklist.insert(node);
        }
        else if (this->c_g->nodes[node].moveEdges.empty())
        {
            this->freeze_worklist.erase(node);
            simplify_worklist.insert(node);
        }
    }
    else if (this->spill_worklist.count(node))
    {
        if (this->c_g->nodes[node].degree() >= REGNUM)
        {
            // this->spill_worklist.erase(node);
            // spill_worklist.insert(node);
        }
        else if (this->c_g->nodes[node].moveEdges.empty())
        {
            this->spill_worklist.erase(node);
            simplify_worklist.insert(node);
        }
        else if (!this->c_g->nodes[node].moveEdges.empty())
        {
            this->spill_worklist.erase(node);
            combine_worklist.insert(node);
        }
    }
    return;
}

void COLOR::COLOR_Assign::simplify()
{
    int node = *this->simplify_worklist.begin();
    assert(this->c_g->nodes[node].moveEdges.empty());
    this->simplify_worklist.erase(node);
    this->select_stack.push(node);
    while (!this->c_g->nodes[node].directEdges.empty())
    {
        int d_node = *this->c_g->nodes[node].directEdges.begin();
        this->c_g->D2AEdge(node, d_node);
        change_worklist(d_node);
    }
}

/**
 * briggs's conservative heuristic
 */
bool COLOR::COLOR_Assign::briggs(int n1, int n2)
{
    std::set<int> cnt;
    for (int i : this->c_g->nodes[n1].directEdges)
    {
        if (this->c_g->nodes[i].degree() >= this->REGNUM)
            cnt.insert(i);
    }
    for (int i : this->c_g->nodes[n2].directEdges)
    {
        if (this->c_g->nodes[i].degree() >= this->REGNUM)
            cnt.insert(i);
    }
    return cnt.size() < this->REGNUM;
}

void COLOR::COLOR_Assign::combine_two_nodes(int n1, int n2)
{
    // add
    for (int d : this->c_g->nodes[n2].directEdges)
        this->c_g->addDirectEdge(n1, d);
    for (int d : this->c_g->nodes[n2].moveEdges)
        this->c_g->addMoveEdge(n1, d);
    for (int d : this->c_g->nodes[n2].advancedEdges)
        this->c_g->addAdvancedEdge(n1, d);

    // rm
    while (!this->c_g->nodes[n2].directEdges.empty())
        this->c_g->rmDirectEdge(n2, *this->c_g->nodes[n2].directEdges.begin());
    while (!this->c_g->nodes[n2].moveEdges.empty())
        this->c_g->rmMoveEdge(n2, *this->c_g->nodes[n2].moveEdges.begin());
    while (!this->c_g->nodes[n2].advancedEdges.empty())
        this->c_g->rmAdvancedEdge(n2, *this->c_g->nodes[n2].advancedEdges.begin());

    // change tmp point
    for (int tmpid : this->c_g->nodes[n2].tmp_var_id_vec)
    {
        this->c_g->nodes[n1].tmp_var_id_vec.push_back(tmpid);
        this->c_g->tmp_id2node_id__mp[tmpid] = n1;
    }
}

void COLOR::COLOR_Assign::combine()
{
    int node = *this->combine_worklist.begin();
    this->combine_worklist.erase(node);
    std::vector<int> near;
    for (int i : this->c_g->nodes[node].moveEdges)
        near.push_back(i);
    bool del = false;

    for (int near_node : near)
    {
        assert(near_node != node);
        if (this->c_g->nodes[near_node].directEdges.count(node))
        {
            // constrained, as directEdge
            this->c_g->nodes[near_node].moveEdges.erase(node);
            this->c_g->nodes[node].moveEdges.erase(near_node);
            change_worklist(near_node);
        }
        else if (briggs(node, near_node))
        { // 可以合并且相邻节点数量没有超过寄存器数量 则直接合并
            this->combine_two_nodes(near_node, node);
            del = true;
            break;
        }
    }
    if (!del)
        this->freeze_worklist.insert(node);
}

void COLOR::COLOR_Assign::do_freeze(int node)
{
    // freeze moveEdges
    while (!this->c_g->nodes[node].moveEdges.empty())
    {
        int nmv = *this->c_g->nodes[node].moveEdges.begin();
        this->c_g->rmMoveEdge(node, nmv);
        change_worklist(nmv);
    }
}

void COLOR::COLOR_Assign::freeze()
{
    int node = *this->freeze_worklist.begin();
    assert(!this->c_g->nodes[node].is_reg());
    do_freeze(node);
    this->freeze_worklist.erase(node);
    this->simplify_worklist.insert(node);
}

void COLOR::COLOR_Assign::spill()
{
    int selected = -1;
    for (auto one : this->spill_worklist)
    {
        if (this->in_stack_use->count(this->c_g->nodes[one].tmp_var_id()))
            continue;
        assert(!this->c_g->nodes[one].is_reg());
        selected = one;
        break;
    }
    assert(selected != -1);
    do_freeze(selected);
    this->spill_worklist.erase(selected);
    this->simplify_worklist.insert(selected);
}

void COLOR::COLOR_Assign::assignColor()
{
    for (auto col : this->pre_colored_assigned)
    {
        int node = this->c_g->findNode_idByTmp_id(col);
        if (node != -1)
        {
            for (auto tmp : this->c_g->nodes[node].tmp_var_id_vec)
                this->tmp2color[tmp] = col;
        }
    }
    while (!this->select_stack.empty())
    {
        int node = this->select_stack.top();
        this->select_stack.pop();
        std::set<int> pre_c = this->pre_colored_assigned;
        // 移除 advanced edge 转成 direct edge
        while (!this->c_g->nodes[node].advancedEdges.empty())
            this->c_g->A2DEdge(node, *this->c_g->nodes[node].advancedEdges.begin());
        // 消去相连边上的预分配节点
        for (auto d_node : this->c_g->nodes[node].directEdges)
        {
            int d_tmpid = this->c_g->nodes[d_node].tmp_var_id();
            if (this->tmp2color.count(d_tmpid))
                pre_c.erase(this->tmp2color[d_tmpid]);
        }
        if (pre_c.empty())
        {
            // all allocate !!!
            // add to spill
            int _this_tmpid = this->c_g->nodes[node].tmp_var_id();
            for (auto tmp : this->c_g->nodes[node].tmp_var_id_vec)
                this->spilled_tmp.insert({tmp, _this_tmpid});
        }
        else
        {
            // allocate one
            int color = *pre_c.begin();
            for (auto tmp : this->c_g->nodes[node].tmp_var_id_vec)
                this->tmp2color[tmp] = color;
        }
    }
    return;
}
