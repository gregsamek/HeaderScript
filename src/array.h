#ifndef ARRAY_H
#define ARRAY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*

typedef struct Array_X
{
    X* arr;
    int len;
    int cap;
} Array_X;

*/

#define Array_Initialize(ARRAY, N_ELEMENTS) \
    do \
    { \
        int elements_per_page = 4096 / sizeof((ARRAY)->arr[0]);\
        int new_cap; \
        if (N_ELEMENTS < elements_per_page) \
            new_cap = elements_per_page; \
        else if (N_ELEMENTS % elements_per_page == 0) \
            new_cap = N_ELEMENTS; \
        else \
            new_cap = (N_ELEMENTS / elements_per_page + 1) * elements_per_page; \
        void* temp_ptr = realloc((ARRAY)->arr, new_cap * sizeof((ARRAY)->arr[0])); \
        if (temp_ptr == NULL) \
        { \
            fprintf(stderr, "Failed to initialize Array memory!"); \
            memset((ARRAY), 0, sizeof(*(ARRAY))); \
        } \
        else \
        { \
            (ARRAY)->arr = temp_ptr; \
            (ARRAY)->cap = new_cap; \
            (ARRAY)->len = 0; \
        } \
    } while (0)

#define Array_Append(ARRAY, ELEMENT) \
    do \
    { \
        if ((ARRAY)->len >= (ARRAY)->cap) \
        { \
            int new_cap = (ARRAY)->cap * 1.5; \
            void* temp_ptr = realloc((ARRAY)->arr, new_cap * sizeof((ARRAY)->arr[0])); \
            if (temp_ptr == NULL) \
            { \
                fprintf(stderr, "Failed to reallocate array memory!"); \
            } \
            else \
            { \
                (ARRAY)->arr = temp_ptr; \
                (ARRAY)->cap = new_cap; \
            } \
        } \
        else \
        { \
            (ARRAY)->arr[(ARRAY)->len] = ELEMENT; \
            (ARRAY)->len++; \
        } \
    } while (0)

# define Array_Concat(ARRAY1, ARRAY2) \
    do \
    { \
        int new_cap = (ARRAY1)->len + (ARRAY2)->len; \
        void* temp_ptr = realloc((ARRAY1)->arr, new_cap * sizeof((ARRAY1)->arr[0])); \
        if (temp_ptr == NULL) \
        { \
            fprintf(stderr, "Failed to concatenate arrays!"); \
        } \
        else \
        { \
            (ARRAY1)->arr = temp_ptr; \
            memcpy((ARRAY1)->arr + (ARRAY1)->len, (ARRAY2)->arr, (ARRAY2)->len * sizeof((ARRAY2)->arr[0])); \
            (ARRAY1)->len += (ARRAY2)->len; \
        } \
    } while (0)

#endif // ARRAY_H