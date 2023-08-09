#pragma once
#include <set>
#include <vector>
#include <assert.h>

namespace GRAPH
{
    class Node;
    class Graph;

    class Node
    {
    public:
        int uid;                // 标识符
        Graph *belong_graph;    // 所属图
        std::set<int> pres;  // 前驱节点
        std::set<int> succs; // 后继节点
        void *info = nullptr;

        Node() {}
        Node(int _k, Graph *_belong_graph, void *_info) : uid(_k), belong_graph(_belong_graph), info(_info)
        {
            this->pres = std::set<int>();
            this->succs = std::set<int>();
        }

        void *nodeInfo() { return this->info; }

        std::set<int> pre() { return this->pres; }

        std::set<int> succ() { return this->succs; }

        int in_degree() { return this->pres.size(); }

        int out_degree() { return this->succs.size(); }
    };

    class Graph
    {
    public:
        std::vector<Node *> in_nodes;
        int node_count;

        Graph() : in_nodes(std::vector<Node *>()), node_count(0) {}

        ~Graph()
        {
            for (auto &it : in_nodes)
                delete it;
        }

        void remove_all_nodes()
        {
            for (auto &it : in_nodes)
                delete it;
            in_nodes.clear();
            node_count = 0;
        }

        Node *add_node(void *info)
        {
            Node *node = new Node(this->node_count++, this, info);
            this->in_nodes.push_back(node);
            return node;
        }

        void add_path(Node *from, Node *to)
        {
            assert(from != nullptr && to != nullptr);
            assert(from->belong_graph == to->belong_graph);
            if (from->succs.count(to->uid)) return;
            from->succs.insert(to->uid);
            to->pres.insert(from->uid);
            return;
        }

        void rm_path(Node *from, Node *to)
        {
            assert(from != nullptr && to != nullptr);
            from->succs.erase(from->succs.find(to->uid));
            to->pres.erase(to->pres.find(from->uid));
        }
    };

} // namespace GRAPH
