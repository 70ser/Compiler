#include "flowgraph.h"

void FLOWGRAPH::FlowPragh::buildFromInstrs(std::vector<ASM::Instruction *> *ls)
{
    const int COMMON = 0;
    const int JUMP = 1;
    const int MOVE = 2;

    GRAPH::Node *prev = nullptr, *curr = nullptr;
    std::set<int> *jump_node_list = new std::set<int>();
    int i = 0;
    for (auto instr : *ls)
    {
        if (instr)
        {
            // std::cerr << i++ << std::endl;
            curr = this->add_node(instr);

            int STATE = COMMON;
            std::vector<int> defs = std::vector<int>();
            std::vector<int> uses = std::vector<int>();
            switch (instr->kind)
            {
            case ASM::InstrType::operand:
            {
                if (!static_cast<ASM::Operand *>(instr)->jumps.empty())
                {
                    STATE = JUMP;
                    jump_node_list->insert(curr->uid);
                }
                defs = static_cast<ASM::Operand *>(instr)->dst;
                uses = static_cast<ASM::Operand *>(instr)->src;
            }
            break;
            case ASM::InstrType::label:
            {
                this->lable_node_table.insert({static_cast<ASM::Label *>(instr)->label, curr});
            }
            break;
            case ASM::InstrType::move:
            {
                STATE = MOVE;
                defs = static_cast<ASM::Move *>(instr)->dst;
                uses = static_cast<ASM::Move *>(instr)->src;
                assert(defs.size() == 1 && uses.size() == 1);
            }
            break;
            }

            this->node_udinfo_table.insert({curr,
                                            new Use_Def_Info(uses, defs, STATE == MOVE)});
            if (prev != nullptr)
                prev->belong_graph->add_path(prev, curr);
            prev = (STATE != JUMP) ? curr : nullptr;
        }
    }
    // 跳转控制
    for (const int id : *jump_node_list)
    {
        ASM::Operand *jp_instr =
            static_cast<ASM::Operand *>(this->in_nodes[id]->nodeInfo());
        std::vector<std::string> jp_labels = jp_instr->jumps;
        for (const auto &l : jp_labels)
        {
            if (!l.empty())
            { // add jump-to path
                GRAPH::Node *node = this->lable_node_table[l];
                this->add_path(this->in_nodes[id], node);
            }
        }
    }
    return;
}