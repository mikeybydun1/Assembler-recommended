/* [mmn 14 : symbolTable.c]:
In this file: The symbol table for all the labels, and also some functions to handle with the symbolTable,
    like check if a label in the symbol table, check if a string is a label and more.

author: Mikey Bar Yaacov Dunevich | Date: 21.8.2022
ID: 215356981 */

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "utilities.h"
#include "symbolTable.h"
#include "dataUtils.h"

#define MAX_LABEL_CHARS 50
#define NOT_IN_LABEL_TABLE -1
#define LABEL_TYPE_DATA 0
#define LABEL_TYPE_EXTERN 2
#define LABEL_TYPE_ENTRY 3
#define LABEL_TYPE_CODE 1


/*The function isInLabelTable get a pointer to a char (string)
and returns true if the string is a name of a label with exists
int the label table. else - return false.*/
bool isInLabelTabel(char *s1) {
    bool flag = false;
    for(int i = 0; i < MAX_LABEL_AMOUNT; i++) {
        if(strcmp(s1,labelTable[i].name) == 0) {
            return true;
        }
    }
    return flag;
}

/*Gloval string to help functions returns strings*/ 
char labelsNames[MAX_LABEL_CHARS] = "";

/*The function inLabel gets a pointer to a char (or string)
and if the string isn't a label (there is not ':' in the string)
return '!' string. else - return the label's name.*/
char* isLabel(char *s1) {
    for(int i = 0; i < MAX_LABEL_CHARS; i++) {
        labelsNames[i] = ' ';
    }
    if(is_substring(":",s1) == true) 
    {
        int count = 0;
        while(s1[count] != ':') {
            count++;
        }
        for(int i = 0; i < count; i++) {
            labelsNames[i] = s1[i];
        }
        removeSpaceFront(labelsNames);
        removeSpaceTail(labelsNames);
        return labelsNames;
    }
    else {
        return "!";
    }
}

/*The function labelType() gets a pointer to a sentence (string), and
return the label type. Data - return 0, extern - return 2, entry - 3 and code - 1.*/
int labelType(char *s1) {
    if(is_substring(".data",s1) || is_substring(".struct",s1)
        || is_substring(".string",s1)) 
    {
        return LABEL_TYPE_DATA;
    }
    else if(is_substring(".extern",s1)) {
        return LABEL_TYPE_EXTERN;
    }
    else if(is_substring(".entry",s1))
    {
        return LABEL_TYPE_ENTRY;
    }
    else {
        return LABEL_TYPE_CODE;
    }
}

/*indexInLabelTable(): The function gets a pointer to a string and
return the index of this string in label table (if it is a real label name).
If the string isn't in the label table - return -1.*/
int indexInLabelTable(char *s) {
    int index = NOT_IN_LABEL_TABLE;
    for(int i = 0; i < MAX_LABEL_AMOUNT; i++) {
        if(strcmp(s,labelTable[i].name) == 0) {
            return i;
        }
    }
    return index;
}

