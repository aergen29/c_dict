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
    result.type = TYPE_INT;
    int len = strlen(val);
    char *strVal = (char *)malloc(sizeof(char) * (len));
    strncpy(strVal, val, len);
    result.value.string_v = strVal;
    return result;
}
var get_var_bool(int val)
{
    var result;
    result.type = TYPE_DOUBLE;
    result.value.bool_v = val;
    return result;
}
var get_var_null()
{
    var result;
    result.type = TYPE_INT;
    result.value.null_v = NULL;
    return result;
}
/* GET VAR END */

/***    CREATE DICT   ****/

Dict *createDict_var(char *key, var variable)
{
    Dict *result = (Dict *)malloc(sizeof(Dict));
    result->key = key;
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
    result->key = key;
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
void getvartests();
void createDictTest();
void main()
{
    printf("Hello World!\n");
    // getvartests();
    createDictTest();
}

/* TESTS */
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
    // BOOL get_var
    int bool_value = 1;
    var bool_v = get_var(TYPE_DOUBLE, &bool_value);
    printf("int_v:%s\n", bool_v.value.bool_v ? "true" : "false");
}