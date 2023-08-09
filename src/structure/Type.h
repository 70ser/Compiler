#ifndef __TYPE_H__
#define __TYPE_H__

#include <vector>
#include <assert.h>

enum class T_KIND
{
    INT,
    FLOAT,
    FUNCTION,
    VOID,
    ARRAY,
    // FLOAT_ARRAY
};

class Type
{
public:
    T_KIND t_kind;

    bool is_const;

    // if var
    // union
    // {
    //     int i_val;
    //     float f_val;
    // } var_val;
    int *var_val;

    // if array
    Type *element_type = NULL; // a[1][add(1,2)][exp]
    int array_size;
    int array_dim;
    // union
    // {
    //     int *i_arr_p;
    //     float *f_arr_p;
    // } arr_val;
    // int *arr_val;

    // if func
    Type *func_return_type = NULL;
    std::vector<Type *> func_param_type;

    Type() {}

    // init var type
    Type(T_KIND kind, int *v, bool _is_const) : t_kind(kind), is_const(_is_const), array_size(1) { this->var_val = v; }
    // Type(T_KIND kind, int *v, bool _is_const) : t_kind(kind), is_const(_is_const), array_size(1) { this->var_val = v; }

    // init arr type
    Type(T_KIND kind, Type *_element_type, int *_i_arr_p, int _dim, bool _is_const)
    {
        assert(_element_type);
        this->t_kind = kind;

        this->element_type = _element_type;
        this->array_size = _dim * _element_type->array_size;
        this->array_dim = _dim;

        this->var_val = _i_arr_p;

        this->is_const = _is_const;
    }

    // Type(T_KIND kind, Type *_element_type, float *_f_arr_p, int _dim, bool _is_const)
    // {
    //     assert(_element_type);
    //     this->t_kind = kind;

    //     this->element_type = _element_type;
    //     this->array_size = _dim * _element_type->array_size;
    //     this->array_dim = _dim;

    //     this->arr_val.f_arr_p = _f_arr_p;

    //     this->is_const = _is_const;
    // }

    // init func call
    Type(T_KIND kind, Type *_return_t, std::vector<Type *> _func_param_type)
    {
        this->t_kind = kind;

        this->func_return_type = _return_t;
        this->func_param_type = _func_param_type;
    }
};

static inline Type *newIntType(int *val, bool _is_const) { return new Type(T_KIND::INT, val, _is_const); }

static inline Type *newFloatType(int *val, bool _is_const) { return new Type(T_KIND::FLOAT, val, _is_const); }

static inline Type *newVoidType() { return new Type(T_KIND::VOID, (int*)NULL, false); }

static inline Type *newArrayType(Type *_element_type, int _dim, bool _is_const) { return new Type(T_KIND::ARRAY, _element_type, (int *)NULL, _dim, _is_const); }

// static inline Type *newFloatArrayType(Type *_element_type, int _dim, bool _is_const) { return new Type(T_KIND::FLOAT_ARRAY, _element_type, (int *)NULL, _dim, _is_const); }

static inline Type *newFuncType(Type *_return_t, std::vector<Type *> _func_param_type) { return new Type(T_KIND::FUNCTION, _return_t, _func_param_type); }

#endif