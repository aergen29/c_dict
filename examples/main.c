#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../src/dict.h"
#include "../src/json.h"
#include "../cJSON/cJSON.h"

// Prototypes
void getvartests();
void createDictTest();
Dict *addDictTest();
void deleteDictTest();
void json_stringify_test();
void benchmark_test();

int main()
{
    printf("STRINGIFY TEST");
    json_stringify_test();
    return 0;
}

// --- Unit & Chaos Tests ---

void json_stringify_test()
{
    printf("\n--- Complex Structure Test ---\n");

    // 1. Mixed Array with double and string
    double coords[] = {36.5, 42.0, -99.9};
    List *limbo_list = create_list(get_var_double_arr(coords, 3));
    add_var_to_list(limbo_list, get_var_string("Deep Layer"));

    // 2. Nested Dict with Null
    Dict *hotel_layer = createDict_int("room_no", 1408);
    addDict_null(hotel_layer, "guest");
    addDict_var(hotel_layer, "evidence", get_var_list(limbo_list));

    // 3. Heterogeneous List
    int fib[] = {1, 1, 2, 3, 5};
    List *city_chaos = create_list(get_var_bool(1));              
    add_var_to_list(city_chaos, get_var_dict(hotel_layer));       
    add_var_to_list(city_chaos, get_var_int_arr(fib, 5));         
    add_var_to_list(city_chaos, get_var_null());                  
    add_var_to_list(city_chaos, get_var_string("Glitch")); 

    // 4. Root Construction
    Dict *config = createDict_string("status", "flying");
    addDict_double(config, "altitude", 30000.5);
    addDict_var(config, "payload", get_var_list(city_chaos));

    Dict *root = createDict_int("simulation_id", 99);
    List *server_rack = create_list(get_var_dict(config));
    add_var_to_list(server_rack, get_var_string("CHECK"));
    addDict_var(root, "servers", get_var_list(server_rack));

    int flags[] = {1, 0, 1, 0, 0, 1};
    addDict_bool_arr(root, "system_flags", flags, 6);

    // Stringify -> Parse -> Stringify Roundtrip
    char *result = malloc(8192);
    char *result2 = malloc(8192);
    
    if(result && result2) {
        result[0] = '\0';
        json_stringify(root, result);
        printf("Output 1 (Length %lu):\n%s\n", strlen(result), result);

        Dict *parsed = json_parse(result);
        result2[0] = '\0';
        json_stringify(parsed, result2);
        printf("Output 2 (Roundtrip):\n%s\n", result2);
        
        free_dict(parsed);
    }
    
    free(result);
    free(result2);
    free_dict(root);
}

void deleteDictTest()
{
    printf("\n--- Delete Test ---\n");
    Dict *dict = addDictTest();
    
    deleteDictKey(&dict, "name");
    deleteDictKey(&dict, "salary");
    
    char result[1024] = {0};
    json_stringify(dict, result);
    printf("After Delete: %s\n", result);
    
    free_dict(dict);
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
    printf("\n--- Create Dict Var Test ---\n");
    Dict *d1 = createDict_var("int", get_var_int(329));
    Dict *d2 = createDict_var("double", get_var_double(329.923));
    Dict *d3 = createDict_var("string", get_var_string("TEST"));
    Dict *d4 = createDict_var("bool", get_var_bool(0));

    printf("Int: %d, Double: %f, String: %s\n", 
           d1->value.value.int_v, 
           d2->value.value.double_v, 
           d3->value.value.string_v);

    free_dict(d1); free_dict(d2); free_dict(d3); free_dict(d4);
}

void getvartests()
{
    printf("\n--- Variable Wrapper Test ---\n");
    variable v1 = get_var_int(329);
    variable v2 = get_var_double(329.923);
    variable v3 = get_var_string("TEST");
    
    printf("Int: %d\nDouble: %f\nString: %s\n", 
           v1.value.int_v, v2.value.double_v, v3.value.string_v);
}