#pragma once

#include "../structure/LIR.h"
#include "../util/EntryTable.h"
#include "../util/linearize.h"

#include <unordered_set>

namespace FI
{
    class FUNC_IN_INFO
    {
    public:
        LIR::StmLinkedList *ir_ll;

        int called_num = 0;
        int call_num = 0;
        int code_len = 0;

        int stack_size = 0;
        ;

        bool is_recur = false; // 是否递归
        bool is_void = false;

        // callee name, call stm
        std::vector<std::pair<std::string, LIR::StmLinkedList *>> pos_called;
        std::unordered_set<std::string> global_val_uesd;

        FUNC_IN_INFO() : pos_called({}), global_val_uesd({}) {}
    };

    class FuncInline
    {
    public:
        const int _MAX_STK_SIZE = 8 * 1024;
        const int _BASE_STK_SIZE = 64;
        const int _MAX_STK_GROWTH_MULTI = 13;

        std::unordered_map<std::string, FUNC_IN_INFO> func_info;

        std::unordered_set<std::string> global_var;
        std::unordered_map<std::string, LIR::StmLinkedList *> func_map;
        std::vector<std::string> func_name;

        EntryTable<FuncEntry *> *func_table;
        EntryTable<Entry *> *var_table;

        FuncInline(EntryTable<Entry *> *_var_table,
                   EntryTable<FuncEntry *> *_func_table,
                   std::vector<std::string> _func_name,
                   std::unordered_map<std::string, std::vector<LIR::Stm *> *> &_func_ir)
        {
            this->var_table = _var_table;
            this->func_table = _func_table;
            this->func_name = _func_name;
            for (auto it = _func_ir.begin(); it != _func_ir.end(); it++)
            {
                std::string name = it->first;
                LIR::StmLinkedList *sll = toStmLinkedList(it->second);
                this->func_map.insert({name, sll});
            }
            for (auto it = var_table->entryMap.begin(); it != var_table->entryMap.end(); it++)
            {
                Entry *entry = var_table->findEntry(it->first);
                assert(entry && entry->kind == EntryKind::GLOBAL);
                if (entry->type->is_const)
                    assert(entry->type->t_kind != T_KIND::ARRAY);
                std::string v_name = static_cast<GlobalvarEntry *>(entry)->label;
                this->global_var.insert(v_name);
            }
            for (const auto &it : this->func_name)
            {
                this->func_info[it] = FUNC_IN_INFO();
            }
        }

        std::vector<std::string> do_func_inline();

        void analyse_func(std::string func_name);

        /**
         * @param: func_name -> caller's func name
         * @return: { <callee's func name, call Stm> }
         */
        std::vector<std::pair<std::string, LIR::StmLinkedList *>> get_call_pos(std::string func_name);

        bool global2local();
    };
}