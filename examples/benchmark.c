#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../src/dict.h"
#include "../src/json.h"
#include "../cJSON/cJSON.h"

void benchmark_comparison();
void benchmark_redemption();
char* create_huge_json_array(int);

int main()
{
    printf("Starting C-Dict Benchmarks...\n");
    benchmark_redemption(); 
    benchmark_comparison(); // Optional: Enable for generic comparison
    return 0;
}

// --- Helpers ---

char* create_huge_json_array(int size) 
{
    // Estimate size: ~7 chars per number (digits + comma) + overhead
    char *buffer = malloc(size * 7 + 10);
    if (!buffer) return NULL;

    char *p = buffer;
    p += sprintf(p, "[");
    for (int i = 0; i < size; i++) 
    {
        p += sprintf(p, "%d", i);
        if (i < size - 1) *p++ = ',';
    }
    *p++ = ']';
    *p = '\0';
    return buffer;
}

// --- Benchmarks ---

void benchmark_redemption() 
{
    int ARRAY_SIZE = 1000000;
    printf("\n--- Large Array Benchmark (%d items) ---\n", ARRAY_SIZE);
    
    printf("Generating JSON... ");
    fflush(stdout);
    char *json_str = create_huge_json_array(ARRAY_SIZE);
    printf("Done.\n");

    clock_t start, end;

    // 1. C-DICT
    start = clock();
    Dict *my_root = json_parse(json_str); 
    
    long long my_sum = 0;
    if (my_root && my_root->value.type == TYPE_INT_ARR) 
    {
        int *arr = my_root->value.value.int_arr_v;
        int count = my_root->value.size;
        for (int i = 0; i < count; i++) 
        {
            my_sum += arr[i]; 
        }
    } 
    else 
    {
        printf("[Error] C-Dict optimization failed.\n");
    }
    end = clock();
    printf("[C-DICT] Parse + Sum: %.4f sec\n", ((double)(end - start)) / CLOCKS_PER_SEC);

    // 2. cJSON
    start = clock();
    cJSON *cj_root = cJSON_Parse(json_str); 
    
    long long cj_sum = 0;
    if (cj_root) 
    {
        cJSON *child = cj_root->child;
        while (child) 
        {
            cj_sum += child->valueint;
            child = child->next;
        }
    }
    end = clock();
    printf("[cJSON ] Parse + Sum: %.4f sec\n", ((double)(end - start)) / CLOCKS_PER_SEC);

    // Verification
    printf("Checksum: C-Dict: %lld | cJSON: %lld\n", my_sum, cj_sum);

    // Cleanup
    free(json_str);
    free_dict(my_root);
    cJSON_Delete(cj_root);
}

void benchmark_comparison()
{
    int TEST_SIZE = 10000;
    char key[32];
    clock_t start, end;

    printf("\n--- Insert & Stringify Benchmark (%d items) ---\n", TEST_SIZE);

    // 1. C-DICT Insert
    Dict *my_root = createDict_string("test", "start");
    start = clock();
    for (int i = 0; i < TEST_SIZE; i++)
    {
        sprintf(key, "k%d", i);
        addDict_int(my_root, key, i);
    }
    end = clock();
    printf("[C-DICT] Insert: %.4f sec\n", ((double)(end - start)) / CLOCKS_PER_SEC);

    // 2. cJSON Insert
    cJSON *cj_root = cJSON_CreateObject();
    start = clock();
    for (int i = 0; i < TEST_SIZE; i++)
    {
        sprintf(key, "k%d", i);
        cJSON_AddNumberToObject(cj_root, key, i);
    }
    end = clock();
    printf("[cJSON ] Insert: %.4f sec\n", ((double)(end - start)) / CLOCKS_PER_SEC);

    // 3. C-DICT Stringify
    // Note: Allocating large buffer on stack might cause overflow on small stacks
    char *my_buf = malloc(1024 * 1024 * 2); 
    if(my_buf) {
        my_buf[0] = '\0';
        start = clock();
        json_stringify(my_root, my_buf); 
        end = clock();
        printf("[C-DICT] Print : %.4f sec\n", ((double)(end - start)) / CLOCKS_PER_SEC);
        free(my_buf);
    }

    // 4. cJSON Stringify
    start = clock();
    char *cj_str = cJSON_PrintUnformatted(cj_root); 
    end = clock();
    printf("[cJSON ] Print : %.4f sec\n", ((double)(end - start)) / CLOCKS_PER_SEC);

    // Cleanup
    free_dict(my_root);
    cJSON_Delete(cj_root);
    free(cj_str);
}

void benchmark_test()
{
    printf("\n--- Internal Performance Test ---\n");

    int TEST_SIZE = 10000; 
    Dict *root = createDict_string("benchmark", "start");

    // Insert
    clock_t start = clock();
    char key[32];
    for (int i = 0; i < TEST_SIZE; i++)
    {
        sprintf(key, "key_%d", i);
        addDict_int(root, key, i);
    }
    clock_t end = clock();
    printf("[INSERT] %d items: %.4f sec\n", TEST_SIZE, ((double)(end - start)) / CLOCKS_PER_SEC);

    // Lookup
    start = clock();
    for (int i = 0; i < TEST_SIZE; i++)
    {
        sprintf(key, "key_%d", i);
        getDictVariable(root, key); 
    }
    end = clock();
    printf("[LOOKUP] %d items: %.4f sec\n", TEST_SIZE, ((double)(end - start)) / CLOCKS_PER_SEC);

    // Stringify
    char *json_buffer = (char *)malloc(1024 * 1024 * 10);
    if (json_buffer)
    {
        start = clock();
        json_stringify(root, json_buffer);
        end = clock();
        printf("[JSON] Stringify: %.4f sec\n", ((double)(end - start)) / CLOCKS_PER_SEC);
        free(json_buffer);
    }

    free_dict(root);
}