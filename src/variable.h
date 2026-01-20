#ifndef VARIABLE_H
#define VARIABLE_H

struct Dict;
struct List;

typedef enum DataType
{
    TYPE_INT,
    TYPE_DOUBLE,
    TYPE_STRING,
    TYPE_BOOL,
    TYPE_NULL,
    TYPE_INT_ARR,
    TYPE_DOUBLE_ARR,
    TYPE_STRING_ARR,
    TYPE_BOOL_ARR,
    TYPE_NULL_ARR,
    TYPE_DICT,
    TYPE_LIST,
    TYPE_UNKNOWN
} DataType;

typedef struct variable
{
    DataType type;
    int size;
    union
    {
        int int_v;
        double double_v;
        char *string_v;
        int bool_v;
        void *null_v;
        int *int_arr_v;
        double *double_arr_v;
        char **string_arr_v;
        int *bool_arr_v;
        struct Dict *dict_v;
        struct List *list_v;
    } value;
} variable;

typedef struct List
{
    variable item;
    struct List *next;
    struct List *tail;
} List;

variable get_var(DataType type, void *value);
variable get_var_int(int value);
variable get_var_double(double value);
variable get_var_string(char *value);
variable get_var_bool(int value);
variable get_var_null();
variable get_var_int_arr(int *value, int size);
variable get_var_double_arr(double *, int size);
variable get_var_bool_arr(int *value, int size);
variable get_var_string_arr(char **value, int size);
variable get_var_null_arr(int);
variable get_var_list(List *value);
variable get_var_dict(struct Dict *value);
List *create_list(variable value);
void add_var_to_list(List *list, variable value);

#endif
