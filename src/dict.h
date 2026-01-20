#ifndef DICT_H
#define DICT_H

#include "variable.h"

typedef struct Dict
{
    char *key;
    variable value;
    struct Dict *next;
} Dict;

// CREATE
Dict *createDict_var(char *, variable );
Dict *createDict_int(char *, int );
Dict *createDict_double(char *, double);
Dict *createDict_string(char *, char *);
Dict *createDict_bool(char *, int);
Dict *createDict_null(char *);
Dict *createDict_dict(char *,Dict *);
Dict *createDict_list(char *,List *);
Dict *createDict(DataType , char *, void *);

// Array Create
Dict *createDict_int_arr(char *, int[], int );
Dict *createDict_double_arr(char *, double[], int );
Dict *createDict_bool_arr(char *, int[], int );
Dict *createDict_string_arr(char *, char *[], int );
Dict *createDict_null_arr(char *, int );

// ADD
Dict *addDict_var(Dict *, char *, variable );
Dict *addDict_int(Dict *, char *, int );
Dict *addDict_double(Dict *, char *, double );
Dict *addDict_string(Dict *, char *, char *);
Dict *addDict_bool(Dict *, char *, int );
Dict *addDict_null(Dict *, char *);
Dict *addDict_dict(Dict *,char *,Dict *);
Dict *addDict_list(Dict *, char *, List *);
Dict *addDict(Dict *, DataType , char *, void *);

// Array Add
Dict *addDict_int_arr(Dict *, char *, int[], int );
Dict *addDict_double_arr(Dict *, char *, double[], int );
Dict *addDict_bool_arr(Dict *, char *, int[], int );
Dict *addDict_string_arr(Dict *, char *, char *[], int );
Dict *addDict_null_arr(Dict *, char *, int );

// MANAGEMENT
void free_dict(Dict *);
int deleteDictKey(Dict **, char *);
Dict *getDictVariable(Dict *, char *);
variable getDictVariable_var(Dict *, char *);
void setDictKey(Dict *, char *);

#endif