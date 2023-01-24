#include <stdio.h>
#include <stdlib.h>
#include "associative.h"

struct element//one element of the associative array
{//each element has a vector of strings, a key, and a pointer to the next element
  GENERIC_VECTOR values;
  MY_STRING key;
  Element* next;
  //int height;
};

typedef struct associativeArray {
	int size;
	Element* root;//the associative array has the root
} AssociativeArray;

ASSOCIATIVE_ARRAY associative_array_init_default(){
	AssociativeArray* pAssociativeArray;
	pAssociativeArray = (AssociativeArray*)malloc(sizeof(AssociativeArray));
	if(pAssociativeArray != NULL){
		pAssociativeArray->size = 0;
		pAssociativeArray->root = NULL;//set this to null so it will get initialized by the first item we add
	}
	else{
		printf("Failed to allocate.\n");
		return NULL;
	}
	return pAssociativeArray;
}

Status add_element(ASSOCIATIVE_ARRAY array, MY_STRING data, MY_STRING key){
	AssociativeArray* pA = (AssociativeArray*)array;
	if(list_insert(pA->root, data, key) == FAILURE){//if this returns failure, we need to make a new node
		pA->root = new_node(pA->root, data, key);//add a new node
	}
	(pA->size)++;//we added something, so increment size
	return SUCCESS;
}

Element* new_node(Element* node, MY_STRING data, MY_STRING key){
	Element* newNode = (Element*)malloc(sizeof(Element));
	if(node = NULL){
		printf("failed to allocate new node.\n");
		return NULL;
	}
	newNode->values = generic_vector_init_default(my_string_assignment, my_string_destroy);//make the node's vector
	generic_vector_push_back(newNode->values, (ITEM)data);//add the string
	newNode->key = my_string_init_c_string(my_string_c_str(key));//initialize the key
	newNode->next = node;//make it point to the rest of the list
	return newNode;
}
	
Status list_insert(Element* node, MY_STRING data, MY_STRING key){//returns success if the string was added to an existing node, returns failure if a new node needs to be created
	Status previous;
	if(node == NULL){
		return FAILURE;
	}
	if(my_string_equals(node->key, key)){//if we find a node that has the same key...
		generic_vector_push_back(node->values, (ITEM)data);//add the string to that node's vector of strings
		return SUCCESS;
	}
	previous = list_insert(node->next, data, key);
	if(previous == SUCCESS){
		return SUCCESS;
	}
}

GENERIC_VECTOR find_by_key(ASSOCIATIVE_ARRAY array, MY_STRING key){
	AssociativeArray* pA = (AssociativeArray*)array;
	Element* temp = pA->root;
	while(temp != NULL){
		if(my_string_equals(temp->key, key)){//if we found the key, return the generic vector
			return temp->values;
		}
		temp = temp->next;
	}
	return NULL;
}

int associative_array_get_size(ASSOCIATIVE_ARRAY array){
	AssociativeArray* pA = (AssociativeArray*)array;
	return pA->size;
}

void associative_array_destroy(ASSOCIATIVE_ARRAY* array){
	AssociativeArray* pA = (AssociativeArray*)*array;
	list_destroy(pA->root);
	free(pA);
	*array = NULL;
}

void list_destroy(Element* node){
	if(node == NULL){//base case
		return;
	}
	list_destroy(node->next);//recursively visit every node
	free(node);//destroy the node
	node = NULL;
}
