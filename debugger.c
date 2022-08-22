/* [mmn 14 : debugger.c]:
In this file: function called debug that checks the file and
print error like too much arguments, invalid addressing type...

author: Mikey Bar Yaacov Dunevich | Date: 21.8.2022
ID: 215356981 */

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#include "dataUtils.h"
#include "symbolTable.h"
#include "utilities.h"
#include "firstRide.h"
#include "preAssembler.h"
#include "debugger.h"

#define DEFULT_LINE_COUNTER 1
#define BUFFER_MAX_SIZE 2048

// Global var for the label name
char *label_name;

/* debug(): This function checks the file and
print error like too much arguments, invalid addressing type...*/
bool debug(char *file_name) {
    bool flag = true;
    FILE *fp = fopen(file_name, "r");
    if(fp == NULL) {
        printf("unrecognized file: %s\n",file_name);
        return false;
    }

    char buffer[BUFFER_MAX_SIZE];
    int line_counter = DEFULT_LINE_COUNTER;
    
    while(feof(fp) == false) {
        fgets(buffer, 1024, fp);
        removeSpaceFront(buffer);
        removeSpaceTail(buffer);

        // Get the label name and instore in 'label_name' pointer
        char *label_name = isLabel(buffer);

        if(is_substring(".entry",buffer) == false && is_substring(".extern",buffer) == false) // Not a entry or extern
        {
            // check if it is a data or string or struct without a label:
            if((is_substring(".data",buffer) == true || is_substring(".struct",buffer) == true
                 || is_substring(".string",buffer) == true) && strcmp(label_name,"!") == 0) 
            {
                flag = false;
                printf("Line %d:\t Data decleration error -> data/string/struct must have a label.\n\n",line_counter);
            }
            else if(is_substring(".data",buffer) == false && is_substring(".string",buffer) == false
                && is_substring(".struct",buffer) == false)
            { // it is a command (with or without a label befor it)

                if(strcmp(label_name,"!") != 0) { // If there is a label - remove it
                    int count  = 0;
                    while(buffer[count] != ':') {
                        buffer[count] = ' ';
                        count++;
                    }
                    buffer[count] = ' ';
                    removeSpaceFront(buffer);
                }

                // checking if the command is leagel:   
                char command_name[] = "aaa";
                command_name[0] = buffer[0];
                command_name[1] = buffer[1];
                command_name[2] = buffer[2];

                if(command_name[0] == 's' && command_name[1] == 't' && command_name[2] == 'o') {
                    command_name[3] = buffer[3];
                    command_name[4] = '\0';
                }      
                
                if(getCommandNumber(command_name) == 16) {
                    flag = false;
                    printf("Line %d:\tCommand type error %s -> not a recognized command.\n\n",line_counter,command_name);
                }

                else 
                { // leagel command

                    // checking arguments amount of the current command:
                    int arg_amount = amountInString(buffer,',') + 1;
                    
                    if(strcmp(command_name,"rts") == 0 || strcmp(command_name,"stop") == 0) {
                        arg_amount = 0;
                    }
                        

                    if(arg_amount != argAmount(command_name)) 
                    {
                        flag = false;
                        printf("Line %d:\t Arguments amount error -> the command '%s' needs %d args\n",line_counter,command_name,argAmount(command_name));
                        printf("         but you enter %d.\n\n",arg_amount);
                    }

                    // Check if the addressing mode is leagel:
                    bool isAddressingleagel = isAddressingTypeOk(command_name,buffer);
                    if(isAddressingleagel == false) {
                        flag = false;
                        printf("Line %d\t Addressing mode error in the arguments of the command '%s'.\n\n",line_counter,command_name);
                    }

                }
            }
            

        }
        
        else if(strcmp(label_name,"!") != 0) // A label
        { 
            if(getCommandNumber(label_name) != 16) {
                printf("Lybel error in line: %d -> The label name is a command name\n",line_counter);   
                flag = false; 
            }
        }
        line_counter++; // Update the line counter
    }
    
    fclose(fp); // Close the file pointer
    return flag; // Return the flag that says if there was errors in the file
}
