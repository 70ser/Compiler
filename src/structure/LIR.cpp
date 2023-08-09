#include "LIR.h"
#include "ast.h"

#include "../util/label.h"
#include "../util/cast.h"
#include "../util/symbolize.h"
#include "../util/patch.h"
#include "../util/linearize.h"
#include <cstring>
#include <iostream>
#include <set>

using namespace LIR;

/* Traverse all CompUnits, use the accept() method,
 * pass in an instance of this class, and an IR will be generated
 * exp:
 *     for (const auto &cu : root->comp_list) { cu->accept(irg); }
 *
 */

Stm *ASTVisitor::visitConstDeclNode(AST::ConstDecl &node)
{
    StmList *stm_list = new StmList();
    for (auto it : *(node.defs))
    {
        decl_def_type = node.b_type;
        stm_list->add2StmList((it->accept(*this)));
    }
    return stm_list;
}

Stm *ASTVisitor::visitConstDefNode(AST::ConstDef &node)
{
    bool is_global = (prev_name == "");
    switch (decl_def_type)
    {
    case AST::base_t::INT:
    {
        if (node.array_index_list->empty())
        { // int
            BaseUnit unit = node.const_init_val->accept(*this);
            assert(unit.unit_res_type->var_val); // confirm init already
            int *ans = new int(*unit.unit_res_type->var_val);
            if (unit.unit_res_type->t_kind == T_KIND::FLOAT)
                *ans = digit_f2i(*unit.unit_res_type->var_val);
            varEntryTable->enterEntry(*(node.id->str),
                                      new GlobalvarEntry(newIntType(ans, true),
                                                         newGloablVarLabel()));
            return NOPStm();
        }
        else
        { // int[]
            Type *ty_head = newIntType(new int(0), false);
            for (int i = node.array_index_list->size() - 1; i >= 0; i--)
            {
                AST::Exp *e = node.array_index_list->at(i);
                BaseUnit unit = e->accept(*this);
                // int a[2][3] = init;
                ty_head = newArrayType(ty_head, *unit.unit_res_type->var_val, false);
            }
            if (is_global)
            {
                std::string arr_begin_label = newGloablVarLabel();
                arr_base = new NameExp(arr_begin_label);
                arr_head = ty_head;
                node.const_init_val->calArray(*this);
                arr_base = nullptr;
                varEntryTable->enterEntry(*(node.id->str),
                                          new GlobalvarEntry(ty_head,
                                                             arr_begin_label));
                return NOPStm();
            }
            else
            {
                int local_temp_var_id = newLocalTempVarID();

                FuncEntry *func_entry = funcEntryTable->findEntry(prev_name);
                func_entry->stack_size += ty_head->array_size * 4;

                StmList *stm_list = new StmList();
                Stm *arr_addr_in_sk = new LIR::MoveStm(new LIR::TempVarExp(local_temp_var_id),
                                                       new BinOpExp(LIR::BIN_OP::T_plus, new LIR::TempVarExp(11), new LIR::ConstExp(-func_entry->stack_size)));
                stm_list->add2StmList(arr_addr_in_sk);
                std::vector<LIR::Exp *> params;
                params.push_back(new LIR::TempVarExp(local_temp_var_id));
                params.push_back(new LIR::ConstExp(0));
                params.push_back(new LIR::ConstExp(ty_head->array_size * 4));
                stm_list->add2StmList(new LIR::ExpStm(new LIR::CallExp("memset", params)));
                arr_base = new LIR::TempVarExp(local_temp_var_id);
                arr_head = ty_head;
                BaseUnit unit = node.const_init_val->calArray(*this);
                arr_base = nullptr;
                varEntryTable->enterEntry(*(node.id->str),
                                          new LocalVarEntry(ty_head, local_temp_var_id));
                stm_list->add2StmList(new LIR::ExpStm(unit.decode2Exp()));
                return stm_list;
            }
        }
    }
    break;
    case AST::base_t::FLOAT:
    {
        if (node.array_index_list->empty())
        {
            BaseUnit unit = node.const_init_val->accept(*this);
            assert(unit.unit_res_type->var_val);
            int *ans = new int(*unit.unit_res_type->var_val);
            if (unit.unit_res_type->t_kind == T_KIND::INT)
                *ans = digit_i2f(*unit.unit_res_type->var_val);
            varEntryTable->enterEntry(*(node.id->str),
                                      new GlobalvarEntry(newFloatType(ans, true),
                                                         newGloablVarLabel()));
            return NOPStm();
        }
        else
        {
            Type *ty_head = newFloatType(new int(0), false);
            for (int i = node.array_index_list->size() - 1; i >= 0; i--)
            {
                AST::Exp *e = node.array_index_list->at(i);
                BaseUnit unit = e->accept(*this);
                // int a[2][3] = init;
                ty_head = newArrayType(ty_head, *unit.unit_res_type->var_val, false);
            }
            if (is_global)
            {
                std::string arr_begin_label = newGloablVarLabel();
                arr_base = new NameExp(arr_begin_label);
                arr_head = ty_head;
                node.const_init_val->calArray(*this);
                arr_base = nullptr;
                varEntryTable->enterEntry(*(node.id->str),
                                          new GlobalvarEntry(ty_head,
                                                             arr_begin_label));
                return NOPStm();
            }
            else
            {
                int local_temp_var_id = newLocalTempVarID();

                FuncEntry *func_entry = funcEntryTable->findEntry(prev_name);
                func_entry->stack_size += ty_head->array_size * 4;

                StmList *stm_list = new StmList();
                Stm *arr_addr_in_sk = new LIR::MoveStm(new LIR::TempVarExp(local_temp_var_id),
                                                       new BinOpExp(LIR::BIN_OP::T_plus, new LIR::TempVarExp(11), new LIR::ConstExp(-func_entry->stack_size)));
                stm_list->add2StmList(arr_addr_in_sk);
                std::vector<LIR::Exp *> params;
                params.push_back(new LIR::TempVarExp(local_temp_var_id));
                params.push_back(new LIR::ConstExp(0));
                params.push_back(new LIR::ConstExp(ty_head->array_size * 4));
                stm_list->add2StmList(new LIR::ExpStm(new LIR::CallExp("memset", params)));
                arr_base = new LIR::TempVarExp(local_temp_var_id);
                arr_head = ty_head;
                BaseUnit unit = node.const_init_val->calArray(*this);
                arr_base = nullptr;
                varEntryTable->enterEntry(*(node.id->str),
                                          new LocalVarEntry(ty_head, local_temp_var_id));
                stm_list->add2StmList(new LIR::ExpStm(unit.decode2Exp()));
                return stm_list;
            }
        }
    }
    break;
    default:
        assert(0);
    }
}

Stm *ASTVisitor::visitVarDeclNode(AST::VarDecl &node)
{
    StmList *stm_list = new StmList();
    for (auto it : *(node.defs))
    {
        decl_def_type = node.b_type;
        stm_list->add2StmList((it->accept(*this)));
    }
    return stm_list;
}

Stm *ASTVisitor::visitVarDefNode(AST::VarDef &node)
{
    bool is_global = (prev_name == "");
    switch (decl_def_type)
    {
    case AST::base_t::INT:
    {
        if (is_global)
        {
            if (node.array_index_list->empty())
            { // int
                if (node.init_val)
                {
                    BaseUnit unit = node.init_val->accept(*this);
                    assert(unit.unit_res_type->var_val);
                    int *ans = new int(*unit.unit_res_type->var_val);
                    if (unit.unit_res_type->t_kind == T_KIND::FLOAT)
                        *ans = digit_f2i(*unit.unit_res_type->var_val);
                    varEntryTable->enterEntry(*(node.id->str),
                                              new GlobalvarEntry(newIntType(ans, false),
                                                                 newGloablVarLabel()));
                }
                else
                {
                    varEntryTable->enterEntry(*(node.id->str),
                                              new GlobalvarEntry(newIntType(new int(0), false),
                                                                 newGloablVarLabel()));
                }
                return NOPStm();
            }
            else
            { // int[]
                Type *ty_head = newIntType(new int(0), false);
                for (int i = node.array_index_list->size() - 1; i >= 0; i--)
                {
                    AST::Exp *e = node.array_index_list->at(i);
                    BaseUnit unit = e->accept(*this);
                    // int a[2][3] = init;
                    ty_head = newArrayType(ty_head, *unit.unit_res_type->var_val, false);
                }
                std::string arr_begin_label = newGloablVarLabel();
                if (node.init_val)
                {
                    arr_base = new NameExp(arr_begin_label);
                    arr_head = ty_head;
                    node.init_val->calArray(*this);
                    arr_base = nullptr;
                    varEntryTable->enterEntry(*(node.id->str),
                                              new GlobalvarEntry(ty_head,
                                                                 arr_begin_label));
                }
                else
                {
                    ty_head->var_val = new int[ty_head->array_size];
                    std::memset(ty_head->var_val, 0, ty_head->array_size * sizeof(int));
                    varEntryTable->enterEntry(*(node.id->str),
                                              new GlobalvarEntry(ty_head,
                                                                 arr_begin_label));
                }

                return NOPStm();
            }
        }
        else
        {
            if (node.array_index_list->empty())
            { // int lcoal
                if (node.init_val)
                {
                    BaseUnit unit = node.init_val->accept(*this);
                    // assert(unit.unit_res_type->var_val.i_val);
                    int *ans = new int(*unit.unit_res_type->var_val);
                    LIR::Exp *ret_exp = unit.decode2Exp();
                    if (unit.unit_res_type->t_kind == T_KIND::FLOAT)
                    {
                        *ans = digit_f2i(*unit.unit_res_type->var_val);
                        ret_exp = fExp2iExp(ret_exp);
                    }
                    int local_temp_var_id = newLocalTempVarID();
                    varEntryTable->enterEntry(*(node.id->str),
                                              new LocalVarEntry(newIntType(ans, false), local_temp_var_id));
                    return new LIR::MoveStm(new LIR::TempVarExp(local_temp_var_id), ret_exp);
                }
                else
                {
                    int local_temp_var_id = newLocalTempVarID();
                    varEntryTable->enterEntry(*(node.id->str),
                                              new LocalVarEntry(newIntType(new int(0), false), local_temp_var_id));
                    return NOPStm();
                }
            }
            else
            { // int[] local
                Type *ty_head = newIntType(new int(0), false);
                for (int i = node.array_index_list->size() - 1; i >= 0; i--)
                {
                    AST::Exp *e = node.array_index_list->at(i);
                    BaseUnit unit = e->accept(*this);
                    // int a[2][3] = init;
                    ty_head = newArrayType(ty_head, *unit.unit_res_type->var_val, false);
                }
                int local_temp_var_id = newLocalTempVarID();

                FuncEntry *func_entry = funcEntryTable->findEntry(prev_name);
                func_entry->stack_size += ty_head->array_size * 4;

                StmList *stm_list = new StmList();
                Stm *arr_addr_in_sk = new LIR::MoveStm(new LIR::TempVarExp(local_temp_var_id),
                                                       new BinOpExp(LIR::BIN_OP::T_plus, new LIR::TempVarExp(11), new LIR::ConstExp(-func_entry->stack_size)));
                stm_list->add2StmList(arr_addr_in_sk);
                if (node.init_val)
                {
                    std::vector<LIR::Exp *> params;
                    params.push_back(new LIR::TempVarExp(local_temp_var_id));
                    params.push_back(new LIR::ConstExp(0));
                    params.push_back(new LIR::ConstExp(ty_head->array_size * 4));
                    stm_list->add2StmList(new LIR::ExpStm(new LIR::CallExp("memset", params)));

                    arr_base = new LIR::TempVarExp(local_temp_var_id);
                    arr_head = ty_head;
                    BaseUnit unit = node.init_val->calArray(*this);
                    arr_base = nullptr;
                    varEntryTable->enterEntry(*(node.id->str),
                                              new LocalVarEntry(ty_head, local_temp_var_id));
                    stm_list->add2StmList(new LIR::ExpStm(unit.decode2Exp()));
                    return stm_list;
                }
                else
                {
                    ty_head->var_val = new int[ty_head->array_size];
                    std::memset(ty_head->var_val, 0, ty_head->array_size * sizeof(int));
                    varEntryTable->enterEntry(*(node.id->str),
                                              new LocalVarEntry(ty_head, local_temp_var_id));
                    return stm_list;
                }
            }
        }
    }
    break;
    case AST::base_t::FLOAT:
    {
        if (is_global)
        {
            if (node.array_index_list->empty())
            { // float
                if (node.init_val)
                {
                    BaseUnit unit = node.init_val->accept(*this);
                    // assert(unit.unit_res_type->var_val.f_val);
                    int *ans = new int(*unit.unit_res_type->var_val);
                    if (unit.unit_res_type->t_kind == T_KIND::INT)
                        *ans = digit_f2i(*unit.unit_res_type->var_val);
                    varEntryTable->enterEntry(*(node.id->str),
                                              new GlobalvarEntry(newFloatType(ans, false),
                                                                 newGloablVarLabel()));
                }
                else
                {
                    varEntryTable->enterEntry(*(node.id->str),
                                              new GlobalvarEntry(newFloatType(new int(0), false),
                                                                 newGloablVarLabel()));
                }
                return NOPStm();
            }
            else
            { // float[]
                Type *ty_head = newFloatType(new int(0), false);
                for (int i = node.array_index_list->size() - 1; i >= 0; i--)
                {
                    AST::Exp *e = node.array_index_list->at(i);
                    BaseUnit unit = e->accept(*this);
                    // int a[2][3] = init;
                    ty_head = newArrayType(ty_head, *unit.unit_res_type->var_val, false);
                }
                std::string arr_begin_label = newGloablVarLabel();
                if (node.init_val)
                {

                    arr_base = new NameExp(arr_begin_label);
                    arr_head = ty_head;
                    node.init_val->calArray(*this);
                    arr_base = nullptr;
                    varEntryTable->enterEntry(*(node.id->str),
                                              new GlobalvarEntry(ty_head,
                                                                 arr_begin_label));
                }
                else
                {
                    ty_head->var_val = new int[ty_head->array_size];
                    std::memset(ty_head->var_val, 0, ty_head->array_size * sizeof(int));
                    varEntryTable->enterEntry(*(node.id->str),
                                              new GlobalvarEntry(ty_head,
                                                                 arr_begin_label));
                }
                arr_base = nullptr;
                return NOPStm();
            }
        }
        else
        {
            if (node.array_index_list->empty())
            { // float local
                if (node.init_val)
                {
                    BaseUnit unit = node.init_val->accept(*this);
                    // assert(unit.unit_res_type->var_val.i_val);
                    int *ans = new int(*unit.unit_res_type->var_val);
                    LIR::Exp *ret_exp = unit.decode2Exp();
                    if (unit.unit_res_type->t_kind == T_KIND::INT)
                    {
                        *ans = digit_i2f(*unit.unit_res_type->var_val);
                        ret_exp = iExp2fExp(ret_exp);
                    }
                    int local_temp_var_id = newLocalTempVarID();
                    varEntryTable->enterEntry(*(node.id->str),
                                              new LocalVarEntry(newFloatType(ans, false), local_temp_var_id));
                    return new LIR::MoveStm(new LIR::TempVarExp(local_temp_var_id), ret_exp);
                }
                else
                {
                    int local_temp_var_id = newLocalTempVarID();
                    varEntryTable->enterEntry(*(node.id->str),
                                              new LocalVarEntry(newFloatType(new int(0), false), local_temp_var_id));
                    return NOPStm();
                }
            }
            else
            { // float[] local
                Type *ty_head = newFloatType(new int(0), false);
                for (int i = node.array_index_list->size() - 1; i >= 0; i--)
                {
                    AST::Exp *e = node.array_index_list->at(i);
                    BaseUnit unit = e->accept(*this);
                    // int a[2][3] = init;
                    ty_head = newArrayType(ty_head, *unit.unit_res_type->var_val, false);
                }
                int local_temp_var_id = newLocalTempVarID();
                FuncEntry *func_entry = funcEntryTable->findEntry(prev_name);
                func_entry->stack_size += ty_head->array_size * 4;

                StmList *stm_list = new StmList();
                Stm *arr_addr_in_sk = new LIR::MoveStm(new LIR::TempVarExp(local_temp_var_id),
                                                       new BinOpExp(LIR::BIN_OP::T_plus, new LIR::TempVarExp(11), new LIR::ConstExp(-func_entry->stack_size)));
                stm_list->add2StmList(arr_addr_in_sk);
                if (node.init_val)
                {
                    std::vector<LIR::Exp *> params;
                    params.push_back(new LIR::TempVarExp(local_temp_var_id));
                    params.push_back(new LIR::ConstExp(0));
                    params.push_back(new LIR::ConstExp(ty_head->array_size * 4));
                    stm_list->add2StmList(new LIR::ExpStm(new LIR::CallExp("memset", params)));

                    arr_base = new LIR::TempVarExp(local_temp_var_id);
                    arr_head = ty_head;
                    BaseUnit unit = node.init_val->calArray(*this);
                    arr_base = nullptr;
                    varEntryTable->enterEntry(*(node.id->str),
                                              new LocalVarEntry(ty_head, local_temp_var_id));
                    stm_list->add2StmList(new LIR::ExpStm(unit.decode2Exp()));
                    return stm_list;
                }
                else
                {
                    ty_head->var_val = new int[ty_head->array_size];
                    std::memset(ty_head->var_val, 0, ty_head->array_size * sizeof(float));
                    varEntryTable->enterEntry(*(node.id->str),
                                              new LocalVarEntry(ty_head, local_temp_var_id));
                    return stm_list;
                }
            }
        }
    }
    break;
    default:
        assert(0);
    }
}

Stm *ASTVisitor::visitFuncDefNode(AST::FuncDef &node)
{
    Type *func_t = newFuncType(NULL, std::vector<Type *>());
    func_t->func_return_type = AST_T2IR_T(node.f_type);
    for (AST::Parameter *it : *(node.parameters))
    {
        if (it->array_index_list->empty())
        {
            func_t->func_param_type.push_back(AST_T2IR_T(it->b_type));
        }
        else
        { // arr
            Type *ty_head;
            bool is_int_arr = false;
            if (it->b_type == AST::base_t::INT)
            {
                ty_head = newIntType(new int(0), false);
                // is_int_arr = true;
            }
            else
            {
                ty_head = newFloatType(new int(0), false);
            }
            for (int i = it->array_index_list->size() - 1; i >= 0; i--)
            {
                // if (is_int_arr)
                ty_head = newArrayType(ty_head, 0, false);
                // else
                //     ty_head = newFloatArrayType(ty_head, 0, false);
            }
            // if (is_int_arr)
            // {
            ty_head->var_val = new int[ty_head->array_size];
            std::memset(ty_head->var_val, 0, ty_head->array_size * sizeof(int));
            // }
            // else
            // {
            //     ty_head->arr_val.f_arr_p = new float[ty_head->array_size];
            //     std::memset(ty_head->arr_val.f_arr_p, 0, ty_head->array_size * sizeof(float));
            // }
            func_t->func_param_type.push_back(ty_head);
        }
    }
    std::string func_name = *(node.func_id->str);
    prev_name = func_name;
    std::string func_label = func_name;
    funcEntryTable->enterEntry(func_label, new FuncEntry(func_t, func_label, 64));
    varEntryTable->beginFuncScope();
    LIR::StmList *stm_list = new StmList;
    stm_list->add2StmList(new LabelStm(func_label));

    int cnt = 0, stk_off = 0;

    /**
     * 1. if para's cnt < 4, it can be passed directly as a parameter using r0-r3
     *  example (in callee):
     *          push    {r7}
                sub     sp, sp, #20
                add     r7, sp, #0
                str     r0, [r7, #12]
                str     r1, [r7, #8]
                str     r2, [r7, #4]
                str     r3, [r7]
                ...

     * 2. else, stack must increase in caller to store these params
     * 3.       and callee can reach these params by sp
     *  example (in calller):
     *          ...
     *          ldr     r3, [r7, #8]    # r7 in caller might point to caleer's stack
                str     r3, [sp, #4]
                ldr     r3, [r7, #12]
                str     r3, [sp]

                ldr     r3, [r7, #16]   # using r0 - r4 to pass param<=4
                ldr     r2, [r7, #20]
                ldr     r1, [r7, #24]
                ldr     r0, [r7, #28]

                bl      add(int, int, int, int, int, int)


                (in callee):
                add(int, int, int, int, int, int):
                ...
                ...                     # might store r0 - r4 in local stack
                ...
                ldr     r3, [r7, #24]   # 20 + 4(r7 pushed)
                ldr     r3, [r7, #28]
                ...

        4. when call for a func,
                for 1., we can directly store params in r0 - r3
                for 2., we manage to store by stack (t11)
            note: 't11' is 'frame pointer'
    */

    for (AST::Parameter *it : *(node.parameters))
    {
        // push vars and increase func's stack
        decl_def_type = it->b_type;
        AST::VarDef(it->id, it->array_index_list, NULL, it->lineno).accept(*this);
        LocalVarEntry *entry = static_cast<LocalVarEntry *>(varEntryTable->findEntry(*(it->id->str)));
        if (cnt < 4)
        {
            //  LIR might be:
            //      mov t1001    t0
            stm_list->add2StmList(new MoveStm(new LIR::TempVarExp(entry->temp_id_int), new LIR::TempVarExp(cnt)));
        }
        else
        {
            //  LIR might be:
            //      mov  t1006   ( t11 add 0/4/8... )
            //      mov  t1004   Mem( t1006 )
            MemExp *mem_addr = new MemExp(
                new BinOpExp(LIR::BIN_OP::T_plus,
                             new LIR::TempVarExp(11), new LIR::ConstExp(stk_off)));
            stk_off += 4;
            stm_list->add2StmList(new MoveStm(new LIR::TempVarExp(entry->temp_id_int), mem_addr));
        }
        cnt++;
    }
    LIR::Stm *block_stm = node.block->accept(*this);
    varEntryTable->endFuncScope();
    prev_name = "";
    stm_list->add2StmList(new LabelStm("_FUNC_BEGIN_" + func_name));
    stm_list->add2StmList(block_stm);
    return stm_list;
}

Stm *ASTVisitor::visitBlockNode(AST::Block &node)
{
    varEntryTable->beginFuncScope();
    StmList *stm_list = new StmList();
    for (AST::BlockItem *&it : *(node.items))
    {
        stm_list->add2StmList((it->accept(*this)));
    }
    varEntryTable->endFuncScope();
    return stm_list;
}

Stm *ASTVisitor::visitAssignStmtNode(AST::AssignStmt &node)
{
    BaseUnit l = node.l_val->accept(*this);
    BaseUnit r = node.exp->accept(*this);
    LIR::Exp *lexp = l.decode2Exp();
    LIR::Exp *rexp = r.decode2Exp();
    T_KIND lk = l.unit_res_type->t_kind;
    T_KIND rk = r.unit_res_type->t_kind;
    rexp = align2LetfExp(rexp, lk, rk);
    return new MoveStm(lexp, rexp);
}

Stm *ASTVisitor::visitExpStmtNode(AST::ExpStmt &node)
{
    return new ExpStm(node.exp->accept(*this).decode2Exp());
}

Stm *ASTVisitor::visitIfStmtNode(AST::IfStmt &node)
{
    Condition cond = node.conditon->accept(*this).decode2Condtion();
    std::string t_label = newLabel(), f_label = newLabel(), done_label = newLabel();
    // condtion -> `t_label` if true / 'f_label'
    doPatch(cond.trues, t_label);
    doPatch(cond.falses, f_label);
    Stm *if_stm = node.if_part->accept(*this);
    Stm *else_stm = NOPStm();
    if (node.else_part)
        else_stm = node.else_part->accept(*this);
    /*
    1. condtion
    2. t_label
    3. if_stm -> `done`
    4. f_label
    5. else_stm
    6. done
    */
    StmList *stm_list = new StmList();
    stm_list->add2StmList(cond.condition_stm);
    stm_list->add2StmList(new LabelStm(t_label));
    stm_list->add2StmList(if_stm);
    stm_list->add2StmList(new JumpStm(done_label));
    stm_list->add2StmList(new LabelStm(f_label));
    stm_list->add2StmList(else_stm);
    stm_list->add2StmList(new LabelStm(done_label));

    return stm_list;
}

Stm *ASTVisitor::visitWhileStmtNode(AST::WhileStmt &node)
{
    Condition cond1 = node.condition->accept(*this).decode2Condtion();
    Condition cond2 = node.condition->accept(*this).decode2Condtion();
    std::string test1 = newLabel(), begin = newLabel(), test2 = newLabel(), done = newLabel();
    // cond1 -> begin if true / done
    doPatch(cond1.trues, begin);
    doPatch(cond1.falses, done);
    doPatch(cond2.trues, begin);
    doPatch(cond2.falses, done);
    /*
    1. test1(cond1)
    2. begin
    3. loop
    4. test2(cond2)
    5. done
    */
    StmList *stm_list = new StmList();
    stm_list->add2StmList(new LabelStm(test1));
    stm_list->add2StmList(cond1.condition_stm);
    stm_list->add2StmList(new LabelStm(begin));


    const auto con = this->conLabel;
    const auto brk = this->brkLabel;
    this->conLabel = test2;
    this->brkLabel = done;
    stm_list->add2StmList(node.loop->accept(*this));
    this->conLabel = con;
    this->brkLabel = brk;

    stm_list->add2StmList(new LabelStm(test2));
    stm_list->add2StmList(cond2.condition_stm);
    stm_list->add2StmList(new LabelStm(done));
    return stm_list;
}

Stm *ASTVisitor::visitBreakStmtNode(AST::BreakStmt &node)
{
    return new JumpStm(brkLabel);
}

Stm *ASTVisitor::visitContinueStmtNode(AST::ContinueStmt &node)
{
    return new JumpStm(conLabel);
}

Stm *ASTVisitor::visitReturnStmtNode(AST::ReturnStmt &node)
{
    if (node.exp == nullptr)
    {
        return new JumpStm(prev_name + "_RETURN");
    }
    else
    {
        T_KIND func_ret_type = funcEntryTable->findEntry(prev_name)->type->func_return_type->t_kind;
        BaseUnit unit = node.exp->accept(*this);
        T_KIND exp_type;
        exp_type = unit.unit_res_type->t_kind;
        LIR::Exp *exp = unit.decode2Exp();
        exp = align2LetfExp(exp, func_ret_type, exp_type);
        StmList *stm_list = new StmList();
        // return as temp_0
        stm_list->add2StmList(new LIR::MoveStm(new LIR::TempVarExp(0), exp));
        stm_list->add2StmList(new JumpStm(prev_name + "_RETURN"));
        return stm_list;
    }
}

BaseUnit ASTVisitor::visitLValStmtNode(AST::LVal &node)
{
    std::string varname = *(node.id->str);
    assert(varEntryTable->existEntry(varname));
    Entry *var_entry = varEntryTable->findEntry(varname);
    LIR::Exp *varExp;
    if (var_entry->kind == EntryKind::GLOBAL)
        varExp = new LIR::NameExp(static_cast<GlobalvarEntry *>(var_entry)->label);
    else
        varExp = new LIR::TempVarExp(static_cast<LocalVarEntry *>(var_entry)->temp_id_int);

    Type *emb_var_type = var_entry->type;
    if (node.array_index_list->empty())
    { // not array
        if (emb_var_type->is_const)
        {
            // if (emb_var_type->t_kind == T_KIND::INT)
            return BaseUnit(emb_var_type, new ConstExp(*emb_var_type->var_val));
            // if (emb_var_type->t_kind == T_KIND::FLOAT)
            //     return BaseUnit(emb_var_type, new ConstExp(emb_var_type->var_val.f_val));
        }
        else
        {
            if (var_entry->kind == EntryKind::GLOBAL)
            {
                if (emb_var_type->t_kind == T_KIND::ARRAY)
                    return BaseUnit(emb_var_type, varExp);
                else
                    return BaseUnit(emb_var_type, new MemExp(varExp));
            }
            else
            {
                return BaseUnit(emb_var_type, varExp);
            }
        }
    }
    else
    { // array
        LIR::Exp *e = new LIR::ConstExp(0);
        int total_off = 0;
        /* LVal → Ident {'[' Exp ']'} */
        for (AST::Exp *it : *(node.array_index_list))
        {
            // no need to think of `float`
            BaseUnit unit = it->accept(*this);

            int i_dim = emb_var_type->array_dim;
            int off_in_i_dim = *unit.unit_res_type->var_val;

            // exp = (exp * dim) + unit_exp
            e = new BinOpExp(LIR::BIN_OP::T_plus,
                             new BinOpExp(LIR::BIN_OP::T_mul, new ConstExp(i_dim), e),
                             unit.decode2Exp());

            // a[b[0][1]][exp][...]
            total_off = total_off * i_dim + off_in_i_dim;

            emb_var_type = emb_var_type->element_type;
        }
        Type *l_type;
        int var_arr_size = var_entry->type->array_size;
        if (emb_var_type->t_kind == T_KIND::INT)
        {
            l_type = newIntType(new int(var_entry->type->var_val[(total_off % var_arr_size + var_arr_size) % var_arr_size]), false);
            // l_type = newIntType(new int(0), false);
        }
        else if (emb_var_type->t_kind == T_KIND::FLOAT)
        {
            l_type = newFloatType(new int(var_entry->type->var_val[(total_off % var_arr_size + var_arr_size) % var_arr_size]), false);
            // l_type = newFloatType(new int(0), false);
        }
        else if (emb_var_type->t_kind == T_KIND::ARRAY)
        {
            /**
             * example:
             *      int buf[2][100];
             *      int n = getarray(buf[0]);
             */
            l_type = emb_var_type;
            return BaseUnit(l_type, new LIR::BinOpExp(LIR::BIN_OP::T_plus, varExp,
                                                            new LIR::BinOpExp(LIR::BIN_OP::T_mul,
                                                                              new LIR::ConstExp(4 * emb_var_type->array_size),
                                                                              e)));
        }
        else
            assert(0);
        return BaseUnit(l_type,
                        new MemExp(new LIR::BinOpExp(LIR::BIN_OP::T_plus, varExp,
                                                     new LIR::BinOpExp(LIR::BIN_OP::T_mul, new LIR::ConstExp(4), e))));
    }
}

BaseUnit ASTVisitor::visitIntNumberStmtNode(AST::IntNumber &node)
{
    return BaseUnit(newIntType(new int(node.value), 0), new ConstExp(node.value));
}

BaseUnit ASTVisitor::visitFloatNumberNode(AST::FloatNumber &node)
{
    // float f = node.constval.val.f;
    return BaseUnit(newFloatType(new int(node.constval.val.i), 0), new ConstExp(node.constval.val.i));
}

BaseUnit ASTVisitor::visitUnaryExpNode(AST::UnaryExp &node)
{
    BaseUnit body = node.exp->accept(*this);
    switch (node.op)
    {
    case AST::unaryop_t::ADD:
        return body;
    case AST::unaryop_t::SUB:
    {
        LIR::Exp *lexp = new ConstExp(0);
        Type *l_type = newIntType(new int(0), false);

        LIR::Exp *rexp = body.decode2Exp();
        Type *r_type = body.unit_res_type;
        Type *ret_type = BinOpResType(l_type, r_type, LIR::BIN_OP::T_minus);
        LIR::Exp *exp = BinOpResExp(LIR::BIN_OP::T_minus, l_type, lexp, r_type, rexp);
        return BaseUnit(ret_type, exp);
    }
    case AST::unaryop_t::NOT:
    {
        LIR::Condition rexp = body.decode2Condtion();
        return BaseUnit(newIntType(new int(0), false),
                        LIR::Condition(rexp.falses, rexp.trues, rexp.condition_stm));
    }
    }
    return LIR::BaseUnit();
}

BaseUnit ASTVisitor::visitCallExpNode(AST::CallExp &node)
{
    assert(funcEntryTable->existEntry(*(node.func_id->str)));
    FuncEntry *func_entry = funcEntryTable->findEntry(*(node.func_id->str));
    std::vector<LIR::Exp *> args;
    assert(node.params->size() == func_entry->type->func_param_type.size());
    for (int i = 0; i < node.params->size(); i++)
    {
        T_KIND l_kind = func_entry->type->func_param_type[i]->t_kind;
        BaseUnit unit = node.params->at(i)->accept(*this);
        LIR::Exp *r_exp = unit.decode2Exp();
        T_KIND r_kind = unit.unit_res_type->t_kind;
        r_exp = align2LetfExp(r_exp, l_kind, r_kind);
        args.push_back(r_exp);
    }
    return BaseUnit(func_entry->type->func_return_type, new LIR::CallExp(func_entry->label, args));
}

BaseUnit ASTVisitor::visitMulExpNode(AST::MulExp &node)
{
    BaseUnit l_unit = node.lhs->accept(*this);
    BaseUnit r_unit = node.rhs->accept(*this);
    LIR::Exp *l_exp = l_unit.decode2Exp();
    LIR::Exp *r_exp = r_unit.decode2Exp();

    Type *lt = l_unit.unit_res_type;

    Type *rt = r_unit.unit_res_type;

    LIR::BIN_OP op;
    switch (node.op)
    {
    case AST::mul_t::MULT:
    {
        op = LIR::BIN_OP::T_mul;
        break;
    }
    case AST::mul_t::DIV:
    {
        op = LIR::BIN_OP::T_div;
        break;
    }
    case AST::mul_t::REM:
    {
        op = LIR::BIN_OP::T_mod;
        break;
    }
    default:
        assert(0);
    }
    Type *t = BinOpResType(lt, rt, op);
    LIR::Exp *exp = BinOpResExp(op, lt, l_exp, rt, r_exp);
    return BaseUnit(t, exp);
}

BaseUnit ASTVisitor::visitAddExpExpNode(AST::AddExp &node)
{
    BaseUnit l_unit = node.lhs->accept(*this);
    BaseUnit r_unit = node.rhs->accept(*this);
    LIR::Exp *l_exp = l_unit.decode2Exp();
    LIR::Exp *r_exp = r_unit.decode2Exp();
    Type *lt = l_unit.unit_res_type;

    Type *rt = r_unit.unit_res_type;

    LIR::BIN_OP op;
    switch (node.op)
    {
    case AST::add_t::ADD:
    {
        op = LIR::BIN_OP::T_plus;
        break;
    }
    case AST::add_t::SUB:
    {
        op = LIR::BIN_OP::T_minus;
        break;
    }
    default:
        assert(0);
    }
    Type *t = BinOpResType(lt, rt, op);
    LIR::Exp *exp = BinOpResExp(op, lt, l_exp, rt, r_exp);
    return BaseUnit(t, exp);
}

BaseUnit ASTVisitor::visitRelExpNode(AST::RelExp &node)
{
    BaseUnit l_unit = node.lhs->accept(*this);
    BaseUnit r_unit = node.rhs->accept(*this);
    LIR::Exp *l_exp = l_unit.decode2Exp();
    LIR::Exp *r_exp = r_unit.decode2Exp();

    Type *lt = l_unit.unit_res_type;

    Type *rt = r_unit.unit_res_type;

    LIR::RelOp op;
    if (lt->t_kind == T_KIND::FLOAT || rt->t_kind == T_KIND::FLOAT)
    {
        if (lt->t_kind == T_KIND::INT)
            l_exp = iExp2fExp(l_exp);
        if (rt->t_kind == T_KIND::INT)
            r_exp = iExp2fExp(r_exp);
        switch (node.op)
        {
        case AST::rel_t::GE:
            op = LIR::RelOp::F_ge;
            break;
        case AST::rel_t::LE:
            op = LIR::RelOp::F_le;
            break;
        case AST::rel_t::GT:
            op = LIR::RelOp::F_gt;
            break;
        case AST::rel_t::LT:
            op = LIR::RelOp::F_lt;
            break;
        }
    }
    else
    {
        switch (node.op)
        {
        case AST::rel_t::GE:
            op = LIR::RelOp::T_ge;
            break;
        case AST::rel_t::LE:
            op = LIR::RelOp::T_le;
            break;
        case AST::rel_t::GT:
            op = LIR::RelOp::T_gt;
            break;
        case AST::rel_t::LT:
            op = LIR::RelOp::T_lt;
            break;
        }
    }
    LIR::CJumpStm *stm = new CJumpStm(op, l_exp, r_exp, "", "");
    LIR::PatchList *trues = new LIR::PatchList(&stm->trueLabel, NULL);
    LIR::PatchList *falses = new LIR::PatchList(&stm->falseLabel, NULL);
    Condition cond = Condition(trues,
                               falses,
                               stm);
    Type *t = newIntType(new int(0), false);
    return BaseUnit(t, cond);
}

BaseUnit ASTVisitor::visitEqExpNode(AST::EqualExp &node)
{
    BaseUnit l_unit = node.lhs->accept(*this);
    BaseUnit r_unit = node.rhs->accept(*this);
    LIR::Exp *l_exp = l_unit.decode2Exp();
    LIR::Exp *r_exp = r_unit.decode2Exp();

    Type *lt = l_unit.unit_res_type;

    Type *rt = r_unit.unit_res_type;

    LIR::RelOp op;
    if (lt->t_kind == T_KIND::FLOAT || rt->t_kind == T_KIND::FLOAT)
    {
        if (lt->t_kind == T_KIND::INT)
            l_exp = iExp2fExp(l_exp);
        if (rt->t_kind == T_KIND::INT)
            r_exp = iExp2fExp(r_exp);
        switch (node.op)
        {
        case AST::equal_t::EQ:
            op = LIR::RelOp::F_eq;
            break;
        case AST::equal_t::NE:
            op = LIR::RelOp::F_ne;
            break;
        }
    }
    else
    {
        switch (node.op)
        {
        case AST::equal_t::EQ:
            op = LIR::RelOp::T_eq;
            break;
        case AST::equal_t::NE:
            op = LIR::RelOp::T_ne;
            break;
        }
    }
    LIR::CJumpStm *stm = new CJumpStm(op, l_exp, r_exp, "", "");
    LIR::PatchList *trues = new LIR::PatchList(&stm->trueLabel, NULL);
    LIR::PatchList *falses = new LIR::PatchList(&stm->falseLabel, NULL);
    Condition cond = Condition(trues,
                               falses,
                               stm);
    Type *t = newIntType(new int(0), false);
    return BaseUnit(t, cond);
}

BaseUnit ASTVisitor::visitLAndNode(AST::LAndExp &node)
{
    BaseUnit l_unit = node.lhs->accept(*this);
    BaseUnit r_unit = node.rhs->accept(*this);
    LIR::Condition l_cond = l_unit.decode2Condtion();
    LIR::Condition r_cond = r_unit.decode2Condtion();
    std::string l_t_label = newLabel();
    doPatch(l_cond.trues, l_t_label);

    LIR::StmList *stm_l = new StmList();
    stm_l->add2StmList(l_cond.condition_stm);
    stm_l->add2StmList(new LabelStm(l_t_label));
    stm_l->add2StmList(r_cond.condition_stm);

    Condition cond = Condition(r_cond.trues, joinPatch(l_cond.falses, r_cond.falses), stm_l);

    return BaseUnit(newIntType(new int(0), false), cond);
}

BaseUnit ASTVisitor::visitLOrExpNode(AST::LOrExp &node)
{
    BaseUnit l_unit = node.lhs->accept(*this);
    BaseUnit r_unit = node.rhs->accept(*this);
    LIR::Condition l_cond = l_unit.decode2Condtion();
    LIR::Condition r_cond = r_unit.decode2Condtion();
    std::string l_f_label = newLabel();
    doPatch(l_cond.falses, l_f_label);

    LIR::StmList *stm_l = new StmList();
    stm_l->add2StmList(l_cond.condition_stm);
    stm_l->add2StmList(new LabelStm(l_f_label));
    stm_l->add2StmList(r_cond.condition_stm);
    Condition cond = Condition(joinPatch(l_cond.trues, r_cond.trues), r_cond.falses, stm_l);

    return BaseUnit(newIntType(new int(0), false), cond);
}

BaseUnit ASTVisitor::visitIDExpNode(AST::IDExp &node)
{
    assert(0);
}

BaseUnit ASTVisitor::calArrayInitValsFromExp(AST::Exp &node)
{
    BaseUnit unit = node.accept(*this);
    LIR::Exp *exp = unit.decode2Exp();
    Type *ret_type = unit.unit_res_type;
    if (ret_type->t_kind == T_KIND::INT && arr_head->t_kind == T_KIND::FLOAT)
    {
        exp = iExp2fExp(exp);
        ret_type = newFloatType(new int(digit_i2f(*ret_type->var_val)), false);
    }
    else if (ret_type->t_kind == T_KIND::FLOAT && arr_head->t_kind == T_KIND::INT)
    {
        exp = fExp2iExp(exp);
        ret_type = newIntType(new int(digit_f2i(*ret_type->var_val)), false);
    }
    MemExp *mem_addr = new MemExp(new BinOpExp(LIR::BIN_OP::T_plus, arr_base, new ConstExp(4 * arr_off)));
    LIR::Stm *stm = new MoveStm(mem_addr, exp);
    return BaseUnit(ret_type, new StmWithRetExp(new ConstExp(0), stm));
}

/* eg: {1, 2, 3, exp1, ...} */
BaseUnit ASTVisitor::calArrayInitValsFromArrayInitExp(AST::ArrayInitVals &node)
{
    StmList *stm_list = new StmList();
    Type *arr_head_org = arr_head;
    // if (arr_head->t_kind == T_KIND::ARRAY)
    // {
    arr_head->var_val = new int[arr_head->array_size];
    memset(arr_head->var_val, 0, arr_head->array_size * sizeof(int));
    int self_arr_off = 0;
    arr_head = arr_head->element_type;
    for (const auto &it : *(node.val_list))
    {
        BaseUnit unit = it->calArray(*this);
        stm_list->add2StmList(new ExpStm(unit.decode2Exp()));
        if (unit.unit_res_type->element_type == NULL)
        { // int
            arr_head_org->var_val[self_arr_off] = *unit.unit_res_type->var_val;
        }
        else
        { // arr to arr
            std::memcpy(&arr_head_org->var_val[self_arr_off],
                        unit.unit_res_type->var_val,
                        unit.unit_res_type->array_size * 4);
            delete unit.unit_res_type->var_val;
        }
        arr_off += unit.unit_res_type->array_size;
        self_arr_off += unit.unit_res_type->array_size;
    }
    arr_off -= self_arr_off;
    arr_head = arr_head_org;
    return BaseUnit(arr_head, new StmWithRetExp(arr_base, stm_list));
}

BaseUnit ASTVisitor::visitGetIntExpNode(AST::GetIntExp &node)
{
    return BaseUnit(newIntType(new int(0), false), new LIR::CallExp("getint", std::vector<LIR::Exp *>()));
}

BaseUnit ASTVisitor::visitGetChExpNode(AST::GetChExp &node)
{
    return BaseUnit(newIntType(new int(0), false), new LIR::CallExp("getch", std::vector<LIR::Exp *>()));
}

BaseUnit ASTVisitor::visitGetFloatExpNode(AST::GetFloatExp &node)
{
    return BaseUnit(newIntType(new int(0), false), new LIR::CallExp("getfloat", std::vector<LIR::Exp *>()));
}

BaseUnit ASTVisitor::visitGetArrayExpNode(AST::GetArrayExp &node)
{
    return BaseUnit(newIntType(new int(0), false), new LIR::CallExp("getarray", std::vector<LIR::Exp *>(1, node.arr->accept(*this).decode2Exp())));
}

BaseUnit ASTVisitor::visitGetFArrayExpNode(AST::GetFArrayExp &node)
{
    return BaseUnit(newIntType(new int(0), false), new LIR::CallExp("getfarray", std::vector<LIR::Exp *>(1, node.arr->accept(*this).decode2Exp())));
}

//

Stm *ASTVisitor::visitPutIntStmtNode(AST::PutIntStmt &node)
{
    BaseUnit unit = node.exp->accept(*this);
    LIR::Exp *exp = unit.decode2Exp();
    T_KIND kd = unit.unit_res_type->t_kind;
    exp = align2LetfExp(exp, T_KIND::INT, kd);
    return new LIR::ExpStm(new LIR::CallExp("putint", std::vector<LIR::Exp *>(1, exp)));
}

Stm *ASTVisitor::visitPutChStmtNode(AST::PutChStmt &node)
{
    BaseUnit unit = node.exp->accept(*this);
    LIR::Exp *exp = unit.decode2Exp();
    T_KIND kd = unit.unit_res_type->t_kind;
    exp = align2LetfExp(exp, T_KIND::INT, kd);
    return new LIR::ExpStm(new LIR::CallExp("putch", std::vector<LIR::Exp *>(1, exp)));
}

Stm *ASTVisitor::visitPutFloatStmtNode(AST::PutFloatStmt &node)
{
    BaseUnit unit = node.exp->accept(*this);
    LIR::Exp *exp = unit.decode2Exp();
    T_KIND kd = unit.unit_res_type->t_kind;
    exp = align2LetfExp(exp, T_KIND::FLOAT, kd);
    return new LIR::ExpStm(new LIR::CallExp("putfloat", std::vector<LIR::Exp *>(1, exp)));
}

Stm *ASTVisitor::visitPutArrayStmtNode(AST::PutArrayStmt &node)
{
    BaseUnit unit = node.len->accept(*this);
    LIR::Exp *exp = unit.decode2Exp();
    T_KIND kd = unit.unit_res_type->t_kind;
    exp = align2LetfExp(exp, T_KIND::FLOAT, kd);
    std::vector<LIR::Exp *> args = std::vector<LIR::Exp *>(1, exp);
    args.push_back(node.arr->accept(*this).decode2Exp());
    return new LIR::ExpStm(new LIR::CallExp("putarray", args));
}

Stm *ASTVisitor::visitPutFArrayStmtNode(AST::PutFArrayStmt &node)
{
    BaseUnit unit = node.len->accept(*this);
    LIR::Exp *exp = unit.decode2Exp();
    T_KIND kd = unit.unit_res_type->t_kind;
    exp = align2LetfExp(exp, T_KIND::FLOAT, kd);
    std::vector<LIR::Exp *> args = std::vector<LIR::Exp *>(1, exp);
    args.push_back(node.arr->accept(*this).decode2Exp());
    return new LIR::ExpStm(new LIR::CallExp("putfarray", args));
}

Stm *ASTVisitor::visitPutFStmtNode(AST::PutFStmt &node)
{
    // 😅😅😅
    assert(0);
    return nullptr;
}

Stm *ASTVisitor::visitStarttimStmt(AST::StarttimeStmt &node)
{
    return new LIR::ExpStm(new LIR::CallExp("_sysy_starttime", std::vector<LIR::Exp *>(1, new LIR::ConstExp(node.lineno))));
}

Stm *ASTVisitor::visitStoptimStmt(AST::StoptimeStmt &node)
{
    return new LIR::ExpStm(new LIR::CallExp("_sysy_stoptime", std::vector<LIR::Exp *>(1, new LIR::ConstExp(node.lineno))));
}

LIR::Stm *BaseUnit::decode2Stm()
{
    switch (this->unit_kind)
    {
    case UnitKind::STM_U:
        return this->stm;
        break;
    case UnitKind::EXP_U:
        return new ExpStm(this->exp);
        break;
    case UnitKind::CONDITION_U:
        return new ExpStm(this->decode2Exp());
        break;
    }
}

LIR::Exp *BaseUnit::decode2Exp()
{
    switch (this->unit_kind)
    {
    case UnitKind::EXP_U:
        return this->exp;
        break;
    case UnitKind::CONDITION_U:
    {
        int temp_id = newLocalTempVarID();
        std::string t_label = newLabel(), f_label = newLabel();
        doPatch(this->condtion_s.trues, t_label);
        doPatch(this->condtion_s.falses, f_label);

        auto tr = new LIR::StmWithRetExp(new LIR::TempVarExp(temp_id),
                                         new LIR::LabelStm(t_label));
        auto f_ass = new LIR::StmWithRetExp(tr, new LIR::MoveStm(new LIR::TempVarExp(temp_id),
                                                                 new LIR::ConstExp(0)));
        auto fr = new LIR::StmWithRetExp(f_ass, new LIR::LabelStm(f_label));
        auto stm = new LIR::StmWithRetExp(fr, this->condtion_s.condition_stm);
        auto t_ass = new LIR::StmWithRetExp(stm, new LIR::MoveStm(new LIR::TempVarExp(temp_id), new LIR::ConstExp(1)));

        return t_ass;
    }
    break;
    default:
        assert(0);
    }
}

Condition BaseUnit::decode2Condtion()
{
    switch (this->unit_kind)
    {
    case UnitKind::EXP_U:
    {
        CJumpStm *stm = new CJumpStm(RelOp::T_ne, this->exp, new ConstExp(0), "", "");
        Condition cond;
        cond.condition_stm = stm;
        cond.trues = new PatchList(&static_cast<CJumpStm *>(stm)->trueLabel, NULL);
        cond.falses = new PatchList(&static_cast<CJumpStm *>(stm)->falseLabel, NULL);
        return cond;
    }
    case UnitKind::CONDITION_U:
        return this->condtion_s;
    default:
        assert(0);
    }
}

void LIR::StmList::add2StmList(LIR::Stm *s)
{
    if (!is_nop(s))
    {
        if (s->stm_kind == LIR::STM_KIND::stm_list)
        {
            for (LIR::Stm *ss : static_cast<LIR::StmList *>(s)->stm_list)
                this->add2StmList(ss);
        }
        else
        {
            this->stm_list.push_back(s);
        }
        // this->stm_list.push_back(s);
    }
}

// Stm
// call after linearize(), each of 's' should not be LIR::StmList
void LIR::StmList::printIR()
{
    for (Stm *s : this->stm_list)
    {
        s->printIR();
        // std::cerr << std::endl;
    }
}

void LIR::LabelStm::printIR()
{
    std::cerr << "LABELSTM:     " << label_str << std::endl;
}

void LIR::JumpStm::printIR()
{
    std::cerr << "JUMPSTM:       " << jump_label_str << std::endl;
}

void LIR::CJumpStm::printIR()
{
    std::cerr << "CJUMPSTM:     ";
    l->printIR();
    std::cerr << " " + relop2string(this->op) + " ";
    r->printIR();
    std::cerr << std::endl
              << "TRUE:     " << trueLabel
              << std::endl
              << "FALSE:       " << falseLabel << std::endl;
}

void LIR::MoveStm::printIR()
{
    std::cerr << "MOVESTM:      ";
    dst->printIR();
    std::cerr << "      ";
    src->printIR();
    std::cerr << std::endl;
}

void LIR::ExpStm::printIR()
{
    std::cerr << "EXPSTM:        ";
    exp->printIR();
    std::cerr << std::endl;
}

// Exp

// void LIR::ExpList::printIR()
// {
//     for (LIR::Exp e : exp_list)
//     {
//         e.printIR();
//         std::cerr << std::endl;
//     }
// }

void LIR::BinOpExp::printIR()
{
    std::cerr << "( ";
    l->printIR();
    std::cerr << " " + binop2string(this->binop) + " ";
    r->printIR();
    std::cerr << " )";
}

void LIR::TempVarExp::printIR()
{
    std::cerr << "tmp" << temp_label_id_int;
}

void LIR::MemExp::printIR()
{
    std::cerr << "Mem( ";
    this->mem_addr->printIR();
    std::cerr << " )";
}

void LIR::NameExp::printIR()
{
    std::cerr << name;
}

void LIR::CallExp::printIR()
{
    std::cerr << "( call " + func_name_str;
    std::cerr << '(';
    for (auto it : args)
    {
        it->printIR();
        std::cerr << ", ";
    }
    std::cerr << ") )";
}

void LIR::ConstExp::printIR()
{
    // if (is_int_constexp)
    std::cerr << this->val;
    // else
    //     std::cerr << value.f_val;
}

// shou not be called after linearize
void LIR::StmWithRetExp::printIR()
{
    assert(0);
}

/**
 * copy inline
 */

void LIR::StmList::copy_inline(std::unordered_set<std::string> &global_var,
                               std::unordered_map<int, int> &tmp_var_mp,
                               std::unordered_map<std::string, std::string> &label_mp,
                               std::vector<LIR::Stm *> &sl)
{
    assert(0);
    // for (auto stm : this->stm_list)
    // {
    //     stm->copy_inline(global_var, tmp_var_mp, label_mp, sl);
    // }
}

void LIR::LabelStm::copy_inline(std::unordered_set<std::string> &global_var,
                                std::unordered_map<int, int> &tmp_var_mp,
                                std::unordered_map<std::string, std::string> &label_mp,
                                std::vector<LIR::Stm *> &sl)
{
    if (global_var.count(this->label_str))
    {
        sl.push_back(new LIR::LabelStm(this->label_str));
        return;
    }
    if (label_mp.count(this->label_str))
    {
        sl.push_back(new LIR::LabelStm(label_mp[this->label_str]));
        return;
    }
    std::string nl = newLabel();
    label_mp[this->label_str] = nl;
    sl.push_back(new LIR::LabelStm(nl));
    return;
}

void LIR::JumpStm::copy_inline(std::unordered_set<std::string> &global_var,
                               std::unordered_map<int, int> &tmp_var_mp,
                               std::unordered_map<std::string, std::string> &label_mp,
                               std::vector<LIR::Stm *> &sl)
{
    if (global_var.count(this->jump_label_str))
    {
        sl.push_back(new LIR::JumpStm(this->jump_label_str));
        return;
    }
    if (label_mp.count(this->jump_label_str))
    {
        sl.push_back(new LIR::JumpStm(label_mp[this->jump_label_str]));
        return;
    }
    std::string nl = newLabel();
    label_mp[this->jump_label_str] = nl;
    sl.push_back(new LIR::JumpStm(nl));
    return;
}

void LIR::CJumpStm::copy_inline(std::unordered_set<std::string> &global_var,
                                std::unordered_map<int, int> &tmp_var_mp,
                                std::unordered_map<std::string, std::string> &label_mp,
                                std::vector<LIR::Stm *> &sl)
{
    std::string t_label, f_label;
    if (global_var.count(this->trueLabel))
    {
        t_label = this->trueLabel;
    }
    else if (label_mp.count(this->trueLabel))
    {
        t_label = label_mp.at(this->trueLabel);
    }
    else
    {
        t_label = newLabel();
        label_mp[this->trueLabel] = t_label;
    }

    if (global_var.count(this->falseLabel))
    {
        f_label = this->falseLabel;
    }
    else if (label_mp.count(this->falseLabel))
    {
        f_label = label_mp.at(this->falseLabel);
    }
    else
    {
        f_label = newLabel();
        label_mp[this->falseLabel] = f_label;
    }
    sl.push_back(new LIR::CJumpStm(this->op,
                                   this->l->copy_inline(global_var, tmp_var_mp, label_mp, sl),
                                   this->r->copy_inline(global_var, tmp_var_mp, label_mp, sl),
                                   t_label, f_label));
    return;
}

void LIR::MoveStm::copy_inline(std::unordered_set<std::string> &global_var,
                               std::unordered_map<int, int> &tmp_var_mp,
                               std::unordered_map<std::string, std::string> &label_mp,
                               std::vector<LIR::Stm *> &sl)
{
    sl.push_back(new LIR::MoveStm(this->dst->copy_inline(global_var, tmp_var_mp, label_mp, sl),
                                  this->src->copy_inline(global_var, tmp_var_mp, label_mp, sl)));
    return;
}

void LIR::ExpStm::copy_inline(std::unordered_set<std::string> &global_var,
                              std::unordered_map<int, int> &tmp_var_mp,
                              std::unordered_map<std::string, std::string> &label_mp,
                              std::vector<LIR::Stm *> &sl)
{
    sl.push_back(new LIR::ExpStm(this->exp->copy_inline(global_var, tmp_var_mp, label_mp, sl)));
}

/**
 * exp copy_inline
 */

LIR::Exp *LIR::BinOpExp::copy_inline(std::unordered_set<std::string> &global_var,
                                     std::unordered_map<int, int> &tmp_var_mp,
                                     std::unordered_map<std::string, std::string> &label_mp,
                                     std::vector<LIR::Stm *> &sl)
{
    return new LIR::BinOpExp(this->binop,
                             this->l->copy_inline(global_var, tmp_var_mp, label_mp, sl),
                             this->r->copy_inline(global_var, tmp_var_mp, label_mp, sl));
}

LIR::Exp *LIR::TempVarExp::copy_inline(std::unordered_set<std::string> &global_var,
                                       std::unordered_map<int, int> &tmp_var_mp,
                                       std::unordered_map<std::string, std::string> &label_mp,
                                       std::vector<LIR::Stm *> &sl)
{
    if (tmp_var_mp.count(this->temp_label_id_int))
        return new LIR::TempVarExp(tmp_var_mp.at(this->temp_label_id_int));
    if (this->temp_label_id_int == 11 || this->temp_label_id_int == 13)
    { // fp/sp
        if (this->temp_label_id_int == 11)
            return new LIR::TempVarExp(tmp_var_mp[11]);
        else
            return new LIR::TempVarExp(13);
    }
    int n_tmp_id = newLocalTempVarID();
    tmp_var_mp[this->temp_label_id_int] = n_tmp_id;
    return new LIR::TempVarExp(n_tmp_id);
}

LIR::Exp *LIR::MemExp::copy_inline(std::unordered_set<std::string> &global_var,
                                   std::unordered_map<int, int> &tmp_var_mp,
                                   std::unordered_map<std::string, std::string> &label_mp,
                                   std::vector<LIR::Stm *> &sl)
{
    return new LIR::MemExp(this->mem_addr->copy_inline(global_var, tmp_var_mp, label_mp, sl));
}

LIR::Exp *LIR::NameExp::copy_inline(std::unordered_set<std::string> &global_var,
                                    std::unordered_map<int, int> &tmp_var_mp,
                                    std::unordered_map<std::string, std::string> &label_mp,
                                    std::vector<LIR::Stm *> &sl)
{
    if (global_var.count(this->name))
        return new LIR::NameExp(this->name);
    else if (label_mp.count(this->name))
        return new LIR::NameExp(label_mp.at(this->name));
    auto n_name = newLabel();
    label_mp[this->name] = n_name;
    return new LIR::NameExp(n_name);
}

LIR::Exp *LIR::CallExp::copy_inline(std::unordered_set<std::string> &global_var,
                                    std::unordered_map<int, int> &tmp_var_mp,
                                    std::unordered_map<std::string, std::string> &label_mp,
                                    std::vector<LIR::Stm *> &sl)
{
    std::vector<LIR::Exp *> n_para_list;
    for (auto it : this->args)
        n_para_list.push_back(it->copy_inline(global_var, tmp_var_mp, label_mp, sl));
    return new LIR::CallExp(this->func_name_str, n_para_list);
}

LIR::Exp *LIR::ConstExp::copy_inline(std::unordered_set<std::string> &global_var,
                                     std::unordered_map<int, int> &tmp_var_mp,
                                     std::unordered_map<std::string, std::string> &label_mp,
                                     std::vector<LIR::Stm *> &sl)
{
    // if (this->is_int_constexp)
    return new LIR::ConstExp(this->val);
    // else
    //     return new LIR::ConstExp(this->value.f_val);
}

LIR::Exp *LIR::StmWithRetExp::copy_inline(std::unordered_set<std::string> &global_var,
                                          std::unordered_map<int, int> &tmp_var_mp,
                                          std::unordered_map<std::string, std::string> &label_mp,
                                          std::vector<LIR::Stm *> &sl)
{
    assert(0);
    return nullptr;
}

RelOp LIR::notRel(RelOp op)
{
    switch (op)
    {
    case RelOp::T_eq:
        return RelOp::T_ne;
    case RelOp::T_ne:
        return RelOp::T_eq;
    case RelOp::T_lt:
        return RelOp::T_ge;
    case RelOp::T_ge:
        return RelOp::T_lt;
    case RelOp::T_gt:
        return RelOp::T_le;
    case RelOp::T_le:
        return RelOp::T_gt;
    case RelOp::F_eq:
        return RelOp::F_ne;
    case RelOp::F_ne:
        return RelOp::F_eq;
    case RelOp::F_lt:
        return RelOp::F_ge;
    case RelOp::F_ge:
        return RelOp::F_lt;
    case RelOp::F_gt:
        return RelOp::F_le;
    case RelOp::F_le:
        return RelOp::F_gt;
    }
}
