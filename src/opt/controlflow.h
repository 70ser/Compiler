#pragma once
#include "../backend/graph.h"
#include "../structure/LIR.h"
#include "../util/linearize.h"
#include <unordered_map>
#include <set>

namespace CFG
{
    typedef std::unordered_map<std::string, GRAPH::Node *> Lable_Node_Table;

    class ControlFlowGraph : public GRAPH::Graph
    {
    public:
        Lable_Node_Table lable_node_table;
        std::unordered_map<int, std::set<int>> node_i_origins; // 在 node(i) 定义的 
        int block_done_id;                                     // block 下个 block id

        std::vector<LIR::StmLinkedList *> blockLabel, blockJump;
        std::vector<bool> exists;
        std::unordered_map<int, std::vector<int>> pre_nodes;

        void cut_edge();
        void combine_node();
        void wake(int from);

        ControlFlowGraph(IRBlock::BlockController bc);
    };
}