#pragma once
#include "graph.h"
#include "../structure/instruction.h"

#include <unordered_map>
/**
 * 1.流图
 * 功能：
 *      1. 标记是否为 mov(后面冲突图中有 moveEdges)
 *      2. 根据 dst 和 src 标记句子的 def 和 use (后面 'livevar' 中用来分析 in out)
*/
namespace FLOWGRAPH
{
    /**
     * int x = 5;
     * int y = x + 3;
     * int z = x * y;
     *
     *
     *  Variable | Use  | Def
     *  ---------|------|-----
     *  x        |      | 1
     *  y        | 1    | 2
     *  z        | 1,2  | 3
     */
    class Use_Def_Info
    {
    public:
        std::vector<int> uses;
        std::vector<int> defs;
        bool is_move;
        Use_Def_Info(std::vector<int> _uses, std::vector<int> _defs, bool _is_move)
            : uses(_uses), defs(_defs), is_move(_is_move) {}
    };

    // line_no  ->  Node*
    typedef std::unordered_map<std::string, GRAPH::Node *> Lable_Node_Table;

    // Node*    ->  Use_Def_Info
    typedef std::unordered_map<GRAPH::Node *, Use_Def_Info *> Node_UseDefInfo_Table;

    class FlowPragh : public GRAPH::Graph
    {
    public:
        Lable_Node_Table lable_node_table;

        Node_UseDefInfo_Table node_udinfo_table;

        std::vector<int> *node_def(GRAPH::Node *node) { return &node_udinfo_table[node]->defs; }

        std::vector<int> *node_use(GRAPH::Node *node) { return &node_udinfo_table[node]->uses; }

        bool node_ismove(GRAPH::Node *node) { return node_udinfo_table[node]->is_move; }

        FlowPragh(std::vector<ASM::Instruction *> *ls)
        {
            lable_node_table = Lable_Node_Table();
            node_udinfo_table = Node_UseDefInfo_Table();
            buildFromInstrs(ls);
        }

        void buildFromInstrs(std::vector<ASM::Instruction *> *ls);
    };

}
