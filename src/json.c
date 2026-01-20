#include "json.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

static void skip_whitespace(const char **cursor)
{
    while (*(*cursor) == ' ')
        (*cursor)++;
}

static char *parse_string_value(const char **cursor)
{
    if (**cursor != '"')
        return NULL;
    (*cursor)++;                 // skip the " character
    const char *start = *cursor; // save start point
    while (**cursor != '"' && **cursor != '\0')
        (*cursor)++; // Find end point
    if (**cursor == '\0')
        return NULL;           // if there is wrong string
    int len = *cursor - start; // find length of the string
    char *result = (char *)malloc(len + 1);
    strncpy(result, start, len); // copy the string
    result[len] = '\0';          // set the string's last elemnt
    (*cursor)++;                 // skip the " character
    return result;
}

static variable parse_value(const char **cursor);
static Dict *parse_object(const char **cursor);
static List *parse_array(const char **cursor);
static variable optimize_array(List *head);

static variable parse_value(const char **cursor)
{
    skip_whitespace(cursor);
    char c = **cursor;

    // String
    if (c == '"')
    {
        char *str = parse_string_value(cursor);
        variable v = get_var_string(str);
        free(str);
        return v;
    }
    // Object (Dict)
    if (c == '{')
        return get_var_dict(parse_object(cursor));
    // Array (List)
    if (c == '[')
    {
        List *raw_list = parse_array(cursor);
        return optimize_array(raw_list);
    }
    // Boolean (True)
    if (strncmp(*cursor, "true", 4) == 0)
    {
        *cursor += 4;
        return get_var_bool(1);
    }
    // Boolean (False)
    if (strncmp(*cursor, "false", 5) == 0)
    {
        *cursor += 5;
        return get_var_bool(0);
    }
    // Null
    if (strncmp(*cursor, "null", 4) == 0)
    {
        *cursor += 4;
        return get_var_null();
    }
    // Number
    if (c == '-' || isdigit(c))
    {
        if (c == '-' || isdigit(c))
        {
            const char *start = *cursor; // number start point
            char *end;
            double d = strtod(start, &end); // finding number end point

            *cursor = end;

            // find type
            int is_float = 0;
            for (const char *p = start; p < end; p++)
            {
                if (*p == '.' || *p == 'e' || *p == 'E')
                {
                    is_float = 1;
                    break;
                }
            }

            if (is_float)
            {
                return get_var_double(d);
            }
            else
            {
                return get_var_int((int)d);
            }
        }
    }

    return get_var_null(); // Error state
}

static void free_temp_list(List *head)
{
    List *temp;
    while (head != NULL)
    {
        temp = head;
        head = head->next;

        if (temp->item.type == TYPE_STRING)
        {
            free(temp->item.value.string_v);
        }
        free(temp);
    }
}

static variable optimize_array(List *head)
{
    if (head == NULL)
        return get_var_null_arr(0);

    int count = 0;
    int all_int = 1;
    int all_double = 1;
    int all_bool = 1;
    int all_string = 1;
    int all_null = 1;

    List *temp = head;
    while (temp != NULL && (all_int || all_double || all_bool || all_string || all_null))
    {
        count++;
        if (temp->item.type != TYPE_INT)
            all_int = 0;
        if (temp->item.type != TYPE_DOUBLE)
            all_double = 0;
        if (temp->item.type != TYPE_BOOL)
            all_bool = 0;
        if (temp->item.type != TYPE_STRING)
            all_string = 0;
        if (temp->item.type != TYPE_NULL)
            all_null = 0;
        temp = temp->next;
    }

    variable result;

    // INT ARRAY
    if (all_int && count > 0)
    {
        int *arr = (int *)malloc(sizeof(int) * count);
        temp = head;
        for (int i = 0; i < count; i++)
        {
            arr[i] = temp->item.value.int_v;
            temp = temp->next;
        }
        result = get_var_int_arr(arr, count);
        free(arr);
        free_temp_list(head);
        return result;
    }

    // DOUBLE ARRAY
    if (all_double && count > 0)
    {
        double *arr = (double *)malloc(sizeof(double) * count);
        temp = head;
        for (int i = 0; i < count; i++)
        {
            arr[i] = temp->item.value.double_v;
            temp = temp->next;
        }
        result = get_var_double_arr(arr, count);
        free(arr);
        free_temp_list(head);
        return result;
    }

    //  BOOL ARRAY
    if (all_bool && count > 0)
    {
        int *arr = (int *)malloc(sizeof(int) * count);
        temp = head;
        for (int i = 0; i < count; i++)
        {
            arr[i] = temp->item.value.bool_v;
            temp = temp->next;
        }
        result = get_var_bool_arr(arr, count);
        free(arr);
        free_temp_list(head);
        return result;
    }

    // STRING ARRAY
    if (all_string && count > 0)
    {
        char **arr = (char **)malloc(sizeof(char *) * count);
        temp = head;
        for (int i = 0; i < count; i++)
        {
            arr[i] = temp->item.value.string_v;
            temp = temp->next;
        }
        result = get_var_string_arr(arr, count);
        free(arr);
        free_temp_list(head);
        return result;
    }
    // NULL ARRAY
    if (all_null && count > 0)
    {
        result = get_var_null_arr(count);
        free_temp_list(head);
        return result;
    }

    return get_var_list(head);
}


static List *parse_array(const char **cursor)
{
    (*cursor)++;
    skip_whitespace(cursor);
    if (**cursor == ']')
    {
        (*cursor)++;
        return NULL;
    }

    variable val = parse_value(cursor);
    
    List *head = create_list(val);
    
    List *tail = head; 

    while (1)
    {
        skip_whitespace(cursor);
        if (**cursor == ']')
        {
            (*cursor)++;
            break;
        }
        if (**cursor == ',')
        {
            (*cursor)++;
            skip_whitespace(cursor);
            variable nextVal = parse_value(cursor);
            
            List *newNode = create_list(nextVal);
            
            tail->next = newNode;
            
            tail = newNode;
        }
        else
        {
            break;
        }
    }
    return head;
}

static Dict *parse_object(const char **cursor)
{
    (*cursor)++;
    skip_whitespace(cursor);
    if (**cursor == '}')
    {
        (*cursor)++;
        return NULL;
    }

    Dict *head = NULL;
    while (1)
    {
        skip_whitespace(cursor);
        char *key = parse_string_value(cursor);
        if (key == NULL) {
            break; 
        }
        skip_whitespace(cursor);
        if (**cursor == ':')
            (*cursor)++;

        variable val = parse_value(cursor);

        if (head == NULL)
            head = createDict_var(key, val);
        else
            addDict_var(head, key, val);

        free(key);

        skip_whitespace(cursor);
        if (**cursor == '}')
        {
            (*cursor)++;
            break;
        }
        if (**cursor == ',')
            (*cursor)++;
        else
            break;
    }
    return head;
}

Dict *json_parse(const char *json_string)
{
    const char *cursor = json_string;
    skip_whitespace(&cursor);

    if (*cursor == '{')
    {
        return parse_object(&cursor);
    }

    if (*cursor == '[')
    {
        List *raw_list = parse_array(&cursor);
        
        variable optimized_var = optimize_array(raw_list);

        return createDict_var("root", optimized_var);
    }

    return NULL;
}

void list_stringify(List *l, char *result)
{
    strcat(result, "[");
    List *temp = l;
    while (temp)
    {
        char buffer[1024];
        switch (temp->item.type)
        {
        case TYPE_INT:
            sprintf(buffer, "%d", temp->item.value.int_v);
            strcat(result, buffer);
            break;
        case TYPE_DOUBLE:
            sprintf(buffer, "%f", temp->item.value.double_v);
            strcat(result, buffer);
            break;
        case TYPE_STRING:
            strcat(result, "\"");
            strcat(result, temp->item.value.string_v);
            strcat(result, "\"");
            break;
        case TYPE_BOOL:
            strcat(result, temp->item.value.bool_v ? "true" : "false");
            break;
        case TYPE_NULL:
            strcat(result, "null");
            break;
        case TYPE_INT_ARR:
            strcat(result, "[");
            for (int i = 0; i < temp->item.size; i++)
            {
                sprintf(buffer, i != temp->item.size - 1 ? "%d," : "%d", temp->item.value.int_arr_v[i]);
                strcat(result, buffer);
            }
            strcat(result, "]");
            break;
        case TYPE_DOUBLE_ARR:
            strcat(result, "[");
            for (int i = 0; i < temp->item.size; i++)
            {
                sprintf(buffer, i != temp->item.size - 1 ? "%f," : "%f", temp->item.value.double_arr_v[i]);
                strcat(result, buffer);
            }
            strcat(result, "]");
            break;
        case TYPE_STRING_ARR:
            strcat(result, "[");
            for (int i = 0; i < temp->item.size; i++)
            {
                strcat(result, "\"");
                strcat(result, temp->item.value.string_arr_v[i]);
                strcat(result, "\"");
                if (i != temp->item.size - 1)
                    strcat(result, ",");
            }
            strcat(result, "]");
            break;
        case TYPE_BOOL_ARR:
            strcat(result, "[");
            for (int i = 0; i < temp->item.size; i++)
            {
                strcat(result, temp->item.value.bool_arr_v[i] ? "true" : "false");
                if (i != temp->item.size - 1)
                    strcat(result, ",");
            }
            strcat(result, "]");
            break;
        case TYPE_NULL_ARR:
            strcat(result, "[");
            for (int i = 0; i < temp->item.size; i++)
            {
                strcat(result, "null");
                if (i != temp->item.size - 1)
                    strcat(result, ",");
            }
            strcat(result, "]");
            break;
        case TYPE_DICT:
            json_stringify(temp->item.value.dict_v, result);
            break;
        case TYPE_LIST:
            list_stringify(temp->item.value.list_v, result);
            break;
        default:
            strcat(result, "null");
        }
        if (temp->next != NULL)
            strcat(result, ",");
        temp = temp->next;
    }
    strcat(result, "]");
}

void json_stringify(Dict *d, char *result)
{
    strcat(result, "{");
    Dict *temp = d;
    while (temp != NULL)
    {
        strcat(result, "\"");
        strcat(result, temp->key);
        strcat(result, "\":");
        char buffer[1024];
        switch (temp->value.type)
        {
        case TYPE_INT:
            sprintf(buffer, "%d", temp->value.value.int_v);
            strcat(result, buffer);
            break;
        case TYPE_DOUBLE:
            sprintf(buffer, "%f", temp->value.value.double_v);
            strcat(result, buffer);
            break;
        case TYPE_STRING:
            strcat(result, "\"");
            strcat(result, temp->value.value.string_v);
            strcat(result, "\"");
            break;
        case TYPE_BOOL:
            strcat(result, temp->value.value.bool_v ? "true" : "false");
            break;
        case TYPE_NULL:
            strcat(result, "null");
            break;
        case TYPE_INT_ARR:
            strcat(result, "[");
            for (int i = 0; i < temp->value.size; i++)
            {
                sprintf(buffer, i != temp->value.size - 1 ? "%d," : "%d", temp->value.value.int_arr_v[i]);
                strcat(result, buffer);
            }
            strcat(result, "]");
            break;
        case TYPE_DOUBLE_ARR:
            strcat(result, "[");
            for (int i = 0; i < temp->value.size; i++)
            {
                sprintf(buffer, i != temp->value.size - 1 ? "%f," : "%f", temp->value.value.double_arr_v[i]);
                strcat(result, buffer);
            }
            strcat(result, "]");
            break;
        case TYPE_STRING_ARR:
            strcat(result, "[");
            for (int i = 0; i < temp->value.size; i++)
            {
                strcat(result, "\"");
                strcat(result, temp->value.value.string_arr_v[i]);
                strcat(result, "\"");
            }
            strcat(result, "]");
            break;
        case TYPE_BOOL_ARR:
            strcat(result, "[");
            for (int i = 0; i < temp->value.size; i++)
            {
                strcat(result, temp->value.value.bool_arr_v[i] ? "true" : "false");
                if (i != temp->value.size - 1)
                    strcat(result, ",");
            }
            strcat(result, "]");
            break;
        case TYPE_NULL_ARR:
            strcat(result, "[");
            for (int i = 0; i < temp->value.size; i++)
            {
                strcat(result, "null");
                if (i != temp->value.size - 1)
                    strcat(result, ",");
            }
            strcat(result, "]");
            break;
        case TYPE_DICT:
            json_stringify(temp->value.value.dict_v, result);
            break;
        case TYPE_LIST:
            list_stringify(temp->value.value.list_v, result);
            break;
        default:
            strcat(result, "null");
        }
        if (temp->next != NULL)
            strcat(result, ",");
        temp = temp->next;
    }
    strcat(result, "}");
}