#include "dict.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/***    CREATE DICT   ****/
Dict *createDict_var(char *key, variable variable)
{
    Dict *result = (Dict *)malloc(sizeof(Dict));
    char *newKey = (char *)malloc(sizeof(char) * (strlen(key) + 1));
    strcpy(newKey, key);
    result->key = newKey;
    result->value = variable;
    result->next = NULL;
    return result;
}

Dict *createDict_int(char *key, int value)
{
    variable variable = get_var_int(value);
    return createDict_var(key, variable);
}
Dict *createDict_double(char *key, double value)
{
    variable variable = get_var_double(value);
    return createDict_var(key, variable);
}
Dict *createDict_string(char *key, char *value)
{
    variable variable = get_var_string(value);
    return createDict_var(key, variable);
}
Dict *createDict_bool(char *key, int value)
{
    variable variable = get_var_bool(value);
    return createDict_var(key, variable);
}
Dict *createDict_null(char *key)
{
    variable variable = get_var_null();
    return createDict_var(key, variable);
}
Dict *createDict_list(char *key, List *list)
{
    variable variable = get_var_list(list);
    return createDict_var(key, variable);
}

Dict *createDict_int_arr(char *key, int val[], int length)
{
    variable variable = get_var_int_arr(val, length);
    return createDict_var(key, variable);
}
Dict *createDict_double_arr(char *key, double val[], int length)
{
    variable variable = get_var_double_arr(val, length);
    return createDict_var(key, variable);
}

Dict *createDict_bool_arr(char *key, int val[], int length)
{
    variable variable = get_var_bool_arr(val, length);
    return createDict_var(key, variable);
}

Dict *createDict_string_arr(char *key, char *val[], int length)
{
    variable variable = get_var_string_arr(val, length);
    return createDict_var(key, variable);
}

Dict *createDict_null_arr(char *key, int length)
{
    variable variable = get_var_null_arr(length);
    return createDict_var(key, variable);
}

Dict *createDict_dict(char *key, Dict *dict)
{
    variable variable = get_var_dict(dict);
    return createDict_var(key, variable);
}

Dict *createDict(DataType type, char *key, void *value)
{
    variable variable = get_var(type, value);
    return createDict_var(key, variable);
}

/* CREATE DICT END */
/***** ADD DICT *****/

Dict *addDict_var(Dict *d, char *key, variable variable)
{
    Dict *result = (Dict *)malloc(sizeof(Dict));
    char *newKey = (char *)malloc(sizeof(char) * (strlen(key) + 1));
    strcpy(newKey, key);
    result->key = newKey;
    result->value = variable;
    result->next = NULL;
    Dict *temp = d;
    while (temp->next != NULL)
    {
        temp = temp->next;
    }
    temp->next = result;
    return result;
}

Dict *addDict_int(Dict *d, char *key, int value)
{
    variable variable = get_var_int(value);
    return addDict_var(d, key, variable);
}
Dict *addDict_double(Dict *d, char *key, double value)
{
    variable variable = get_var_double(value);
    return addDict_var(d, key, variable);
}
Dict *addDict_string(Dict *d, char *key, char *value)
{
    variable variable = get_var_string(value);
    return addDict_var(d, key, variable);
}
Dict *addDict_bool(Dict *d, char *key, int value)
{
    variable variable = get_var_bool(value);
    return addDict_var(d, key, variable);
}
Dict *addDict_null(Dict *d, char *key)
{
    variable variable = get_var_null();
    return addDict_var(d, key, variable);
}

Dict *addDict_dict(Dict *d, char *key, Dict *dict)
{
    variable variable = get_var_dict(dict);
    return addDict_var(d, key, variable);
}

Dict *addDict_list(Dict *d, char *key, List *list)
{
    variable variable = get_var_list(list);
    return addDict_var(d, key, variable);
}

Dict *addDict(Dict *d, DataType type, char *key, void *value)
{
    variable variable = get_var(type, value);
    return addDict_var(d, key, variable);
}

Dict *addDict_int_arr(Dict *d, char *key, int val[], int length)
{
    variable variable = get_var_int_arr(val, length);
    return addDict_var(d, key, variable);
}

Dict *addDict_double_arr(Dict *d, char *key, double val[], int length)
{
    variable variable = get_var_double_arr(val, length);
    return addDict_var(d, key, variable);
}

Dict *addDict_bool_arr(Dict *d, char *key, int val[], int length)
{
    variable variable = get_var_bool_arr(val, length);
    return addDict_var(d, key, variable);
}

Dict *addDict_string_arr(Dict *d, char *key, char *val[], int length)
{
    variable variable = get_var_string_arr(val, length);
    return addDict_var(d, key, variable);
}

Dict *addDict_null_arr(Dict *d, char *key, int length)
{
    variable variable = get_var_null_arr(length);
    return addDict_var(d, key, variable);
}

/* ADD DICT END */

/***** DELETE DICT *****/

void free_dict(Dict *d);
void free_list(List *l);

void free_variable(variable var)
{
    switch (var.type)
    {
    case TYPE_STRING:
        if (var.value.string_v)
            free(var.value.string_v);
        break;

    case TYPE_DICT:
        if (var.value.dict_v)
            free_dict(var.value.dict_v);
        break;

    case TYPE_LIST:
        if (var.value.list_v)
            free_list(var.value.list_v);
        break;

    case TYPE_INT_ARR:
        if (var.value.int_arr_v)
            free(var.value.int_arr_v);
        break;
    case TYPE_DOUBLE_ARR:
        if (var.value.double_arr_v)
            free(var.value.double_arr_v);
        break;
    case TYPE_BOOL_ARR:
        if (var.value.bool_arr_v)
            free(var.value.bool_arr_v);
        break;
    case TYPE_STRING_ARR:
        if (var.value.string_arr_v)
        {
            for (int i = 0; i < var.size; i++)
            {
                if (var.value.string_arr_v[i])
                    free(var.value.string_arr_v[i]);
            }
            free(var.value.string_arr_v);
        }
        break;

    default:
        break;
    }
}

void free_list(List *l)
{
    List *current = l;
    while (current != NULL)
    {
        List *next = current->next;

        free_variable(current->item);

        free(current);

        current = next;
    }
}

void free_dict(Dict *d)
{
    Dict *current = d;
    while (current != NULL)
    {
        Dict *next = current->next;

        if (current->key)
            free(current->key);

        free_variable(current->value);

        free(current);

        current = next;
    }
}

int deleteDictKey(Dict **head, char *key)
{
    if (*head == NULL)
        return 0;

    Dict *current = *head;
    Dict *prev = NULL;
    while (current != NULL)
    {
        if (strcmp(current->key, key) == 0 && strlen(current->key) == strlen(key))
        {
            if (prev == NULL)
                *head = current->next;

            else
                prev->next = current->next;

            free(current->key);

            switch (current->value.type)
            {
            case TYPE_STRING:
                free(current->value.value.string_v);
                break;
            case TYPE_BOOL_ARR:
                free(current->value.value.bool_arr_v);
                break;
            case TYPE_DOUBLE_ARR:
                free(current->value.value.double_arr_v);
                break;
            case TYPE_INT_ARR:
                free(current->value.value.int_arr_v);
                break;

            case TYPE_STRING_ARR:
                for (int i = current->value.size - 1; i >= 0; i--)
                {
                    free(current->value.value.string_arr_v[i]);
                }
                free(current->value.value.string_arr_v);
                break;

            default:
                break;
            }

            free(current);
            return 1;
        }
        prev = current;
        current = current->next;
    }

    return 0;
}
/* DELETE DICT END */

/***** GET DICT VARIABLE *****/

Dict *getDictVariable(Dict *d, char *key)
{
    Dict *temp = d;
    while ((strlen(key) != strlen(temp->key) || strcmp(key, temp->key) != 0))
    {
        temp = temp->next;
        if (temp == NULL)
            break;
    }
    return temp;
}
variable getDictVariable_var(Dict *d, char *key)
{
    Dict *dict = getDictVariable(d, key);
    if (dict == NULL)
        return get_var_unknown();
    else
        return dict->value;
}

/* GET DICT VARIABLE END */

/***** SET DICT *****/

void setDictKey(Dict *d, char *key)
{
    char *newKey = (char *)malloc(sizeof(char) * (strlen(key) + 1));
    strcpy(newKey, key);
    d->key = newKey;
}

/* SET DICT END */
