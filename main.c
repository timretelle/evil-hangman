#include <stdio.h>
#include <stdlib.h>
#include "my_string.h"
#include "generic_vector.h"
#include "associative.h"

//Precondition:current_word_family, new_key and word are all handles to valid
// MY_STRING opaque objects. guess is an alphabetical character that can be either
// upper or lower case.
//Postcondition: Returns SUCCESS after replacing the string in new_key with the key
// value formed by considering the current word family, the word and the guess.
// Returns failure in the case of a resizing problem with the new_key string.
Status get_word_key_value(MY_STRING current_word_family, MY_STRING new_key, MY_STRING word, char guess, int* wrongGuess);

Boolean complete_word(MY_STRING word);


  
int main(int argc, char* argv[])
{
  ASSOCIATIVE_ARRAY array = associative_array_init_default();
  GENERIC_VECTOR useful_vector;
  GENERIC_VECTOR keys;
  GENERIC_VECTOR *vector = (GENERIC_VECTOR*)malloc(sizeof(GENERIC_VECTOR)*30);
  FILE *fP;
  fP = fopen("dictionary.txt", "r");
  MY_STRING hString = my_string_init_default();
  MY_STRING new_key;
  int i;
  int currentMax = 0;
  int maxIndex = 0;
  int currentSize;
  char continuePlaying;
  char seeWords;
  int length;
  int guesses;
  int wrongGuesses = 0;
  MY_STRING current;
  MY_STRING word;
  MY_STRING word2;
  char guess; 
  for(i = 0; i < 30; i++){
    vector[i] = generic_vector_init_default(my_string_assignment, my_string_destroy);
  }
  
  while(my_string_extraction(hString, fP) != FAILURE){
    if(my_string_get_size(hString) <= 30){
      generic_vector_push_back(vector[my_string_get_size(hString)],(ITEM)hString);
    }
  }

  for(i = 0; i < 30; i++){
    printf("word size is %d with %d words.\n", i, generic_vector_get_size(vector[i]));
  }

  do{
	current = my_string_init_default();
	word = my_string_init_c_string("");
	useful_vector = generic_vector_init_default(my_string_assignment, my_string_destroy);
	do{
		printf("Do you want to see the word lists as you play? (y/n): ");
		scanf("%c", &seeWords);
	}while(seeWords != 'y' && seeWords != 'n');//continue reprompting until they give a valid answer
	printf("How many guesses do you want?: ");
    scanf("%d", &guesses);
	do{
		printf("What length word do you want to play with?: ");
		scanf("%d", &length);
	}while(!((length >= 2 && length <= 22) || length == 24 || length == 28 || length == 29));//reprompt if the specified length isn't one of the ones that has words
    current = my_string_init_default();
    for(i = 0; i < length; i++){
      my_string_push_back(current, '-');//initialize current to length amount of dashes
    }
	for(i = 0; i < generic_vector_get_size(vector[length]); i++){//vector of every word that has the correct length
			generic_vector_push_back(useful_vector, *generic_vector_at(vector[length], i));
	}
	//printf("useful: %s\n", my_string_c_str(*generic_vector_at(useful_vector, i)));
	while ((getchar()) != '\n'); 
	while(!complete_word(current) && wrongGuesses < guesses){//while they haven't finished the word and haven't run out of guesses...
		keys = generic_vector_init_default(my_string_assignment, my_string_destroy);
		printf("%s\n", my_string_c_str(current));
		do{
			printf("What is your guess?: ");
			scanf("%c", &guess);
		}while(!(guess >= 'a' && guess <= 'z'));//reprompt until they guess something that's actually a letter
		//new_key = my_string_init_c_string(my_string_c_str(current));//make sure new key is equal to the current family before we change it, maybe dumb
		for(i = 0; i < generic_vector_get_size(useful_vector); i++){
			new_key = my_string_init_c_string(my_string_c_str(current));
			//printf("new key: %s\n", my_string_c_str(new_key));
			//printf("current: %s\n", my_string_c_str(current));
			get_word_key_value(current, new_key, *generic_vector_at(useful_vector, i), guess, &wrongGuesses);//this line doesn't work properly: all the keys are always retrieved as just dashes
			generic_vector_push_back(keys, new_key);//add the key to the vector of stored keys, used later for checking the lengths of each key's vector
			//printf("%s\n", my_string_c_str(new_key));
			//printf("useful: %s\n", my_string_c_str(*generic_vector_at(useful_vector, i)));
			add_element(array, *generic_vector_at(useful_vector, i), new_key);
			my_string_destroy(&new_key);
		}
		for(i = 0; i < generic_vector_get_size(keys); i++){
			currentSize = generic_vector_get_size(find_by_key(array, *generic_vector_at(keys, i)));//segmentation fault here, I think if I fix these two errors then it'll all work fine
			if(currentSize > currentMax){
				currentMax = currentSize;
				maxIndex = i;
			}
			if(seeWords == 'y'){//print each key and its size if the user wanted us to
				printf("%s %d\n", my_string_c_str(*generic_vector_at(keys, i)), currentSize);
			}
		}
		new_key = *generic_vector_at(keys, maxIndex);
		printf("%d guesses remaining.\n", (guesses - wrongGuesses));
		current = new_key;//might be unnecessary 2
		while(getchar()!='\n');
		generic_vector_destroy(&keys);
	}
	length = 0;//reset values
	guesses = 0;
	wrongGuesses = 0;
	currentMax = 0;
	maxIndex = 0;
	if(complete_word(current)){
		printf("%s\n", my_string_c_str(current));
		printf("You win!\n");
	}
	else{
		printf("Sorry, you're out of guesses. The word was %s\n", my_string_c_str(word));
	}
    printf("Play again? (y/n): ");
    scanf("%c", &continuePlaying);
	my_string_destroy(&current);//destroy this stuff inside the loop because it's going to be reinitialized if they want to play again
	generic_vector_destroy(&useful_vector);
	//my_string_destroy(&new_key);
  }while(continuePlaying == 'y');
  my_string_destroy(&hString);
  for(i = 0; i < 30; i++){//destroy all the vectors of words
	  generic_vector_destroy(&(vector[i]));
  }
  associative_array_destroy(&array);
  free(vector);
  fclose(fP);
  return 0;
} 

Status get_word_key_value(MY_STRING current_word_family, MY_STRING new_key, MY_STRING word, char guess, int* wrongGuess){
  int i;
  Boolean wrong = TRUE;
  if(current_word_family == NULL || new_key == NULL || word == NULL){
    return FAILURE;
  }
  for(i = 0; i < my_string_get_size(current_word_family); i++){
    if(*(my_string_at(word, i)) == guess){
      *(my_string_at(new_key, i)) = guess;
	  wrong = FALSE;
    }
  }
  if(wrong){//if the guess didn't appear, increment the wrongGuesses variable
	  (*wrongGuess)++;
  }
  return SUCCESS;
}

Boolean complete_word(MY_STRING word){//this function returns true if there are no dashes in the string, used to detect when the user has guessed the whole word
	int i;
	for(i = 0; i < my_string_get_size(word); i++){
		if(*my_string_at(word, i) == '-')
			return FALSE;
	}
	return TRUE;
}

