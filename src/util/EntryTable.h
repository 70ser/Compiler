#ifndef __ENTRY_TABLE
#define __ENTRY_TABLE

#include <unordered_map>
#include <string>
#include <stack>
#include <assert.h>

#include "../structure/Type.h"

// Save context information by mapping func/var name to Entry
// may init as `FunctionEntryTable` or `VarEntryTable` to save 'function' info or 'gloabl/local var' info
// depends on 'Entry' type it pass to 'enterEntry(std::string label, Entry entry)' func

enum class EntryKind
{
    LOCAL,
    GLOBAL,
    FUNCTION
};

class Entry
{
public:
    EntryKind kind;
    Type *type;        // also save val
    std::string label; // for jump

    int temp_id_int;

    Entry() {}
    ~Entry() {}
};

class LocalVarEntry : public Entry
{
public:
    LocalVarEntry(Type *_type, int temp_id)
    {
        assert(_type->t_kind == T_KIND::INT || _type->t_kind == T_KIND::FLOAT || _type->t_kind == T_KIND::ARRAY);
        this->kind = EntryKind::LOCAL;
        this->type = _type;
        this->temp_id_int = temp_id;
    }

    LocalVarEntry(const Entry &entry)
    {
        this->kind = EntryKind::LOCAL;
        this->type = entry.type;
        this->temp_id_int = entry.temp_id_int;
    }
};

class GlobalvarEntry : public Entry
{
public:
    GlobalvarEntry(Type *_type, std::string _label)
    {
        assert(_type->t_kind == T_KIND::INT || _type->t_kind == T_KIND::FLOAT || _type->t_kind == T_KIND::ARRAY);
        this->kind = EntryKind::GLOBAL;
        this->type = _type;
        this->label = _label;
    }

    GlobalvarEntry(const Entry &entry)
    {
        this->kind = EntryKind::GLOBAL;
        this->type = entry.type;
        this->label = entry.label;
    }
};

class FuncEntry : public Entry
{
public:
    int stack_size;

    FuncEntry(Type *_type, std::string _label, int _stack_size = 64)
    {
        assert(_type->t_kind == T_KIND::FUNCTION);
        this->kind = EntryKind::FUNCTION;
        this->type = _type;
        this->label = _label;
        this->stack_size = _stack_size;
    }
};

const std::string FUNC_BEGIN = "_FUNC_BEGIN_";

// varName -> <varVal, is_global>
template <typename T>
class EntryTable
{
public:
    std::unordered_map<std::string, T> entryMap;

    // in-stack var <varname, <T, is_gloabl>>
    std::stack<std::pair<std::string, std::pair<T, bool>>> funcStack;

    bool enterEntry(std::string label, T entry)
    {
        if (this->entryMap.count(label))
            this->funcStack.emplace(std::make_pair(label, std::make_pair(entryMap[label], true)));
        else
            this->funcStack.emplace(std::make_pair(label, std::make_pair(entryMap[label], false)));
        this->entryMap[label] = entry;
        return true;
    }

    T findEntry(std::string k) { return this->entryMap[k]; }

    bool existEntry(std::string k) { return this->entryMap.count(k); }

    std::pair<std::string, T> popFromFuncStack()
    {
        assert(this->funcStack.size());
        std::pair<std::string, T> ans = *this->entryMap.find(this->funcStack.top().first);
        if (this->funcStack.top().second.second)
        { // global, change but not erase
            this->entryMap[this->funcStack.top().first] = this->funcStack.top().second.first;
        }
        else
        {
            this->entryMap.erase(this->funcStack.top().first);
        }
        this->funcStack.pop();
        return ans;
    }

    bool beginFuncScope()
    {
        // insert a label 'V' to 'entryMap' and 'funcStack'
        this->enterEntry(FUNC_BEGIN, {});
        return true;
    }

    bool endFuncScope()
    {
        // pop local var from 'entryMap' and 'funcStack' until 'V'
        while (this->popFromFuncStack().first != FUNC_BEGIN)
            ;
        return true;
    }

    EntryTable() { this->entryMap.reserve(1); }
    ~EntryTable() {}
};

static EntryTable<FuncEntry *> *withLibFunc()
{
    // add lib fun into global func EntryTable
    EntryTable<FuncEntry *> *g = new EntryTable<FuncEntry *>();
    Type *i_type = newIntType(new int(0), false);
    Type *f_type = newFloatType(new int(0), false);
    Type *v_type = newVoidType();
    g->enterEntry("getint",
                  new FuncEntry(newFuncType(i_type, std::vector<Type *>()), "getint"));
    g->enterEntry("getch",
                  new FuncEntry(newFuncType(i_type, std::vector<Type *>()), "getch"));
    g->enterEntry("getfloat",
                  new FuncEntry(newFuncType(f_type, std::vector<Type *>()), "getfloat"));
    g->enterEntry("getarray",
                  new FuncEntry(newFuncType(
                                    i_type,
                                    std::vector<Type *>(1, newArrayType(i_type, 1, false))),
                                "getarray"));
    g->enterEntry("getfarray",
                  new FuncEntry(newFuncType(
                                    i_type,
                                    std::vector<Type *>(1, newArrayType(f_type, 1, false))),
                                "getfarray"));

    g->enterEntry("putint", new FuncEntry(newFuncType(
                                              v_type, std::vector<Type *>(1, i_type)),
                                          "putint"));
    g->enterEntry("putch", new FuncEntry(newFuncType(
                                             v_type, std::vector<Type *>(1, i_type)),
                                         "putch"));
    g->enterEntry("putfloat", new FuncEntry(newFuncType(
                                                v_type, std::vector<Type *>(1, f_type)),
                                            "putfloat"));
    g->enterEntry("putarray", new FuncEntry(newFuncType(
                                                v_type, std::vector<Type *>{i_type,
                                                                            newArrayType(f_type, 1, false)}),
                                            "putarray"));
    g->enterEntry("putfarray", new FuncEntry(newFuncType(
                                                 v_type, std::vector<Type *>{i_type,
                                                                             newArrayType(f_type, 1, false)}),
                                             "putfarray"));
    return g;
}

#endif