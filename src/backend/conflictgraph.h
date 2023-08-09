#pragma once
#include "livevar.h"

/**
 * 3.冲突图
 *  功能: 标记冲突边(三类) 
 *        为染色算法做准备
*/
namespace CONFLICTGRAPH
{
    class Node
    {
    public:
        int uid;
        std::set<int> directEdges;
        std::set<int> moveEdges;
        std::set<int> advancedEdges;
        std::vector<int> tmp_var_id_vec; // 11 13    ....    1001
        int degree() { return this->directEdges.size(); }
        int tmp_var_id() { return this->tmp_var_id_vec.at(0); }
        int is_reg() { return abs(tmp_var_id_vec[0]) < 100; }

        Node(int _uid)
            : uid(_uid),
              directEdges(std::set<int>()), moveEdges(std::set<int>()), advancedEdges(std::set<int>()),
              tmp_var_id_vec(std::vector<int>()) {}
    };

    class ConflictGraph
    {
    public:
        // temp_var_id(1001, ....) to node_id
        std::unordered_map<int, int> tmp_id2node_id__mp;
        std::vector<Node> nodes;
        int node_cnt;
        bool is_float;

        void build_cg(LIVEVAR::LiveVariableAnalysis *lva);

        ConflictGraph(LIVEVAR::LiveVariableAnalysis *lva, bool _f)
            : node_cnt(0), is_float(_f), tmp_id2node_id__mp(std::unordered_map<int, int>())
        {
            this->build_cg(lva);
        }

        void addNode(int tmp_var_id)
        {
            if (tmp_id2node_id__mp.count(tmp_var_id))
                return;
            if (tmp_var_id == 11 || tmp_var_id == 13)
                return; // sp
            if (tmp_var_id >= 0 && is_float || tmp_var_id < 0 && !is_float)
                return;
            Node node(this->node_cnt);
            // node.tmp_var_id_vec.push_back(tmp_var_id);
            nodes.push_back(node);
            tmp_id2node_id__mp[tmp_var_id] = this->node_cnt;
            this->nodes[this->node_cnt].tmp_var_id_vec.push_back(tmp_var_id);
            this->node_cnt++;
        }

        int findNode_idByTmp_id(int t)
        {
            return tmp_id2node_id__mp.count(t) ? tmp_id2node_id__mp[t] : -1;
        }

        void addDirectEdge(int node_id1, int node_id2);
        void rmDirectEdge(int node_id1, int node_id2);

        void addMoveEdge(int node_id1, int node_id2);
        void rmMoveEdge(int node_id1, int node_id2);

        void addAdvancedEdge(int node_id1, int node_id2);
        void rmAdvancedEdge(int node_id1, int node_id2);

        void D2AEdge(int node_id1, int node_id2);
        void A2DEdge(int node_id1, int node_id2);

        void addMoveInstr(ASM::Move *mv);
    };

}