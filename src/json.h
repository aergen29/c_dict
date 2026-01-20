#ifndef JSON_H
#define JSON_H

#include "dict.h"

Dict *json_parse(const char *json_string);

void json_stringify(Dict *, char *);

#endif