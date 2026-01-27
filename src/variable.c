#include "variable.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

variable get_var(DataType type, void *val)
{
    switch (type)
    {
    case TYPE_INT:
        return get_var_int(*(int *)val);

    case TYPE_DOUBLE:
        return get_var_double(*(double *)val);

    case TYPE_STRING:
        return get_var_string((char *)val);

    case TYPE_BOOL:
        return get_var_bool(*(int *)val);

    default:
        return get_var_null();
    }
}

variable get_var_arr(DataType type, void *val, int length)
{
    switch (type)
    {
    case TYPE_INT_ARR:
        return get_var_int_arr((int *)val, length);
    case TYPE_DOUBLE_ARR:
        return get_var_double_arr((double *)val, length);
    case TYPE_BOOL_ARR:
        return get_var_bool_arr((int *)val, length);
    case TYPE_STRING_ARR:
        return get_var_string_arr((char **)val, length);
    default:
        return get_var_null();
    }
}

variable get_var_int(int val)
{
    variable result;
    result.type = TYPE_INT;
    result.value.int_v = val;
    return result;
}
variable get_var_double(double val)
{
    variable result;
    result.type = TYPE_DOUBLE;
    result.value.double_v = val;
    return result;
}
variable get_var_string(char *val)
{
    variable result;
    result.type = TYPE_STRING;
    int len = strlen(val);
    char *strVal = (char *)malloc(len + 1);
    strcpy(strVal, val);
    strVal[len] = '\0';
    result.value.string_v = strVal;
    return result;
}
variable get_var_bool(int val)
{
    variable result;
    result.type = TYPE_BOOL;
    result.value.bool_v = val;
    return result;
}
variable get_var_null()
{
    variable result;
    result.type = TYPE_NULL;
    result.value.null_v = NULL;
    return result;
}

variable get_var_int_arr(int val[], int length)
{
    variable result;
    result.type = TYPE_INT_ARR;
    result.size = length;
    int *arr = (int *)malloc(sizeof(int) * length);
    for (int i = 0; i < length; i++)
        arr[i] = val[i];
    result.value.int_arr_v = arr;
    return result;
}

variable get_var_bool_arr(int val[], int length)
{
    variable result;
    result.type = TYPE_BOOL_ARR;
    result.size = length;
    int *arr = (int *)malloc(sizeof(int) * length);
    for (int i = 0; i < length; i++)
        arr[i] = val[i];
    result.value.bool_arr_v = arr;
    return result;
}

variable get_var_double_arr(double val[], int length)
{
    variable result;
    result.type = TYPE_DOUBLE_ARR;
    result.size = length;
    double *arr = (double *)malloc(sizeof(double) * length);
    for (int i = 0; i < length; i++)
        arr[i] = val[i];
    result.value.double_arr_v = arr;
    return result;
}

variable get_var_string_arr(char *val[], int length)
{
    variable result;
    result.type = TYPE_STRING_ARR;
    result.size = length;
    char **arr = (char **)malloc(sizeof(char *) * length);
    for (int i = 0; i < length; i++)
    {
        arr[i] = (char *)malloc(sizeof(char) * (strlen(val[i]) + 1));
        strcpy(arr[i], val[i]);
    }
    result.value.string_arr_v = arr;
    return result;
}

variable get_var_null_arr(int length)
{
    variable result;
    result.type = TYPE_NULL_ARR;
    result.size = length;
    return result;
}
variable get_var_dict(struct Dict *dict)
{
    variable result;
    result.type = TYPE_DICT;
    result.value.dict_v = dict;
    return result;
}

variable get_var_list(List *list)
{
    variable result;
    result.type = TYPE_LIST;
    result.value.list_v = list;
    return result;
}

variable get_var_unknown()
{
    variable result;
    result.type = TYPE_UNKNOWN;
    return result;
}

List *create_list(variable var)
{
    List *list = (List *)malloc(sizeof(List));
    list->item = var;
    list->next = NULL;
    list->tail = list;
    return list;
}

void add_var_to_list(List *head, variable var)
{
    if (head == NULL)
        return;
    List *result = (List *)malloc(sizeof(List));
    result->item = var;
    result->next = NULL;
    result->tail = result;
    List *tail = head->tail;
    head->next = result;
    head->tail = result;
    if (head->next == NULL)
        return;

    tail->next = result;
    tail->tail = result;
}