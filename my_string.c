#include <stdio.h>
#include <stdlib.h>
#include "my_string.h"

typedef struct my_string {
  int size;
  int capacity;
  char *data;
} My_string;

MY_STRING my_string_init_default(void){
  My_string* pMy_string;
  pMy_string = (My_string*)malloc(sizeof(My_string));
  if(pMy_string != NULL){
    pMy_string->size = 0;
    pMy_string->capacity = 7;
    pMy_string->data = (char*)(malloc(sizeof(char)*pMy_string->capacity));
    if(pMy_string->data == NULL){
      free(pMy_string);
      pMy_string = NULL;
    }
  }

  return pMy_string;
}
MY_STRING my_string_init_c_string(const char* c_string){
  My_string* pMy_string;
  pMy_string = (My_string*)malloc(sizeof(My_string));
  if(pMy_string != NULL){
    int i = 0;
    while(c_string[i] != '\0'){
      i++;
    }
    pMy_string->capacity = i+1;
    pMy_string->size = i;
    pMy_string->data = (char*)malloc(sizeof(char)*pMy_string->capacity);
    if(pMy_string->data == NULL){
      free(pMy_string);
    }
	else{
		i = 0;
		while(i < pMy_string->size){
			pMy_string->data[i] = c_string[i];
			i++;
		}
    return pMy_string;
	}
  }
  return NULL;
}

int my_string_get_size(MY_STRING hMy_string){
  My_string* pMy_string = (My_string*)hMy_string;
  return pMy_string->size;
}

int my_string_get_capacity(MY_STRING hMy_string){
  My_string* pMy_string = (My_string*)hMy_string;
  return pMy_string->capacity;
}

int my_string_compare(MY_STRING hLeft_string, MY_STRING hRight_string){
  My_string* pLeft_string = (My_string*)hLeft_string;
  My_string* pRight_string = (My_string*)hRight_string;
  if(pLeft_string->size < pRight_string->size){
    return -1;
  }
  else if(pLeft_string->size > pRight_string->size){
    return 1;
  }
  else{
    return 0;
  }  
}

void my_string_destroy(Item* pItem){
  My_string* pMy_string = (My_string*)*pItem;
  free(pMy_string->data);
  free(pMy_string);
  *pItem = NULL;
}

Status my_string_extraction(MY_STRING hMy_string, FILE* fp){
  My_string* pMy_string = (My_string*)hMy_string;
  char c = fgetc(fp);
  char* pTemp;
  int i = 0;
  int count = 0;
  if(c == EOF){
    return FAILURE;
  }
  
  
  if(fp == NULL){
    printf("Failed to open file\n");
    return FAILURE;
  }
  while(c == ' ' || c == '\n'){
    c = fgetc(fp);
  }  
  while(c != ' ' && c != EOF && c != '\n'){
    pMy_string->size = count;
    if(pMy_string->size >= pMy_string->capacity){
      pTemp = (malloc(sizeof(char)*pMy_string->capacity*2));
      if(pTemp == NULL){
	return FAILURE;
      }
      for(i = 0; i < pMy_string->size; i++){
	pTemp[i] = pMy_string->data[i];
      }
      free(pMy_string->data);
      pMy_string->data = pTemp;
      pMy_string->capacity *= 2;
    }
    pMy_string->data[count] = c;
    count++;
    pMy_string->size = count;
    c = fgetc(fp);
  }
  if(c == ' ' && count != 0){
    ungetc(c, fp);
    return SUCCESS;
  }
  if(count == 0){
    return FAILURE;
  }
  return SUCCESS;
 }
Status my_string_insertion(MY_STRING hMy_string, FILE* fp){
  My_string* pMy_string = (My_string*)hMy_string;
  int i;
  for(i = 0; i < pMy_string->size; i++){
    fputc(pMy_string->data[i], fp);
  }
  if(pMy_string->data[0] == '\0'){
    return FAILURE;
  }
  return SUCCESS;
}

Status my_string_push_back(MY_STRING hMy_string, char item){
    My_string *pMy_string = (My_string*)hMy_string;
    char* pTemp;
    int i;
    if(pMy_string->size >= pMy_string->capacity){
      pTemp = (char*)malloc(sizeof(char) * 2 * pMy_string->capacity);
      if(pTemp == NULL){
	return FAILURE;
      }
      for(i = 0; i < pMy_string->size; i++){
	pTemp[i] = pMy_string->data[i];
      }
      free(pMy_string->data);
      pMy_string->data = pTemp;
      pMy_string->capacity *= 2;
    }
    pMy_string->data[pMy_string->size] = item;
    pMy_string->size++;
    return SUCCESS;
  }
  
Status my_string_pop_back(MY_STRING hMy_string){
  My_string *pMy_string = (My_string*)hMy_string;
  if(pMy_string->size <= 0){
    return FAILURE;
  }
  pMy_string->data[pMy_string->size-1] = '\0';
  pMy_string->size--;
  return SUCCESS;
}

char* my_string_at(MY_STRING hMy_string, int index){
  My_string *pMy_string = (My_string*)hMy_string;
  //printf("index: %d\n", index);
  if(index < 0 || index >= pMy_string->size){
    return NULL;
  }
  return &(pMy_string->data[index]); 
}

char* my_string_c_str(MY_STRING hMy_string){
  int i;
  My_string *pMy_string = (My_string*)hMy_string;
  if(pMy_string->size >= pMy_string->capacity){
    char* cString = (char*)malloc(sizeof(char)*(pMy_string->capacity) + 1);
    if(cString == NULL){
      return NULL;
    }
    for(i = 0; i < pMy_string->size; i++){
      cString[i] = pMy_string->data[i];
    }
    //cString[i+1] = '\0';
    free(pMy_string->data);
    pMy_string->data = cString;
    pMy_string->capacity += 1;
  }
  pMy_string->data[pMy_string->size] = '\0';
  return(my_string_at(hMy_string, 0));
}
Status my_string_concat(MY_STRING hResult, MY_STRING hAppend){
  My_string *pResult = (My_string*)hResult;
  My_string *pAppend = (My_string*)hAppend;
  int i;
  int totalSize = pResult->size+pAppend->size;

  pResult->data = (char*)realloc(pResult->data, sizeof(char) * totalSize);
  if(pResult->data == NULL){
    return FAILURE;
  }
  for(i = 0; i < pAppend->size; i++){
    my_string_push_back(hResult, pAppend->data[i]);
  }
  return SUCCESS;
}

Boolean my_string_empty(MY_STRING hMy_string){
  My_string *pMy_string = (My_string*)hMy_string;
  if(pMy_string->size <= 0){
    return TRUE;
  }
  return FALSE;
}

void my_string_assignment(Item* pLeft, Item Right){
  int i;
  My_string* pRight = (My_string*)Right;
  My_string* phLeft = (My_string*)*pLeft;
  char* temp = my_string_c_str(pRight);
  if(phLeft == NULL)
    phLeft = my_string_init_c_string(temp);
  else{
    phLeft->size = 0;
    for(i = 0; i < pRight->size + 1; i++){
      
      my_string_push_back(phLeft, pRight->data[i]);
    }
  }
  *pLeft = phLeft;
}

Boolean my_string_equals(MY_STRING hLeft, MY_STRING hRight){//returns true if 2 strings are equal
	My_string *pLeft = (My_string*)hLeft;
	My_string *pRight = (My_string*)hRight;
	int i;
	if(pLeft->size != pRight->size){
		return FALSE;
	}
	for(i = 0; i < pLeft->size; i++){
		
		if(*my_string_at(pLeft, i) != *my_string_at(pRight, i)){
			return FALSE;
		}
	}
	return TRUE;
}
