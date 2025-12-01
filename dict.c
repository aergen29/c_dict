#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef enum DataType
{
    TYPE_INT,
    TYPE_DOUBLE,
    TYPE_STRING,
    TYPE_BOOL,
    TYPE_NULL
} DataType;

typedef struct var
{
    DataType type;
    union
    {
        int int_v;
        double double_v;
        char *string_v;
        int bool_v;
        void *null_v;
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

            if (current->value.type == TYPE_STRING)
                free(current->value.value.string_v);

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

/* DICT TO JSON END */
void getvartests();
void createDictTest();
Dict *addDictTest();
void dictToJsonTest();
void dictArrayToJsonTest();
void deleteDictTest();
void main()
{
    printf("Hello World!\n");
    // getvartests();
    // createDictTest();
    // addDictTest();
    // dictToJsonTest();
    // dictArrayToJsonTest();
    deleteDictTest();
}

/* TESTS */

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