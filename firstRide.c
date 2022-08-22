/* [mmn 14 : firstRide.c]:
In this file: Very importent function - ride1(). Going over the file,
    and add every label to the label table with the right values.llll

author: Mikey Bar Yaacov Dunevich | Date: 21.8.2022
ID: 215356981 */

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "symbolTable.h"
#include "utilities.h"
#include "firstRide.h"
#include "preAssembler.h"
#include "dataUtils.h"

#define DEFULT_COUNTER_VALUE 0
#define EXTERN_TYPE_VALUE 2
#define DEFULT_LABEL_VALUE 0
#define DATA_LABEL 0
#define CODE_LABEL 1
#define MAX_LABEL_AMOUNT 100

int IC = DEFULT_COUNTER_VALUE;
int DC = DEFULT_COUNTER_VALUE;

void ride1(char *fileName) {
    FILE *fp = fopen(fileName, "r");

    if(fp == NULL) {
        printf("unrecognized file: %s\n",fileName);
        return;
    }
    
    char buffer[2048];
    int labelCounter = DEFULT_COUNTER_VALUE;
    int line_counter = DEFULT_COUNTER_VALUE;
    char *label_name;

    while(feof(fp) == false) {
        fgets(buffer, 1024, fp);

        if(is_substring(".entry",buffer) == true) {
            continue;
        }

        // Clean the sentence from blank spaces (front and tail)
        removeSpaceFront(buffer);
        removeSpaceTail(buffer);

        int L = sentenceLength(buffer); // The length in bytes of buffer
        label_name = isLabel(buffer); 

        // If the current line is a extern decleration - add the label as an extern label
        if(is_substring(".extern",buffer) == true)
        {
            // Remove the '.extern'
            buffer[0] = ' '; buffer[1] = ' '; buffer[2] = ' '; buffer[3] = ' '; 
            buffer[4] = ' '; buffer[5] = ' '; buffer[6] = ' '; 
            removeSpaceFront(buffer); 

            // Create the label as a struct label
            struct label ext_labe;
            ext_labe.type = EXTERN_TYPE_VALUE;
            ext_labe.value = DEFULT_LABEL_VALUE;
            strcpy(ext_labe.name,buffer);

            // Add the new label to the label table
            labelTable[labelCounter] = ext_labe;
            labelCounter += 1;
            continue;           
        }

        // No label - only update IC
        else if(label_name[0] == '!') { 
            IC += L;
        }

        // The current line is a label sentence (not extern)
        else if(label_name[0] != '!' && is_substring(".extern",buffer) == false)
        {   
            // Check if this sentence is an exisiting label. If it is - print error message
            if(isInLabelTabel(label_name) == true) {
                if(line_counter != amountLinesInFile(fileName)) {
                    printf("Line %d:\tLabel error -> the Label '%s' already exists\n\n",line_counter,label_name);
                }
                continue;
            }
            else 
            { // ----------- NEW LABEL -----------
                int label_type = labelType(buffer);

                if(label_type == DATA_LABEL) 
                { // data label
                    struct label data_label;
                    char *label_name1 = isLabel(buffer);

                    strcpy(data_label.name,label_name1);
                    data_label.type = 0;
                    data_label.value = DC;

                    DC = DC + L;
                    labelTable[labelCounter] = data_label;
                }
                else if(label_type == CODE_LABEL) 
                { // code label
                    struct label code_label;
                    char *label_name1 = isLabel(buffer);

                    strcpy(code_label.name,label_name1);
                    code_label.type = 1;
                    code_label.value = IC;

                    IC += L; // update the IC
                    labelTable[labelCounter] = code_label;
                }
                labelCounter++; // Update the label counter
            }
        }
        line_counter++; // Updata the line counter
    }

    /* Update all data labels values with adding the last value of IC */
    for(int i = 0; i < MAX_LABEL_AMOUNT; i++) {
        if(labelTable[i].type == DATA_LABEL) {
            labelTable[i].value += IC;
        }
    }
    

    fclose(fp); // Close the file pointer

    
} // end of function

/* amountLinesInFile(): This function gets a pointer to a file name,
and return the amount of lines in the file with the given.*/
int amountLinesInFile(char *file_name) {
    FILE *fp = fopen(file_name,"r");
    int line_counter = 0;
    char buffer[2048];

    while(feof(fp) == false) {
        line_counter++;
        fgets(buffer, 1024, fp);
    }
    fclose(fp);
    return line_counter-1;
}