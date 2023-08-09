#pragma once

#include <set>
#include <stack>
#include <algorithm>
#include "flowgraph.h"

/**
 *  2.变量活跃分析
 *  功能：
 *      接收 flowgraph根据每个 node 中的 def 和 use 信息
 *      更新每个节点的变量活跃信息 (int out)
 */
namespace LIVEVAR
{
    /**
     * in[n] = U(p ∈ pred[n]) out[p]
     * out[n] = def[n] ∪ (in[n] - kill[n])
     *
     *     kill[n] = in[n] ∩ def[n]
     */
    class IN_OUT_Set
    {
    public:
        std::set<int> *in;
        std::set<int> *out;

        // 'use' 'def' comes from FLowGraph
        std::set<int> *use;
        std::set<int> *def;

        IN_OUT_Set(std::set<int> *_in, std::set<int> *_out,
                   std::set<int> *_use, std::set<int> *_def)
            : in(_in), out(_out), use(_use), def(_def) {}

        ~IN_OUT_Set()
        {
            delete in;
            delete out;
            delete use;
            delete def;
        }
    };

    typedef std::unordered_map<GRAPH::Node *, IN_OUT_Set *> IN_OUT_Table;

    class LiveVariableAnalysis
    {
    public:
        FLOWGRAPH::FlowPragh *flow_graph;
        IN_OUT_Table in_out_table;

        std::set<int> *liveVar_IN(GRAPH::Node *node) { return this->in_out_table[node]->in; }

        std::set<int> *liveVar_OUT(GRAPH::Node *node) { return this->in_out_table[node]->out; }

        /**
         * gen in/out set
         * */
        void analysis_live_var()
        {
            std::stack<GRAPH::Node *> ws;
            for (auto node : this->flow_graph->in_nodes)
            {
                ws.push(node);
                assert(in_out_table[node]->in->empty());
                assert(in_out_table[node]->out->empty());
            }
            // bfs
            while (!ws.empty())
            {
                auto node = ws.top();
                ws.pop();

                std::set<int> *newIn = new std::set<int>();
                std::set<int> *newOut = new std::set<int>();
                std::set<int> next_tmp_set;
                assert(next_tmp_set.empty());

                IN_OUT_Set *node_io = this->in_out_table.at(node);
                for (auto nxt : node->succs)
                {
                    IN_OUT_Set *nxt_io = in_out_table.at(flow_graph->in_nodes[nxt]);
                    for (auto in_nxt : *nxt_io->in)
                        newOut->insert(in_nxt);
                }
                // in = use + out(输出) - def(包括重新定义的节点)
                std::set_difference(newOut->begin(), newOut->end(),
                                    node_io->def->begin(), node_io->def->end(),
                                    std::inserter(next_tmp_set, next_tmp_set.begin()));
                std::set_union(node_io->use->begin(), node_io->use->end(),
                               next_tmp_set.begin(), next_tmp_set.end(),
                               std::inserter(*newIn, newIn->begin()));
                
                if (*node_io->in != *newIn)
                {
                    assert(node_io->in->size() < newIn->size());
                    assert(newIn->size() < this->in_out_table.size());
                    for (auto pre : node->pres)
                    {
                        assert(pre != node->uid);
                        ws.push(this->flow_graph->in_nodes[pre]);
                    }
                }

                delete node_io->in;
                delete node_io->out;
                node_io->in = newIn;
                node_io->out = newOut;
            }
            // // in out 中会多出 1
            // for (auto it = in_out_table.begin(); it != in_out_table.end(); it++)
            // {
            //     std::cerr << "++++++++" << std::endl;
            //     std::cerr << it->first->uid << std::endl;

            //     std::cerr << " use: ";
            //     for (int i : *it->second->use) std::cerr << i << " ";
            //     std::cerr << std::endl;

            //     std::cerr << " def: ";
            //     for (int i : *it->second->def) std::cerr << i << " ";
            //     std::cerr << std::endl;

            //     std::cerr << " in: ";
            //     for (int i : *it->second->in) std::cerr << i << " ";
            //     std::cerr << std::endl;

            //     std::cerr << " out: ";
            //     for (int i : *it->second->out) std::cerr << i << " ";
            //     std::cerr << std::endl;

            //     std::cerr << "++++++++" << std::endl;
            // }
        }

        LiveVariableAnalysis(FLOWGRAPH::FlowPragh *_flow_graph)
        {
            this->flow_graph = _flow_graph;
            in_out_table = IN_OUT_Table();
            for (auto node : flow_graph->in_nodes)
            {
                if (in_out_table.count(node) == 0)
                {
                    std::set<int> *in, *out, *use, *def;
                    in = new std::set<int>();
                    out = new std::set<int>();
                    use = new std::set<int>();
                    def = new std::set<int>();

                    for (auto it : *flow_graph->node_use(node))
                        use->insert(it);
                    for (auto it : *flow_graph->node_def(node))
                        def->insert(it);
                    this->in_out_table.insert({node, new IN_OUT_Set(in, out, use, def)});
                }
            }
            analysis_live_var();
        }

        ~LiveVariableAnalysis()
        {
            for (auto it : this->in_out_table)
            {
                delete it.second;
            }
        }
    };
}
