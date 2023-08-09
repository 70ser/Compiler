#pragma once
#include "conflictgraph.h"

/**
 * 4. 颜色分配
 *  功能：
 *      颜色即寄存器
 *      主要包括 1.简化 2.合并 3.冻结 4.溢出
 */
namespace COLOR
{
    class COLOR_Assign
    {
    public:
        CONFLICTGRAPH::ConflictGraph *c_g;

        std::unordered_map<int, int> tmp2color;
        std::unordered_map<int, int> spilled_tmp; // conflict tmp(s) -> tmp

        int REGNUM;
        std::set<int> pre_colored_assigned; // r0-r12 (-r11) for int, [~32 - ~1] for float

        std::set<int> simplify_worklist;
        std::set<int> combine_worklist;
        std::set<int> freeze_worklist;
        std::set<int> spill_worklist;

        std::stack<int> select_stack;

        std::set<int> *in_stack_use; // 在 stack 中使用的变量不能被溢出

        void make_worklist();

        void simplify();
        void combine();
        void freeze();
        void spill();

        void change_worklist(int node);
        bool briggs(int n1, int n2);
        void combine_two_nodes(int n1, int n2);
        void do_freeze(int node);

        void assignColor();

        void try_assign();

        COLOR_Assign(CONFLICTGRAPH::ConflictGraph *_c_g, std::set<int> *_in_stack_use)
        {
            this->c_g = _c_g;

            tmp2color = std::unordered_map<int, int>();
            for (int i = 0; i < 15; i++)
                tmp2color[i] = i;
            spilled_tmp = std::unordered_map<int, int>();

            if (c_g->is_float)
            {
                this->REGNUM = 32;
                this->pre_colored_assigned.clear();
                for (int i = 0; i < 32; i++)
                    this->pre_colored_assigned.insert(~i);
            }
            else
            {
                this->REGNUM = 13;
                this->pre_colored_assigned = std::set<int>({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 12, 14}); // 11, 13 for sp
            }

            this->simplify_worklist = std::set<int>();
            this->combine_worklist = std::set<int>();
            this->freeze_worklist = std::set<int>();
            this->spill_worklist = std::set<int>();

            this->select_stack = std::stack<int>();

            this->in_stack_use = _in_stack_use;
            this->try_assign();
        }
    };

}