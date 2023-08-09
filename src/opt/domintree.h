#pragma once

#include "../backend/graph.h"

/**
 * dominator tree, like `Union-Find`, controls bolcks link info
 */

namespace DT
{
    class DT
    {

    private:
        int node_cnt;
        int entry_id;
        GRAPH::Graph *g;

        std::vector<int> sc_pos; // position in source code
        std::vector<int> i_node; // [i] -> node
        std::vector<int> parent; // direct parent

        std::vector<int> i_same_domi;
        std::vector<int> i_semi_ancestor;
        std::vector<int> i_domi;

        std::vector<int> best;
        std::vector<int> i_ancestor;
        std::vector<std::vector<int>> i_succs;

        void dfs(int node, int fa)
        {
            sc_pos[node] = this->node_cnt;
            i_node[this->node_cnt] = node;
            parent[node] = fa;
            this->node_cnt++;
            for (const auto it : this->g->in_nodes[node]->succs)
            {
                if (sc_pos[it] == -1)
                    dfs(it, node);
            }
        }

        int find_lowest_semi_ancestor(int node)
        {
            int a = i_ancestor[node];
            if (i_ancestor[a] != -1)
            {
                int aa = find_lowest_semi_ancestor(a);
                i_ancestor[node] = i_ancestor[a];
                if (sc_pos[i_semi_ancestor[aa]] < sc_pos[i_semi_ancestor[best[node]]])
                    best[node] = aa;
            }
            return best[node];
        }

        int union_find(int node)
        {
            if (i_ancestor[node] == node)
                return node;
            return i_ancestor[node] = union_find(i_ancestor[node]);
        }

        /**
         * 支配前沿是指在支配树中，一个基本块支配的所有后继基本块中，不被该基本块直接支配的基本块
         */
        void gen_DF(int node)
        {
            for (auto succ : this->g->in_nodes[node]->succs)
            {
                if (i_domi[succ] != node)
                    DF[node].push_back(succ);
            }
            for (auto succ : this->CH[node])
            {
                gen_DF(succ);
                i_ancestor[succ] = node;
            }
            for (auto succ : this->CH[node])
            {
                for (int d : DF[succ])
                {
                    if (d == node || union_find(d) != node)
                        DF[node].push_back(d);
                }
            }
        }

    public:
        std::vector<std::vector<int>> DF;
        std::vector<std::vector<int>> CH;

        DT(GRAPH::Graph *_g, int _entry_id)
            : node_cnt(0),
              entry_id(_entry_id),
              g(_g),
              sc_pos(_g->node_count, -1),
              i_node(_g->node_count, -1),
              parent(_g->node_count, -1),
              i_same_domi(_g->node_count, -1),
              i_semi_ancestor(_g->node_count, -1),
              i_domi(_g->node_count, -1),
              best(_g->node_count, -1),
              i_ancestor(_g->node_count, -1),
              i_succs(_g->node_count, std::vector<int>()),
              DF(_g->node_count, std::vector<int>()),
              CH(_g->node_count, std::vector<int>())
        {
            dfs(entry_id, -1);
            // reverse iter
            for (int i = this->node_cnt - 1; i >= 1; i--)
            {
                auto node = i_node[i];
                auto pa = parent[node];
                int ans = pa;
                for (auto p : this->g->in_nodes[node]->pres)
                {
                    int nxt = -1;
                    if (sc_pos[p] <= sc_pos[node])
                        nxt = p;
                    else
                        nxt = i_semi_ancestor[find_lowest_semi_ancestor(p)];
                    if (sc_pos[nxt] < sc_pos[ans])
                        ans = nxt;
                }
                i_semi_ancestor[node] = ans;
                i_succs[ans].push_back(node);

                i_ancestor[node] = pa;
                best[node] = node;

                for (auto su : i_succs[pa])
                {
                    auto i = find_lowest_semi_ancestor(su);
                    if (i_semi_ancestor[i] == i_semi_ancestor[su])
                        i_domi[su] = pa;
                    else
                        i_same_domi[su] = i;
                }
                i_succs[pa].clear();
            }

            for (int i = 1; i < this->node_cnt; i++)
            {
                int node = i_node[i];
                if (i_same_domi[node] != -1)
                    i_domi[node] = i_domi[i_same_domi[node]];
            }

            for (int i = 1; i < this->node_cnt; i++)
            {
                int node = i_node[i];
                this->CH[i_domi[node]].push_back(node);
            }
        }

        void gen_DF()
        {
            for (int i = 0; i < this->i_ancestor.size(); i++)
                i_ancestor[i] = i;
            this->gen_DF(i_node[0]);
        }
    };
}
