#pragma once

#include <queue>

#include "../backend/graph.h"

#include "controlflow.h"
#include "domintree.h"

/**
 * 在CDG中，如果一个基本块的执行依赖于另一个基本块的控制流决策结果，那么这两个基本块之间存在一条边
 */
namespace CDG
{
    class CDGraph : public GRAPH::Graph
    {
    private:
        void bfs(int node)
        {
            std::vector<bool> vis(this->node_count, 0);
            std::queue<int> q;
            q.push(node);
            while (!q.empty())
            {
                auto tp = q.front();
                q.pop();
                for (auto nxt : dt->DF[tp])
                {
                    if (nxt != this->node_count - 1)
                        cd_node[node].push_back(nxt);
                    if (!vis[nxt])
                    {
                        vis[nxt] = true;
                        q.push(nxt);
                    }
                }
            }
        }

    public:
        std::vector<std::vector<int>> cd_node;
        DT::DT *dt;
        int block_done_id;

        CDGraph(CFG::ControlFlowGraph *cfg)
        {
            for (auto node : cfg->in_nodes)
            {
                auto n_node = this->add_node(node);
                n_node->pres = node->succs;
                n_node->succs = node->pres;
            }
            this->block_done_id = cfg->block_done_id;

            auto root_node = this->add_node(0);
            add_path(in_nodes[block_done_id], root_node);
            add_path(in_nodes[0], root_node);

            this->dt = new DT::DT(this, this->block_done_id);
            this->dt->gen_DF();

            this->cd_node = std::vector<std::vector<int>>(this->node_count, std::vector<int>());
            for (int i = 0; i < this->node_count; i++)
                this->bfs(i);
        }
    };

}