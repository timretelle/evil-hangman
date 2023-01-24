#ifndef MY_VECTOR_H
#define MY_VECTOR_H

#include "status.h"

typedef void* GENERIC_VECTOR;
typedef void* ITEM;

GENERIC_VECTOR generic_vector_init_default(
	void(*fpAssignment)(ITEM* pLeft, ITEM right),
	void(*fpDestroy)(ITEM* phItem));

int generic_vector_get_size(GENERIC_VECTOR hVector);
int generic_vector_get_capacity(GENERIC_VECTOR hVector);

Status generic_vector_push_back(GENERIC_VECTOR hVector, ITEM hItem);

Status generic_vector_pop_back(GENERIC_VECTOR hVector);

//shrink to fit

ITEM* generic_vector_at(GENERIC_VECTOR hVector, int index);


//set the capacity??


void generic_vector_destroy(GENERIC_VECTOR* phVector);

#endif
