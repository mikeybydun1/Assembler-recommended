/* [mmn 14 : dataUtils.c]:
In this file: many usefull functions like sentenceLength() 
but more copmlicated then the functions in utilities.c

author: Mikey Bar Yaacov Dunevich | Date: 21.8.2022
ID: 215356981 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "utilities.h"
#include "dataUtils.h"

#define CHECH_STRING_SIZE 50
#define REGISTER_ADDRESSING_TYPE 3
#define NUMBER_AADDRESSING_TYPE 0


char checkString[CHECH_STRING_SIZE];
/* sentenceLength(): The function gets a pointer to a sentence
and return the length of the sentence in bytes.*/
int sentenceLength(char *s1) {
  int L = 1;
  
  if(strlen(s1) == 3) { // No arguments - return 1.
    return L;
  }
  else if(is_substring(",",s1) == false && is_substring(".string",s1) == false
      && is_substring(".data",s1) == false 
        && is_substring(".struct",s1) == false ) 
  { // 1 argument
    char *argument = getFirstArg(s1);
    L++;
    if(is_substring(".",argument) == true) {
      L++;
    }
    return L;
  }

  else if(is_substring(",",s1) == true && is_substring(".string",s1) == false
      && is_substring(".data",s1) == false 
        && is_substring(".struct",s1) == false) 
  { // 2 argument
    char *argument1 = getFirstArg(s1);
    char *argument2 = getSecondArg(s1);
    L = L + 2;

    if(is_substring(".",argument1) == true && is_substring(".",argument2) == true) {
      L += 2;
      return L;
    }
    else if(getAddressingType(argument1) == REGISTER_ADDRESSING_TYPE 
        && getAddressingType(argument2) == REGISTER_ADDRESSING_TYPE) {
      L--;
      return L;
    }
    else if(is_substring(".",argument1) == true || 
      is_substring(".",argument2) == true) {
        L++;
        return L;
      }
    else {
      return L;
    }
  }
  
  else if(is_substring(".string",s1) && is_substring(".data",s1) == false 
        && is_substring(".struct",s1) == false) 
  { // String label
          int count = 0;
          while(s1[count] != 'i' && s1[count+1] != 'n' && s1[count+2] != 'g') {
            count ++;
          }
          count+=4;
          int p = 0;
          for(int i = count+1; i < strlen(s1); i++) {
            checkString[p] = s1[count];
            p++;
          }
          L += strlen(checkString)-2;
          return L+1;
  }
  
  else if(is_substring(".string",s1) == false && is_substring(".data",s1) 
        && is_substring(".struct",s1) == false) 
  { // Data label
    for(int i = 0; i < strlen(s1); i++) {
      if(s1[i] == ',') {
        L++;
      }
    }
    return L;
  }

  else if(is_substring(".string",s1) == false && is_substring(".data",s1) == false
        && is_substring(".struct",s1))
  { // A struct label
    char s1_copy[1024];
    strcpy(s1_copy,s1);

    int count = 0;
    while(s1_copy[count] != ':') {
        s1_copy[count] = ' ';
        count++;
    }
    s1_copy[count] = ' ';
    removeSpaceFront(s1_copy);

    s1_copy[0] = ' '; s1_copy[1] = ' '; s1_copy[2] = ' '; s1_copy[3] = ' '; 
    s1_copy[4] = ' '; s1_copy[5] = ' '; s1_copy[6] = ' '; 
    removeSpaceFront(s1_copy);

    char dev[] = ",";
    char *pointer = strtok(s1_copy,dev);
    int pointer_num;

    while(pointer != NULL) {

        pointer_num = atoi(pointer);
        
        if(pointer_num == 0) {
            L += strlen(pointer)-1;
        }
        else {
            L++;
        }
        pointer = strtok(NULL,dev);
    }
    return L-1;
  }
}

/* argAmount(): The function gets a command name,
and return the arg amount of the command.*/
int argAmount(char s1[]) {
    int arguments = -1;

    if(strcmp(s1,"mov") == 0 || strcmp(s1,"cmp") == 0 || 
      strcmp(s1,"add") == 0 || strcmp(s1,"sub") == 0 
        || strcmp(s1,"lea") == 0) 
    {
      arguments = 2;
    }

    else if(strcmp(s1,"not") == 0 || strcmp(s1,"clr") == 0 || strcmp(s1,"inc") == 0
      || strcmp(s1,"dec") == 0 || strcmp(s1,"jmp") == 0 || strcmp(s1,"bne") == 0
      || strcmp(s1,"red") == 0 || strcmp(s1,"prn") == 0 || strcmp(s1,"jsr") == 0)
    {
      arguments = 1;
    }

    else if(strcmp(s1,"rts") == 0 || strcmp(s1,"stop") == 0)
    {
      arguments = 0;
    }
    
    return arguments; // Return the arg amount
}

/* getAddressingType(): The funtion gets a pointer to string and return
the addressing type of the string. register - 3, number - 0 and so on...*/
int getAddressingType(char *s1) {

    if(strcmp(s1,"r1") == 0 || strcmp(s1,"r2") == 0 || strcmp(s1,"r3") == 0 
      || strcmp(s1,"r4") == 0 || strcmp(s1,"r5") == 0 || strcmp(s1,"r6") == 0
        || strcmp(s1,"r0") == 0 || strcmp(s1,"r7") == 0)
    { // register type - 3
        return REGISTER_ADDRESSING_TYPE;
    }

    if(is_substring("#",s1) == true) 
    { // number type - 0
        return NUMBER_AADDRESSING_TYPE;
    }

    if(is_substring(".",s1) == true) 
    { // type - 1
        return 2;
    }

    else {
        return 1;
    }
}

/* isAddressingTypeOk(): The function gets a command name and pointer to a string
and return true if the addressing mode of the args are leagel, else - returns false. */
bool isAddressingTypeOk(char command[], char *sentence) {
    if(strcmp(command,"rts") == 0 || strcmp(command,"stop") == 0 || strcmp(command,"cmp") == 0 || strcmp(command,"prn") == 0) 
    { // The commands that the addressing type doesnt matter
        return true;
    }

    else if(strcmp(command,"mov") == 0 || strcmp(command,"add") == 0 || strcmp(command,"sub") == 0) 
    {   // If the addressing type of the second arg is 0 - return false, else - return true
        char *second_arg = getSecondArg(sentence);
        if(getAddressingType(second_arg) == NUMBER_AADDRESSING_TYPE) {
            return false;
        }
        else {
            return true;
        }
    }

    else if(strcmp(command,"not") == 0 || strcmp(command,"clr") == 0 || strcmp(command,"inc") == 0
        || strcmp(command,"dec") == 0 || strcmp(command,"jmp") == 0 || strcmp(command,"bne") == 0
          || strcmp(command,"red") == 0 || strcmp(command,"jsr") == 0 || strcmp(command,"sub") == 0)
    { 
        char *argument = getFirstArg(sentence);
        if(getAddressingType(argument) == 0) {
            return false;
        }
        else {
            return true;
        }
    }

    else if(strcmp(command,"lea") == 0) 
    {
        char *first_arg = getFirstArg(sentence);
        char *sec_arg = getSecondArg(sentence);

        if((getAddressingType(first_arg) == 1 || getAddressingType(first_arg) == 2)
           && getAddressingType(sec_arg) != 0) {
            return true;
        }
        else {
            return false;
        }
    }
}


