#include "conflictgraph.h"

/**
 * 
*/
void CONFLICTGRAPH::ConflictGraph::build_cg(LIVEVAR::LiveVariableAnalysis *lva)
{
    for (auto it : lva->flow_graph->in_nodes)
    {
        std::vector<int> *defs = lva->flow_graph->node_def(it);
        std::vector<int> *uses = lva->flow_graph->node_use(it);
        std::set<int> *outs = lva->liveVar_OUT(it);
        for (auto i : *defs)
            this->addNode(i);
        for (auto i : *uses)
            this->addNode(i);
        for (auto i : *outs)
            this->addNode(i);
    }
    for (auto it : lva->flow_graph->in_nodes)
    {
        std::vector<int> *defs = lva->flow_graph->node_def(it);
        std::vector<int> *uses = lva->flow_graph->node_use(it);
        std::set<int> *outs = lva->liveVar_OUT(it);
        if (lva->flow_graph->node_ismove(it))
        {
            assert(defs->size() == 1 && uses->size() == 1);
            int dst = defs->at(0);
            int src = uses->at(0);
            auto mv =
                static_cast<ASM::Move *>((ASM::Instruction *)(it)->nodeInfo());
            addMoveInstr(mv);
            for (int ot : *outs)
            {
                // ot == src 表示寄存器可重用 不产生冲突
                if (ot != src)
                {
                    int n1 = findNode_idByTmp_id(ot);
                    int n2 = findNode_idByTmp_id(dst);
                    if (n1 != -1 && n2 != -1)
                        addDirectEdge(n1, n2);
                }
            }
        }
        else
        {
            for (auto def : *defs)
            {
                for (int ot : *outs)
                {
                    int n1 = findNode_idByTmp_id(def);
                    int n2 = findNode_idByTmp_id(ot);
                    if (n1 != -1 && n2 != -1)
                        addDirectEdge(n1, n2);
                }
            }
        }
    }
    // for (auto no : this->nodes)
    // {
    //     std::cerr << "++++++++++" << std::endl;
    //     std::cerr << "Node ID: " << no.uid << std::endl;
    //     std::cerr << "directEdges: " << std::endl;
    //     for (int i : no.directEdges) std::cerr << i << ", ";
    //     std::cerr << std::endl;
    //     std::cerr << "moveEdges: " << std::endl;
    //     for (int i : no.moveEdges) std::cerr << i << ", ";
    //     std::cerr << std::endl;
    //     std::cerr << "advancedEdges: " << std::endl;
    //     for (int i : no.advancedEdges) std::cerr << i << ", ";
    //     std::cerr << std::endl;
    // }
}

void CONFLICTGRAPH::ConflictGraph::addDirectEdge(int node_id1, int node_id2)
{
    if (node_id1 == node_id2)
        return;
    assert(!nodes[node_id1].advancedEdges.count(node_id2) &&
           !nodes[node_id2].advancedEdges.count(node_id1));
    nodes[node_id1].directEdges.insert(node_id2);
    nodes[node_id2].directEdges.insert(node_id1);
    return;
}

void CONFLICTGRAPH::ConflictGraph::rmDirectEdge(int node_id1, int node_id2)
{
    if (node_id1 == node_id2)
        return;
    assert(nodes[node_id1].directEdges.count(node_id2) &&
           nodes[node_id2].directEdges.count(node_id1));
    nodes[node_id1].directEdges.erase(node_id2);
    nodes[node_id2].directEdges.erase(node_id1);
    return;
}

void CONFLICTGRAPH::ConflictGraph::addMoveEdge(int node_id1, int node_id2)
{
    if (node_id1 == node_id2)
        return;
    nodes[node_id1].moveEdges.insert(node_id2);
    nodes[node_id2].moveEdges.insert(node_id1);
    return;
}

void CONFLICTGRAPH::ConflictGraph::rmMoveEdge(int node_id1, int node_id2)
{
    if (node_id1 == node_id2)
        return;
    assert(nodes[node_id1].moveEdges.count(node_id2) &&
           nodes[node_id2].moveEdges.count(node_id1));
    nodes[node_id1].moveEdges.erase(node_id2);
    nodes[node_id2].moveEdges.erase(node_id1);
    return;
}

void CONFLICTGRAPH::ConflictGraph::addAdvancedEdge(int node_id1, int node_id2)
{
    if (node_id1 == node_id2)
        return;
    assert(!nodes[node_id1].directEdges.count(node_id2) &&
           !nodes[node_id2].directEdges.count(node_id1));
    nodes[node_id1].advancedEdges.insert(node_id2);
    nodes[node_id2].advancedEdges.insert(node_id1);
    return;
}

void CONFLICTGRAPH::ConflictGraph::rmAdvancedEdge(int node_id1, int node_id2)
{
    if (node_id1 == node_id2)
        return;
    assert(nodes[node_id1].advancedEdges.count(node_id2) &&
           nodes[node_id2].advancedEdges.count(node_id1));
    nodes[node_id1].advancedEdges.erase(node_id2);
    nodes[node_id2].advancedEdges.erase(node_id1);
    return;
}

void CONFLICTGRAPH::ConflictGraph::D2AEdge(int node_id1, int node_id2)
{
    assert(nodes[node_id1].directEdges.count(node_id2) &&
           nodes[node_id2].directEdges.count(node_id1));

    nodes[node_id1].directEdges.erase(node_id2);
    nodes[node_id2].directEdges.erase(node_id1);

    nodes[node_id1].advancedEdges.insert(node_id2);
    nodes[node_id2].advancedEdges.insert(node_id1);
}

void CONFLICTGRAPH::ConflictGraph::A2DEdge(int node_id1, int node_id2)
{
    assert(nodes[node_id1].advancedEdges.count(node_id2) &&
           nodes[node_id2].advancedEdges.count(node_id1));

    nodes[node_id1].advancedEdges.erase(node_id2);
    nodes[node_id2].advancedEdges.erase(node_id1);

    nodes[node_id1].directEdges.insert(node_id2);
    nodes[node_id2].directEdges.insert(node_id1);
}

void CONFLICTGRAPH::ConflictGraph::addMoveInstr(ASM::Move *mv)
{
    int dst_id = findNode_idByTmp_id(mv->dst[0]);
    int src_id = findNode_idByTmp_id(mv->src[0]);
    if (dst_id == -1 || src_id == -1)
        return;
    addMoveEdge(dst_id, src_id);
}