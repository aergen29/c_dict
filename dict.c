#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
    TYPE_NULL_ARR
} DataType;

typedef struct var
{
    DataType type;
    size_t size;
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
    } value;
} var;

typedef struct Dict
{
    char *key;
    var value;
    struct Dict *next;
} Dict;

/* Function Initialize */
Dict *createDict_var(char *, var);
Dict *createDict_int(char *, int);
var get_var_int(int);
var get_var_double(double);
var get_var_string(char *);
var get_var_bool(int);
var get_var_null();
var get_var(DataType, void *);
var get_var_int_arr(int *, int);
var get_var_double_arr(double *, int);
var get_var_bool_arr(int *, int);
var get_var_string_arr(char **, int);

/***    GET VAR   ****/
var get_var(DataType type, void *val)
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

var get_var_arr(DataType type, void *val, int length)
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

var get_var_int(int val)
{
    var result;
    result.type = TYPE_INT;
    result.value.int_v = val;
    return result;
}
var get_var_double(double val)
{
    var result;
    result.type = TYPE_DOUBLE;
    result.value.double_v = val;
    return result;
}
var get_var_string(char *val)
{
    var result;
    result.type = TYPE_STRING;
    int len = strlen(val);
    char *strVal = (char *)malloc(sizeof(char) * (len + 1));
    strncpy(strVal, val, len);
    strVal[len] = '\0';
    result.value.string_v = strVal;
    return result;
}
var get_var_bool(int val)
{
    var result;
    result.type = TYPE_BOOL;
    result.value.bool_v = val;
    return result;
}
var get_var_null()
{
    var result;
    result.type = TYPE_NULL;
    result.value.null_v = NULL;
    return result;
}

var get_var_int_arr(int val[], int length)
{
    var result;
    result.type = TYPE_INT_ARR;
    result.size = length;
    int *arr = (int *)malloc(sizeof(int) * length);
    for (int i = 0; i < length; i++)
        arr[i] = val[i];
    result.value.int_arr_v = arr;
    return result;
}

var get_var_bool_arr(int val[], int length)
{
    var result;
    result.type = TYPE_BOOL_ARR;
    result.size = length;
    int *arr = (int *)malloc(sizeof(int) * length);
    for (int i = 0; i < length; i++)
        arr[i] = val[i];
    result.value.bool_arr_v = arr;
    return result;
}

var get_var_double_arr(double val[], int length)
{
    var result;
    result.type = TYPE_DOUBLE_ARR;
    result.size = length;
    double *arr = (double *)malloc(sizeof(double) * length);
    for (int i = 0; i < length; i++)
        arr[i] = val[i];
    result.value.double_arr_v = arr;
    return result;
}

var get_var_string_arr(char *val[], int length)
{
    var result;
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

var get_var_null_arr(int length)
{
    var result;
    result.type = TYPE_NULL_ARR;
    result.size = length;
    return result;
}

/* GET VAR END */

/***    CREATE DICT   ****/

Dict *createDict_var(char *key, var variable)
{
    Dict *result = (Dict *)malloc(sizeof(Dict));
    char *newKey = (char *)malloc(sizeof(char) * strlen(key));
    strcpy(newKey, key);
    result->key = newKey;
    result->value = variable;
    return result;
}

Dict *createDict_int(char *key, int value)
{
    var variable = get_var_int(value);
    return createDict_var(key, variable);
}
Dict *createDict_double(char *key, double value)
{
    var variable = get_var_double(value);
    return createDict_var(key, variable);
}
Dict *createDict_string(char *key, char *value)
{
    var variable = get_var_string(value);
    return createDict_var(key, variable);
}
Dict *createDict_bool(char *key, int value)
{
    var variable = get_var_bool(value);
    return createDict_var(key, variable);
}
Dict *createDict_null(char *key)
{
    var variable = get_var_null();
    return createDict_var(key, variable);
}

Dict *createDict_int_arr(char *key, int val[], int length)
{
    var variable = get_var_int_arr(val, length);
    return createDict_var(key, variable);
}
Dict *createDict_double_arr(char *key, double val[], int length)
{
    var variable = get_var_double_arr(val, length);
    return createDict_var(key, variable);
}

Dict *createDict_bool_arr(char *key, int val[], int length)
{
    var variable = get_var_bool_arr(val, length);
    return createDict_var(key, variable);
}

Dict *createDict_string_arr(char *key, char *val[], int length)
{
    var variable = get_var_string_arr(val, length);
    return createDict_var(key, variable);
}

Dict *createDict_null_arr(char *key, int length)
{
    var variable = get_var_null_arr(length);
    return createDict_var(key, variable);
}

Dict *createDict(DataType type, char *key, void *value)
{
    var variable = get_var(type, value);
    return createDict_var(key, variable);
}

/* CREATE DICT END */
/***** ADD DICT *****/

Dict *addDict_var(Dict *d, char *key, var variable)
{
    Dict *result = (Dict *)malloc(sizeof(Dict));
    char *newKey = (char *)malloc(sizeof(char) * strlen(key));
    strcpy(newKey, key);
    result->key = newKey;
    result->value = variable;
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
    var variable = get_var_int(value);
    return addDict_var(d, key, variable);
}
Dict *addDict_double(Dict *d, char *key, double value)
{
    var variable = get_var_double(value);
    return addDict_var(d, key, variable);
}
Dict *addDict_string(Dict *d, char *key, char *value)
{
    var variable = get_var_string(value);
    return addDict_var(d, key, variable);
}
Dict *addDict_bool(Dict *d, char *key, int value)
{
    var variable = get_var_bool(value);
    return addDict_var(d, key, variable);
}
Dict *addDict_null(Dict *d, char *key)
{
    var variable = get_var_null();
    return addDict_var(d, key, variable);
}

Dict *addDict(Dict *d, DataType type, char *key, void *value)
{
    var variable = get_var(type, value);
    return addDict_var(d, key, variable);
}

Dict *addDict_int_arr(Dict *d, char *key, int val[], int length)
{
    var variable = get_var_int_arr(val, length);
    return addDict_var(d, key, variable);
}

Dict *addDict_double_arr(Dict *d, char *key, double val[], int length)
{
    var variable = get_var_double_arr(val, length);
    return addDict_var(d, key, variable);
}

Dict *addDict_bool_arr(Dict *d, char *key, int val[], int length)
{
    var variable = get_var_bool_arr(val, length);
    return addDict_var(d, key, variable);
}

Dict *addDict_string_arr(Dict *d, char *key, char *val[], int length)
{
    var variable = get_var_string_arr(val, length);
    return addDict_var(d, key, variable);
}

Dict *addDict_null_arr(Dict *d, char *key, int length)
{
    var variable = get_var_null_arr(length);
    return addDict_var(d, key, variable);
}

/* ADD DICT END */

/***** DELETE DICT *****/
int deleteDict(Dict **head, char *key)
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
void getDictVariable_var(Dict *d, char *key, var *variable)
{
    Dict *dict = getDictVariable(d, key);
    if (dict == NULL)
        variable = NULL;
    else
        variable = &dict->value;
}

/* GET DICT VARIABLE END */

/***** SET DICT *****/

void setDictKey(Dict *d, char *key)
{
    char *newKey = (char *)malloc(sizeof(char) * strlen(key));
    strcpy(newKey, key);
    d->key = newKey;
}

/* SET DICT END */

/***** DICT TO JSON *****/

Dict *jsonToDict(char *json)
{
    Dict *result = (Dict *)malloc(sizeof(Dict));
    char temp[250];
    char *statusOne, *statusTwo;
    char *line = strtok_r(json, ",", &statusOne);
    int is_head = 1;
    while (line != NULL)
    {
        DataType type = TYPE_INT;
        char *key = strtok_r(line, ":", &statusTwo);
        /* Divide */
        if (key[0] == '{')
            key = key + 1;
        if (key[0] == '"')
            key = key + 1;
        if (key[strlen(key) - 1] == '"')
            key[strlen(key) - 1] = '\0';
        char *value = strtok_r(NULL, ":", &statusTwo);
        if (value[0] == '"')
        {
            type = TYPE_STRING;
            value = value + 1;
        }
        if (value[strlen(value) - 1] == '}')
            value[strlen(value) - 1] = '\0';
        if (value[strlen(value) - 1] == '"')
            value[strlen(value) - 1] = '\0';
        /* Divide END */

        /* FIND TYPE */
        if (type == TYPE_STRING)
            ;
        else if (strncmp(value, "null", 4) == 0 && strlen(value) == 4)
            type = TYPE_NULL;
        else if ((strncmp(value, "true", 4) == 0 && strlen(value) == 4) || (strncmp(value, "false", 5) == 0 && strlen(value) == 5))
            type = TYPE_BOOL;
        else if (strchr(value, '.') != NULL)
            type = TYPE_DOUBLE;
        else
            type = TYPE_INT;
        /* FIND TYPE END */

        /* CREATE DICT */

        switch (type)
        {
        case TYPE_STRING:
            if (is_head)
                result = createDict(TYPE_STRING, key, value);
            else
                addDict(result, TYPE_STRING, key, value);
            break;
        case TYPE_INT:
            if (is_head)
                result = createDict_int(key, atoi(value));
            else
                addDict_int(result, key, atoi(value));
            break;
        case TYPE_DOUBLE:
            if (is_head)
                result = createDict_double(key, strtod(value, NULL));
            else
                addDict_double(result, key, strtod(value, NULL));
            break;
        case TYPE_BOOL:
            if (is_head)
                result = createDict_bool(key, strcmp(value, "true") == 0);
            else
                addDict_bool(result, key, strcmp(value, "true") == 0);
            break;
        case TYPE_NULL:
            if (is_head)
                result = createDict_null(key);
            else
                addDict_null(result, key);
            break;
        }
        is_head = 0;

        /* CREATE DICT END */

        line = strtok_r(NULL, ",", &statusOne);
    }
    return result;
}

void dictToJson(Dict *dict, char *result)
{
    strcat(result, "{");
    Dict *temp = dict;
    while (temp != NULL)
    {
        strcat(result, "\"");
        strcat(result, temp->key);
        strcat(result, "\":");
        char str[100];
        switch (temp->value.type)
        {
        case TYPE_INT:
            sprintf(str, "%d", temp->value.value.int_v);
            strcat(result, str);
            break;
        case TYPE_DOUBLE:
            sprintf(str, "%f", temp->value.value.double_v);
            strcat(result, str);
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
        default:
            strcat(result, "null");
            break;
        }
        if (temp->next != NULL)
            strcat(result, ",");
        temp = temp->next;
    }
    strcat(result, "}");
}

void dictArrayToJson(Dict *dicts[], int length, char *result)
{
    *result = '\0';
    strcat(result, "[");
    for (int i = 0; i < length; i++)
    {
        if (dicts[i] == NULL)
            continue;
        char temp[256] = {0};
        dictToJson(dicts[i], temp);
        strcat(result, temp);
        if (i != length - 1)
            strcat(result, ",");
    }
    strcat(result, "]");
}

/* ARRAY VALUES */
void int_arr_to_json(int arr[], char *result, int length)
{
    *result = '[';
    result[1] = '\0';
    int i = 0;
    char snum[11];
    for (; i < length; i++)
    {
        sprintf(snum, "%d,", arr[i]);
        strcat(result, snum);
    }
    result[strlen(result) - 1] = '\0';
    strcat(result, "]");
}
void double_arr_to_json(double arr[], char *result, int length)
{
    *result = '[';
    result[1] = '\0';
    int i = 0;
    char snum[24];
    for (; i < length; i++)
    {
        sprintf(snum, "%f,", arr[i]);
        strcat(result, snum);
    }
    result[strlen(result) - 1] = '\0';
    strcat(result, "]");
}
void bool_arr_to_json(int arr[], char *result, int length)
{
    *result = '[';
    result[1] = '\0';
    int i = 0;
    char snum[8];
    for (; i < length; i++)
    {
        sprintf(snum, "%s,", arr[i] == 0 ? "false" : "true");
        strcat(result, snum);
    }
    result[strlen(result) - 1] = '\0';
    strcat(result, "]");
}

void null_arr_to_json(char *result, int length)
{
    *result = '[';
    result[1] = '\0';
    int i = 0;
    char snum[8];
    for (; i < length; i++)
    {
        sprintf(snum, "%s,", "null");
        strcat(result, snum);
    }
    result[strlen(result) - 1] = '\0';
    strcat(result, "]");
}

void string_arr_to_json(char *arr[], char *result, int length)
{
    *result = '[';
    result[1] = '\0';
    int i = 0;
    char snum[1024];
    for (; i < length; i++)
    {
        sprintf(snum, "\"%s\",", arr[i]);
        strcat(result, snum);
    }
    result[strlen(result) - 1] = '\0';
    strcat(result, "]");
}
/*----------*/
void dictToJson_arr(Dict *dict, char *result)
{
    *result = '{';
    *(result + 1) = '\0';
    Dict *temp = dict;
    char str[100];
    while (temp != NULL)
    {
        strcat(result, "\"");
        strcat(result, temp->key);
        strcat(result, "\":");
        char tempStr[1024] = {0};
        switch (temp->value.type)
        {
        case TYPE_INT_ARR:
            int_arr_to_json(temp->value.value.int_arr_v, tempStr, temp->value.size);
            break;
        case TYPE_DOUBLE_ARR:
            double_arr_to_json(temp->value.value.double_arr_v, tempStr, temp->value.size);
            break;
        case TYPE_NULL_ARR:
            null_arr_to_json(tempStr, temp->value.size);
            break;
        case TYPE_BOOL_ARR:
            bool_arr_to_json(temp->value.value.bool_arr_v, tempStr, temp->value.size);
            break;
        case TYPE_STRING_ARR:
            string_arr_to_json(temp->value.value.string_arr_v, tempStr, temp->value.size);
            break;
        case TYPE_INT:
            sprintf(str, "%d", temp->value.value.int_v);
            strcat(result, str);
            break;
        case TYPE_DOUBLE:
            sprintf(str, "%f", temp->value.value.double_v);
            strcat(result, str);
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
        default:
            strcat(result, "null");
            break;
        }
        strcat(result, tempStr);
        strcat(result,",");
        temp = temp->next;
    }
    result[strlen(result)-1] = '}';
}
/* ARRAY VALUES */

/* DICT TO JSON END */
void getvartests();
void createDictTest();
Dict *addDictTest();
void dictToJsonTest();
void dictArrayToJsonTest();
void deleteDictTest();
void dictToJson_arrTest();
void main()
{
    printf("Hello World!\n");
    // char result[1024] = {0};
    // int arr[] = {1, 3, 698, 67, 36, 4156, 7855, 631, 75};
    // int_arr_to_json(arr, result, 9);
    // printf("%s\n", result);
    dictToJson_arrTest();
    // getvartests();
    // createDictTest();
    // addDictTest();
    // dictToJsonTest();
    // dictArrayToJsonTest();
    // deleteDictTest();
}

/* TESTS */
void dictToJson_arrTest()
{
    Dict *dict = createDict_bool("bool",0);
    addDict_double(dict,"double",15.68);
    addDict_int(dict,"int",29);
    addDict_null(dict,"null");
    addDict_string(dict,"string","ALLAME");
    int barr[7] = {0,0,0,1,0,1,1};
    double darr[5] = {1.52,2.29,3.45,8.75,10.04};
    int iarr[4] = {2,9,29,2929};
    char *strarr[3] = {"FREEMAN","WAS","HERE"};
    addDict_bool_arr(dict,"boolarr",barr,7);
    addDict_double_arr(dict,"doublearr",darr,5);
    addDict_int_arr(dict,"intarr",iarr,4);
    addDict_null_arr(dict,"nullarr",22);
    addDict_string_arr(dict,"stringarr",strarr,3);
    char json[1024]={0};
    dictToJson_arr(dict,json);
    printf("\ntest\n\t%s\n",json);
}

void deleteDictTest()
{
    Dict *dict = addDictTest();
    char result[1024] = {0};
    dictToJson(dict, result);
    printf("%s\n", result);
    *result = '\0';
    deleteDict(&dict, "name");
    dictToJson(dict, result);
    printf("%s\n", result);
    *result = '\0';
    deleteDict(&dict, "salary");
    dictToJson(dict, result);
    printf("%s\n", result);
}

void dictArrayToJsonTest()
{
    char result[2048] = {0};
    Dict *dicts[5];
    for (int i = 0; i < 5; i++)
    {
        dicts[i] = addDictTest();
    }
    setDictKey(getDictVariable(dicts[3], "age"), "ageV2");
    Dict *tempDict = getDictVariable(dicts[3], "address");
    if (tempDict == NULL)
        printf("NOT FOUNDED");
    else
        tempDict->value = get_var_string("Fatih mah. Şalgamlı cad. no:35 daire:1");
    dictArrayToJson(dicts, 5, result);
    printf("%s\n", result);
}

void dictToJsonTest()
{
    char result[1024] = {0}, result2[1024] = {0};
    Dict *dict = addDictTest();
    dictToJson(dict, result);
    printf("%s\n", result);
    Dict *dict2 = jsonToDict(result);
    dictToJson(dict2, result2);
    printf("%s\n", result2);
}
Dict *addDictTest()
{
    Dict *dict = createDict(TYPE_STRING, "name", "Abdullah");
    addDict_int(dict, "age", 23);
    addDict_bool(dict, "is_verified", 1);
    addDict_null(dict, "address");
    addDict_double(dict, "salary", 21001.15);
    addDict(dict, TYPE_STRING, "role", "admin");
    return dict;
}

void createDictTest()
{
    /*DICT VAR*/

    // INT
    Dict *dict_var_int = createDict_var("int", get_var_int(329));
    printf("dict_var_int=>key:%s\tvalue:%d\n", dict_var_int->key, dict_var_int->value.value.int_v);
    // DOUBLE
    Dict *dict_var_double = createDict_var("double", get_var_double(329.923));
    printf("dict_var_double=>key:%s\tvalue:%f\n", dict_var_double->key, dict_var_double->value.value.double_v);
    // STRING
    Dict *dict_var_string = createDict_var("string", get_var_string("PIRaNHA"));
    printf("dict_var_string=>key:%s\tvalue:%s\n", dict_var_string->key, dict_var_string->value.value.string_v);
    // BOOL
    Dict *dict_var_bool = createDict_var("bool", get_var_bool(0));
    printf("dict_var_bool=>key:%s\tvalue:%s\n", dict_var_bool->key, dict_var_bool->value.value.bool_v ? "true" : "false");

    /*DICT VAR END*/
}

void getvartests()
{
    // INT
    var int_var = get_var_int(329);
    printf("int_var:%d\n", int_var.value.int_v);
    // DOUBLE
    var double_var = get_var_double(329.923);
    printf("double_var:%f\n", double_var.value.double_v);
    // STRING
    var string_var = get_var_string("I AM HERE");
    printf("string_var:%s\n", string_var.value.string_v);
    // bool
    var bool_var = get_var_double(0);
    printf("bool_var:%s\n", bool_var.value.bool_v ? "true" : "false");

    printf("\n");

    // INT get_var
    int int_value = 29;
    var int_v = get_var(TYPE_INT, &int_value);
    printf("int_v:%d\n", int_v.value.int_v);
    // DOUBLE get_var
    double double_value = 29.68546;
    var double_v = get_var(TYPE_DOUBLE, &double_value);
    printf("double_v:%f\n", double_v.value.double_v);
    // STRING get_var
    char *string_value = "FREEMAN WAS HERE";
    var string_v = get_var(TYPE_STRING, string_value);
    printf("string_v:%s\n", string_v.value.string_v);
    // STRING get_var
    var string_v2 = get_var(TYPE_STRING, "FREEMAN WAS HERE2");
    printf("string_v:%s\n", string_v2.value.string_v);
    // BOOL get_var
    int bool_value = 1;
    var bool_v = get_var(TYPE_DOUBLE, &bool_value);
    printf("int_v:%s\n", bool_v.value.bool_v ? "true" : "false");
}