#pragma once
#include <memory>
#include "color.h"

namespace REGALLOC
{
    static int _this_stk_size = 0;

    static std::vector<ASM::Instruction *> *gen_spill(std::vector<ASM::Instruction *> *is,
                                                      COLOR::COLOR_Assign *ca,
                                                      std::unordered_map<int, int> *stk_map,
                                                      std::set<int> *stk_use)
    {
        assert(is);
        std::vector<ASM::Instruction *> *ans = new std::vector<ASM::Instruction *>();
        // tend to find in local stack first
        for (auto &in : *is)
        {
            std::vector<int> *dst = nullptr, *src = nullptr;
            switch (in->kind)
            {
            case ASM::InstrType::label:
                break;
            case ASM::InstrType::operand:
            {
                dst = &static_cast<ASM::Operand *>(in)->dst;
                src = &static_cast<ASM::Operand *>(in)->src;
            }
            break;
            case ASM::InstrType::move:
            {
                dst = &static_cast<ASM::Operand *>(in)->dst;
                assert(dst->size() == 1);
                src = &static_cast<ASM::Operand *>(in)->src;
                assert(src->size() == 1);
                if (ca->spilled_tmp.count(dst->at(0)) && ca->spilled_tmp.count(src->at(0)) &&
                    ca->spilled_tmp.at(dst->at(0)) == ca->spilled_tmp.at(src->at(0)))
                    continue; // still conflict
            }
            break;
            }
            if (src)
            {
                for (auto &tmpid : *src)
                {
                    if (ca->spilled_tmp.count(tmpid))
                    {
                        if (tmpid < 0)
                        {
                            if (!stk_map->count(ca->spilled_tmp.at(tmpid)))
                            {
                                stk_map->insert({ca->spilled_tmp.at(tmpid),
                                                 newLocalTempVarID()});
                            }
                            int stk_tmp = stk_map->at(ca->spilled_tmp.at(tmpid));
                            int new_stk_tmp = ~newLocalTempVarID();
                            stk_use->insert(new_stk_tmp);
                            tmpid = new_stk_tmp;
                            ans->push_back(new ASM::Operand("vmov $d0, $s0", {new_stk_tmp}, {stk_tmp}, {}));
                        }
                        else
                        {
                            int offset;
                            if (!stk_map->count(ca->spilled_tmp.at(tmpid)))
                            {
                                _this_stk_size += 4;
                                offset = _this_stk_size;
                                stk_map->insert({ca->spilled_tmp.at(tmpid),
                                                 offset});
                            }
                            else
                            {
                                offset = stk_map->at(ca->spilled_tmp.at(tmpid));
                            }
                            int new_stk_tmp = newLocalTempVarID();
                            stk_use->insert(new_stk_tmp);
                            tmpid = new_stk_tmp;
                            // up to 8 MB
                            if (offset < 4 * 1024 && offset > -4 * 1024)
                            {
                                std::string s = "ldr $d0,[fp, #-" + std::to_string(offset) + "]";
                                ans->push_back(new ASM::Operand(s, {new_stk_tmp}, {}, {}));
                            }
                            else
                            {
                                int constTemp = newLocalTempVarID();
                                ans->push_back(new ASM::Operand("$mov $d0, #" + std::to_string(offset), {constTemp},
                                                                {}, {}));
                                ans->push_back(new ASM::Operand("ldr $d0,[$s0, -$s1]", {new_stk_tmp}, {11, constTemp},
                                                                {}));
                            }
                        }
                    }
                }
            }

            ans->push_back(in);

            if (dst)
            {
                for (auto &tmpid : *dst)
                {
                    if (ca->spilled_tmp.count(tmpid))
                    {
                        if (tmpid < 0)
                        {
                            if (!stk_map->count(ca->spilled_tmp.at(tmpid)))
                            {
                                stk_map->insert({ca->spilled_tmp.at(tmpid),
                                                 newLocalTempVarID()});
                            }
                            int stk_tmp = stk_map->at(ca->spilled_tmp.at(tmpid));
                            int new_stk_tmp = ~newLocalTempVarID();
                            stk_use->insert(new_stk_tmp);
                            tmpid = new_stk_tmp;
                            ans->push_back(new ASM::Operand("vmov $d0, $s0", {stk_tmp}, {new_stk_tmp}, {}));
                        }
                        else
                        {
                            int offset;
                            if (!stk_map->count(ca->spilled_tmp.at(tmpid)))
                            {
                                _this_stk_size += 4;
                                offset = _this_stk_size;
                                stk_map->insert({ca->spilled_tmp.at(tmpid),
                                                 offset});
                            }
                            else
                            {
                                offset = stk_map->at(ca->spilled_tmp.at(tmpid));
                            }

                            int new_stk_tmp = newLocalTempVarID();
                            stk_use->insert(new_stk_tmp);
                            tmpid = new_stk_tmp;
                            if (offset < 4 * 1024 && offset > -4 * 1024)
                            {
                                std::string s = "str $s0,[fp, #-" + std::to_string(offset) + "]";
                                ans->push_back(new ASM::Operand(s, {}, {new_stk_tmp}, {}));
                            }
                            else
                            {
                                int constTemp = newLocalTempVarID();
                                ans->push_back(new ASM::Operand("$mov $d0, #" + std::to_string(offset),
                                                                {constTemp}, {},
                                                                {}));
                                ans->push_back(new ASM::Operand("str $s0,[$s1, -$s2]",
                                                                {}, {new_stk_tmp, 11, constTemp},
                                                                {}));
                            }
                        }
                    }
                }
            }
        }
        return ans;
    }

    static std::vector<ASM::Instruction *> *trans_from_ca(std::vector<ASM::Instruction *> *is,
                                                          std::unordered_map<int, int> *tmp2color,
                                                          bool is_float)
    {
        assert(is);
        std::vector<ASM::Instruction *> *ans = new std::vector<ASM::Instruction *>();
        for (ASM::Instruction *in : *is)
        {
            switch (in->kind)
            {
            case ASM::InstrType::label:
                break;
            case ASM::InstrType::operand:
            {
                ASM::Operand *ins = static_cast<ASM::Operand *>(in);
                // ins->print();
                int idx = 0, cnt = 0;
                while ((idx = ins->asm_stm.find("$d", idx)) != std::string::npos)
                {
                    if (is_float && ins->dst[cnt] < 0 ||
                        !is_float && ins->dst[cnt] >= 0)
                        ins->dst[cnt] = tmp2color->at(ins->dst[cnt]);
                    idx++;
                    cnt++;
                }
                idx = 0, cnt = 0;
                while ((idx = ins->asm_stm.find("$s", idx)) != std::string::npos)
                {
                    if (is_float && ins->src[cnt] < 0 ||
                        !is_float && ins->src[cnt] >= 0)
                        ins->src[cnt] = tmp2color->at(ins->src[cnt]);
                    idx++;
                    cnt++;
                }
                // ins->print();
            }
            break;
            case ASM::InstrType::move:
            {
                ASM::Move *ins = static_cast<ASM::Move *>(in);
                int idx = 0, cnt = 0;
                while ((idx = ins->asm_stm.find("$d", idx)) != std::string::npos)
                {
                    if (is_float && ins->dst[cnt] < 0 ||
                        !is_float && ins->dst[cnt] >= 0)
                        ins->dst[cnt] = tmp2color->at(ins->dst[cnt]);
                    idx++;
                    cnt++;
                }
                idx = 0, cnt = 0;
                while ((idx = ins->asm_stm.find("$s", idx)) != std::string::npos)
                {
                    if (is_float && ins->src[cnt] < 0 ||
                        !is_float && ins->src[cnt] >= 0)
                        ins->src[cnt] = tmp2color->at(ins->src[cnt]);
                    idx++;
                    cnt++;
                }
                if (ins->dst.at(0) == ins->src.at(0))
                    continue; // mov r0, r0
            }
            break;
            }
            ans->push_back(in);
        }
        return ans;
    }

    /**
     * R11 frame pointer
     * R13 stack pointer
     */
    static std::vector<ASM::Instruction *> *wrap_func(std::vector<ASM::Instruction *> *is)
    {
        assert(is);
        std::vector<ASM::Instruction *> *ans = new std::vector<ASM::Instruction *>();
        ans->push_back(is->at(0)); // label
        ans->push_back(new ASM::Operand("stmdb sp, {r4, r5, r6, r7, r8, r9, r10, fp, lr}",
                                        std::vector<int>(), std::vector<int>(), std::vector<std::string>()));
        ans->push_back(new ASM::Operand("mov fp,sp",
                                        std::vector<int>(), std::vector<int>(), std::vector<std::string>()));

        // allocate stack size to store tmp var
        for (int i = 0; i < 4; i++)
        {
            if ((_this_stk_size & (0xff << (i * 8))) == 0)
                continue;
            std::string s1 = "sub sp, sp, #" +
                             std::to_string(_this_stk_size & (0xff << (i * 8)));
            ans->push_back(new ASM::Operand(s1, {}, {}, {}));
        }

        for (int i = 1; i < is->size(); i++)
        {
            ans->push_back(is->at(i));
        }
        ans->push_back(new ASM::Operand("mov sp,fp",
                                        std::vector<int>(), std::vector<int>(), std::vector<std::string>()));
        ans->push_back(new ASM::Operand("ldmdb sp, {r4, r5, r6, r7, r8, r9, r10, fp, lr}",
                                        std::vector<int>(), std::vector<int>(), std::vector<std::string>()));
        ans->push_back(new ASM::Operand("bx lr",
                                        std::vector<int>(), std::vector<int>(), std::vector<std::string>()));
        return ans;
    }

    static void AllocaReg(std::vector<ASM::Instruction *> *is, int stksize)
    {
        _this_stk_size = stksize;
        std::unordered_map<int, int> *stk_map = new std::unordered_map<int, int>();
        std::set<int> *stk_use = new std::set<int>();
        for (;;)
        {
            FLOWGRAPH::FlowPragh *fg = new FLOWGRAPH::FlowPragh(is);
            LIVEVAR::LiveVariableAnalysis *lva = new LIVEVAR::LiveVariableAnalysis(fg);
            CONFLICTGRAPH::ConflictGraph *cg = new CONFLICTGRAPH::ConflictGraph(lva, true);
            COLOR::COLOR_Assign *ca = new COLOR::COLOR_Assign(cg, stk_use);
            if (!ca->spilled_tmp.empty())
                is = gen_spill(is, ca, stk_map, stk_use);
            else
            {
                is = trans_from_ca(is, &ca->tmp2color, true);
                delete ca;
                break;
            }
            delete ca;
        }
        for (;;)
        {
            FLOWGRAPH::FlowPragh *fg = new FLOWGRAPH::FlowPragh(is); // pass
            LIVEVAR::LiveVariableAnalysis *lva = new LIVEVAR::LiveVariableAnalysis(fg);
            CONFLICTGRAPH::ConflictGraph *cg = new CONFLICTGRAPH::ConflictGraph(lva, false);
            COLOR::COLOR_Assign *ca = new COLOR::COLOR_Assign(cg, stk_use);
            if (!ca->spilled_tmp.empty())
                is = gen_spill(is, ca, stk_map, stk_use);
            else
            {
                is = trans_from_ca(is, &ca->tmp2color, false);
                delete ca;
                break;
            }
            delete ca;
        }
        is = wrap_func(is);
        for (auto it : *is)
            it->print();
        return;
    }
}