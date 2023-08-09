#include <stdio.h>
#include <string>
#include <iostream>
#include <sstream>
// #include "frontend/parser.hpp"
#include "structure/ast.h"
#include "structure/LIR.h"
#include "util/linearize.h"
#include "backend/regalloc.h"
#include "opt/funcinline.h"

#include "opt/SSAIR.h"
#include "opt/DCE.h"

extern CompRoot *root;

extern int yyparse();

std::vector<ASM::Instruction *> *addNULLStm(std::vector<ASM::Instruction *> *is, bool is_void)
{
    if (!is_void)
    { // r0 as in
        is->push_back(new ASM::Operand("", {}, {0}, {}));
    }
    return is;
}

int main(int argc, char *argv[])
{
    std::cerr << "Compile Begin ... " << std::endl;

    char *sy_input = 0;
    char *sy_output = 0;

    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] == '-' && argv[i][1] == 'o')
        {
            sy_output = argv[i + 1];
            break;
        }
    }

    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] != '-' && argv[i] != sy_output)
        {
            sy_input = argv[i];
        }
    }
    assert(sy_input);
    assert(sy_output);
    freopen(sy_input, "r", stdin);
    freopen(sy_output, "w+", stdout);
    yyparse();
    // freopen("src/testcase/test.sy", "r", stdin);
    // freopen("src/testcase/cmp.s", "w+", stdout);
    // yyparse();

    EntryTable<FuncEntry *> *func_table = withLibFunc();
    EntryTable<Entry *> *var_table = new EntryTable<Entry *>();
    LIR::ASTVisitor *v = new LIR::ASTVisitor(func_table, var_table);

    std::ostringstream *global_var_out = new std::ostringstream(),
                       *global_arr_out = new std::ostringstream();

    std::vector<std::string> func;
    std::unordered_map<std::string, std::vector<LIR::Stm *> *> func_ir;

    std::cerr << std::endl
              << "RAW IR   ..." << std::endl;

    std::cout << ".arch armv7ve\n.arm\n.global main\n";

    for (CompUnit *cu : root->comp_list)
    {
        LIR::Stm *si = cu->accept(*v);

        // ignore global var def
        if (typeid(*cu) != typeid(AST::FuncDef))
            continue;

        std::string func_name = *static_cast<AST::FuncDef *>(cu)->func_id->str;
        assert(v->funcEntryTable->existEntry(func_name));
        std::vector<LIR::Stm *> *i = Linearize(si);
        if (func_name == "main")
        {
            std::vector<LIR::Stm *> *g_init = handleGlobalVar(global_var_out,
                                                              global_arr_out,
                                                              v->varEntryTable);
            i->insert(i->begin() + 1, g_init->begin(), g_init->end());
        }
        i = Linearize(r_allocate_temp_var(i));
        showIR(i);

        func.push_back(func_name);
        func_ir.insert({func_name, i});

        if (func_name == "main")
        {
            std::cout << global_var_out->str();
            std::cout << global_arr_out->str();
            break;
        }
    }

    std::cout << ".text\n";

    // for (std::string func_name : func)
    // {
    //     IRBlock::BlockController *bc = IRBlock::genBlock(func_ir[func_name], func_name);
    //     LIR::StmLinkedList *ir = IRBlock::reorder(bc);
    //     std::cerr << std::endl
    //               << "IR after reordered !!!" << std::endl
    //               << std::endl;
    //     showIR(ir);
    //     REGALLOC::AllocaReg(addNULLStm(&LIR::ir2asm(ir)->i_list, func_table->findEntry(func_name)->type->func_return_type->t_kind == T_KIND::VOID ),
    //                         func_table->findEntry(func_name)->stack_size);
    // }

    // FI::FuncInline *fin = new FI::FuncInline(var_table, func_table, func, func_ir);
    // auto func_name_st = fin->do_func_inline();
    // for (std::string func_name : func_name_st)
    // {
    //     auto ir_ll = fin->func_info[func_name].ir_ll;
    //     IRBlock::BlockController *bc = IRBlock::genBlock(ir_ll, func_name);
    //     LIR::StmLinkedList *ir = IRBlock::reorder(bc);
    //     std::cerr << std::endl
    //               << "IR after reordered !!!" << std::endl
    //               << std::endl;
    //     showIR(ir);
    //     REGALLOC::AllocaReg(addNULLStm(&LIR::ir2asm(ir)->i_list, fin->func_info[func_name].is_void),
    //                         fin->func_info[func_name].stack_size);
    // }

    FI::FuncInline *fin = new FI::FuncInline(var_table, func_table, func, func_ir);
    auto func_name_st = fin->do_func_inline();
    for (std::string func_name : func_name_st)
    {
        auto ir_ll = fin->func_info[func_name].ir_ll;
        IRBlock::BlockController *bc = IRBlock::genBlock(ir_ll, func_name);

        SSA::SSAIR *ssa = new SSA::SSAIR(*bc);
        new DCE(ssa);
        bc = ssa->SSA2blks();

        LIR::StmLinkedList *ir = IRBlock::reorder(bc);
        std::cerr << std::endl
                  << "IR after reordered !!!" << std::endl
                  << std::endl;
        showIR(ir);
        REGALLOC::AllocaReg(addNULLStm(&LIR::ir2asm(ir)->i_list, fin->func_info[func_name].is_void),
                            fin->func_info[func_name].stack_size);
    }

    std::cerr << "Compile End ... " << std::endl;
}