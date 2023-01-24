#ifndef ASSOCIATIVE_H
#define ASSOCIATIVE_H
#include "status.h"
#include "generic.h"
#include "generic_vector.h"
#include "my_string.h"

typedef struct element Element;

typedef void* ASSOCIATIVE_ARRAY;

ASSOCIATIVE_ARRAY associative_array_init_default();

Status add_element(ASSOCIATIVE_ARRAY array, MY_STRING data, MY_STRING key);

GENERIC_VECTOR find_by_key(ASSOCIATIVE_ARRAY array, MY_STRING key);

void associative_array_destroy(ASSOCIATIVE_ARRAY* array);

void list_destroy(Element* node);

Status list_insert(Element* node, MY_STRING data, MY_STRING key);

Element* new_node(Element* node, MY_STRING data, MY_STRING key);

int associative_array_get_size(ASSOCIATIVE_ARRAY array);
#endif