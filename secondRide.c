/* [mmn 14 : secondRide.c]:
In this file: The most importent (and longest) file in the project.
There are 2 main importent functions

1) TranslateLineToBin(): gets a pointer to a line and translate him to bin code
2) TranslateFileToBinary(): gets a pointer to a .am file and translate him to binary code

author: Mikey Bar Yaacov Dunevich | Date: 21.8.2022
ID: 215356981 */

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdbool.h>

#include "utilRide2.h"
#include "utilities.h"
#include "symbolTable.h"
#include "firstRide.h"
#include "preAssembler.h"
#include "secondRide.h"
#include "debugger.h"
#include "dataUtils.h"
#include "outputFiles.h"

#define COUNTERS_DEFULT_VALUE 0
#define FIRST_LINE_VALUE 1
#define MAX_BYTES 2048
#define START_CODE_PLACE 100
#define DECIMAL_BASE 10
#define BYTE_FULL_SIZE 10
#define MAX_LABEL_AMOUNT 100
#define BYTE_LABEL_WITHOUT_ARE 8
#define MAX_BUFFER_SIZE 2048
#define ENTRY_LABEL_TYPE 3
#define LABEL_EXTERN_TYPE 2

int ic1 = COUNTERS_DEFULT_VALUE;
int dc1 = COUNTERS_DEFULT_VALUE;
int line_counter = FIRST_LINE_VALUE;
bool is_errors = false; // Flag for check if there is errors in the line


void reasetArr() {
    for(int i = 0; i < MAX_BYTES; i++) {
        strcpy(code_arr[i],"0000000000");
        strcpy(data_arr[i],"0000000000");
    }
}

/* translateLineToBinary(): This function gets a pointer to a line and
translate code the line to 10 binary chars. 
The function knows how to translate commands, .data, .string and .struct */
void translateLineToBinary(char *line) {

    //int length = sentenceLength(line); // The length in bytes of the sentence
    char *label_name = isLabel(line); // The label name of the sentence (if there is a label)

    if(is_substring(".entry",line) == true || is_substring(".extern",line) == true) {
        return;
    }

    if(label_name[0] == '!' && is_substring(".entry",line) == false && is_substring(".extern",line) == false)
    { // There is no label - the line is a command:

        char command_name[4];
        strcpy(command_name,getCommand(line)); // Get the name of the command
        
        // Translate the command name to binary code and push to the right place
        int command_number = getCommandNumber(command_name);
        int command_binary = decimalToBinary(command_number);

        char command_str[BYTE_FULL_SIZE] = {0};
        my_itoa(command_binary,command_str);

        if(strlen(command_str) == 4) {
            code_arr[ic1][0] = command_str[0];
            code_arr[ic1][1] = command_str[1];
            code_arr[ic1][2] = command_str[2];
            code_arr[ic1][3] = command_str[3];
        }
        else if(strlen(command_str) == 3) {
            code_arr[ic1][1] = command_str[0];
            code_arr[ic1][2] = command_str[1];
            code_arr[ic1][3] = command_str[2];
        }
        else if(strlen(command_str) == 2) {
            code_arr[ic1][2] = command_str[0];
            code_arr[ic1][3] = command_str[1];
        }
        else if(strlen(command_str) == 1) {
            code_arr[ic1][3] = command_str[0];
        }

        

        if(is_substring(",",line) == false) 
        { 
            // ------------ THERE IS ONLY ONE ARGUMENT ------------

            char *first_arg = getOnlyArg(line); // get the argument
            int addressing_type = getAddressingType(first_arg);
            
            if(strcmp("stop",line) == 0) { // if the command is 'stop' there is no args
                first_arg = "";
                addressing_type = -1;
                ic1--;
            }

            if(addressing_type == 3) { // the only argument is a register
                code_arr[ic1][6] = '1';
                code_arr[ic1][7] = '1';

                char reg_bin[4];
                strcpy(reg_bin,getRegisterBainry(first_arg));

                // Coding the register number in binary to the next byte
                code_arr[ic1+1][0] = reg_bin[0];
                code_arr[ic1+1][1] = reg_bin[1];
                code_arr[ic1+1][2] = reg_bin[2];
                code_arr[ic1+1][3] = reg_bin[3];


            }
            else if(isInLabelTabel(first_arg) == true && addressing_type != -1) 
            { // The argument is an exisiting label
                code_arr[ic1][7] = '1';

                // Enter to the next byte the address of the label:
                int label_index = indexInLabelTable(first_arg);
                int label_value = labelTable[label_index].value;

                if(labelTable[label_index].type != 2 && labelTable[label_index].type != -1)
                    label_value += START_CODE_PLACE;

                int label_value_bin = decimalToBinary(label_value);

                char label_bin_str[] = {0};
                my_itoa(label_value_bin, label_bin_str);

                int bin_counter = BYTE_LABEL_WITHOUT_ARE - strlen(label_bin_str);
                for(int i = 0; i < strlen(label_bin_str); i++) {
                    code_arr[ic1+1][bin_counter] = label_bin_str[i];
                    bin_counter++;

                }

                if(labelTable[label_index].type == 2 || labelTable[label_index].type == -1) 
                    code_arr[ic1+1][9] = '1';
                else
                    code_arr[ic1+1][8] = '1';
            }

            else if(is_substring(".",first_arg) == true) 
            { // The line is LABEL.OFFSET sentence
                code_arr[ic1][6] = '1';

                // Clean the sentence from blank spaces (front and back)
                removeSpaceFront(first_arg);
                removeSpaceTail(first_arg);

                // Get the offest number -> translate to bin -> push to the rigth byte
                int number = atoi(&first_arg[strlen(first_arg)-1]);
                int bin_number = decimalToBinary(number);
                char bin_str[BYTE_FULL_SIZE] = {0};

                my_itoa(bin_number,bin_str);

                int bin_counter = BYTE_LABEL_WITHOUT_ARE - strlen(bin_str);
                for(int i = 0; i < strlen(bin_str); i++) {
                    code_arr[ic1+2][bin_counter] = bin_str[i];
                    bin_counter++;
                }

                // Get the label name and the value and translate to bin and push to the right byte
                char dev[] = ".";
                char *label_name = strtok(first_arg,dev);
                
                if(isInLabelTabel(label_name) == false) { // Print error messange
                    printf("Line: %d\tLabel error -> '%s' doesn't exist\n\n",line_counter,label_name);
                    is_errors = true;
                }
                else {
                    int label_index = indexInLabelTable(first_arg); // The label index
                    int label_value = START_CODE_PLACE + labelTable[label_index].value; // The label value

                    int label_value_bin = decimalToBinary(label_value); // label value in base 2

                    char label_bin_str[] = {0};
                    my_itoa(label_value_bin, label_bin_str);

                    // Copying the label value in binary to the right byte
                    int bin_counter1 = BYTE_LABEL_WITHOUT_ARE - strlen(label_bin_str);
                    for(int i = 0; i < strlen(label_bin_str); i++) {
                        code_arr[ic1+1][bin_counter1] = label_bin_str[i];
                        bin_counter1++;

                    }
                }

                code_arr[ic1+1][8] = '1'; 
             
            }
            else if(addressing_type == 0) 
            { // The argument is a number
                
                if(is_substring("-",first_arg) == true) {
                    removeSpaceFront(first_arg);
                    removeSpaceTail(first_arg);
                    negativNumberToBin(first_arg,false,ic1+1);
                }

                else {
                    first_arg[0] = ' ';
                    removeSpaceFront(first_arg);

                    // Get the register number and code him in binary
                    int bin_num = decimalToBinary(atoi(first_arg));
                    char bin_str[] = {0};
                    my_itoa(bin_num,bin_str);

                    int bin_counter = BYTE_LABEL_WITHOUT_ARE - strlen(bin_str);
                    for(int i = 0; i < strlen(bin_str); i++) {
                        code_arr[ic1+1][bin_counter] = bin_str[i];
                        bin_counter++;
                    }
                }


                
            }
            else {
                if(first_arg != "") // check if it not a stop command
                {
                    printf("Line %d\tError: unrecognized argument '%s'\n\n",line_counter,first_arg);
                    is_errors = true;
                }    
            }

        }

        else
        { // ---------- THERE ARE 2 ARGUMENTS ----------

            char *first_arg = getFirstArg(line); // Get the first arg
            char *second_arg = getSecondArg(line); // Get the second arg
            bool isFirstArgHasPoint = false; // flag for checking if the first arg is LABEL.OFFSET

            int addressing_type_first = getAddressingType(first_arg);
            int addressing_type_second = getAddressingType(second_arg);

            if(addressing_type_first == 3 && addressing_type_second == 3) {
                code_arr[ic1][4] = '1';
                code_arr[ic1][5] = '1';

                char reg_bin[4];
                strcpy(reg_bin,getRegisterBainry(first_arg));

                // Coding the register number in binary to the next byte
                code_arr[ic1+1][0] = reg_bin[0];
                code_arr[ic1+1][1] = reg_bin[1];
                code_arr[ic1+1][2] = reg_bin[2];
                code_arr[ic1+1][3] = reg_bin[3];

                code_arr[ic1][6] = '1';
                code_arr[ic1][7] = '1';

                char reg_bin1[4];
                strcpy(reg_bin1,getRegisterBainry(second_arg));

                code_arr[ic1+1][4] = reg_bin1[0];
                code_arr[ic1+1][5] = reg_bin1[1];
                code_arr[ic1+1][6] = reg_bin1[2];
                code_arr[ic1+1][7] = reg_bin1[3];
                return;
            }

            // ----- Coding the first argument ------

            if(addressing_type_first == 3) { // register
                code_arr[ic1][4] = '1';
                code_arr[ic1][5] = '1';

                char reg_bin[4];
                strcpy(reg_bin,getRegisterBainry(first_arg));

                // Coding the register number in binary to the next byte
                code_arr[ic1+1][0] = reg_bin[0];
                code_arr[ic1+1][1] = reg_bin[1];
                code_arr[ic1+1][2] = reg_bin[2];
                code_arr[ic1+1][3] = reg_bin[3];

            }
            else if(addressing_type_first == 0) { // The first arg is a number
                
                if(is_substring("-",first_arg) == true) {
                    removeSpaceFront(first_arg);
                    removeSpaceTail(first_arg);

                    negativNumberToBin(first_arg,false,ic1+1);
                }

                else {
                    // Get the register number and code him in binary in the right byte
                    first_arg[0] = ' ';
                    removeSpaceFront(first_arg);

                    int bin_num = decimalToBinary(atoi(first_arg));
                    char bin_str[] = {0};
                    my_itoa(bin_num,bin_str);

                    int bin_counter = BYTE_LABEL_WITHOUT_ARE - strlen(bin_str);
                    for(int i = 0; i < strlen(bin_str); i++) {
                        code_arr[ic1+1][bin_counter] = bin_str[i];
                        bin_counter++;
                    }
                }

                

            }
            else if(isInLabelTabel(first_arg) == true)
            { // exisitng label
                code_arr[ic1][5] = '1';

                // Enter to the next byte the address of the label:
                int label_index1 = indexInLabelTable(first_arg);
                int label_value1 = labelTable[label_index1].value;

                if(labelTable[label_index1].type != 2)
                    label_value1 += START_CODE_PLACE;

                int label_value_bin1 = decimalToBinary(label_value1);

                char label_bin_str1[] = {0};
                my_itoa(label_value_bin1, label_bin_str1);

                int bin_counter1 = BYTE_LABEL_WITHOUT_ARE - strlen(label_bin_str1);
                for(int i = 0; i < strlen(label_bin_str1); i++) {
                    code_arr[ic1+1][bin_counter1] = label_bin_str1[i];
                    bin_counter1++;
                }

                if(labelTable[label_index1].type == 2) // External label
                    code_arr[ic1+1][9] = '1';
                else
                    code_arr[ic1+1][8] = '1';

            } 
            else if(is_substring(".",first_arg) == true)
            { // The first argument is LABEL.OFFSET sentence
                isFirstArgHasPoint = true;
                code_arr[ic1][4] = '1';

                // Clean from blank spaces (front and back)
                removeSpaceFront(first_arg);
                removeSpaceTail(first_arg);

                // Get the offset number and the label name and code them in binary

                int number = atoi(&first_arg[strlen(first_arg)-1]);
                int bin_number = decimalToBinary(number);
                char bin_str[BYTE_FULL_SIZE] = {0};

                my_itoa(bin_number,bin_str);

                int bin_counter = BYTE_LABEL_WITHOUT_ARE - strlen(bin_str);
                for(int i = 0; i < strlen(bin_str); i++) {
                    code_arr[ic1+2][bin_counter] = bin_str[i];
                    bin_counter++;
                }

                char dev[] = ".";
                char *label_name = strtok(first_arg,dev);
                
                if(isInLabelTabel(label_name) == false) {
                    printf("Line: %d\tLabel error -> '%s' doesn't exist\n\n",line_counter,label_name);
                    is_errors = true;
                }
                else {
                    int label_index = indexInLabelTable(first_arg);
                    int label_value = START_CODE_PLACE + labelTable[label_index].value;

                    int label_value_bin = decimalToBinary(label_value);

                    char label_bin_str[] = {0};
                    my_itoa(label_value_bin, label_bin_str);

                    int bin_counter1 = BYTE_LABEL_WITHOUT_ARE - strlen(label_bin_str);
                    for(int i = 0; i < strlen(label_bin_str); i++) {
                        code_arr[ic1+1][bin_counter1] = label_bin_str[i];
                        bin_counter1++;

                    }
                    code_arr[ic1+1][8] = '1';
                }

            }
                
            else { // Print error massege
                printf("Line %d\tError: unrecognized argument\n\n",line_counter);
                is_errors = true;
            }

            // ---------- Coding the second argument ----------
            
            if(addressing_type_second == 3) 
            { // The second arg is a register
                code_arr[ic1][6] = '1';
                code_arr[ic1][7] = '1';

                char reg_bin1[4];
                strcpy(reg_bin1,getRegisterBainry(second_arg));

                if(isFirstArgHasPoint == true) {
                    code_arr[ic1+3][0] = reg_bin1[0];
                    code_arr[ic1+3][1] = reg_bin1[1];
                    code_arr[ic1+3][2] = reg_bin1[2];
                    code_arr[ic1+3][3] = reg_bin1[3];
                }

                else {
                    code_arr[ic1+2][0] = reg_bin1[0];
                    code_arr[ic1+2][1] = reg_bin1[1];
                    code_arr[ic1+2][2] = reg_bin1[2];
                    code_arr[ic1+2][3] = reg_bin1[3];
                }
            }
            else if(addressing_type_second == 0) 
            { // The second arg is a number
                
                int diff = 2;
                if(isFirstArgHasPoint == true) {diff = 3;}

                if(is_substring("-",second_arg) == true) {
                    
                    removeSpaceFront(second_arg);
                    removeSpaceTail(second_arg);
                    
                    
                    negativNumberToBin(second_arg,false,ic1+diff);
                }

                else {
                    second_arg[0] = ' ';
                    removeSpaceFront(second_arg);

                    int bin_num = decimalToBinary(atoi(second_arg));
                    char bin_str[] = {0};
                    my_itoa(bin_num,bin_str);

                    int bin_counter = BYTE_LABEL_WITHOUT_ARE - strlen(bin_str);
                    for(int i = 0; i < strlen(bin_str); i++) {
                        code_arr[ic1+diff][bin_counter] = bin_str[i];
                        bin_counter++;
                    }
                }
                
            }
            else if(isInLabelTabel(second_arg) == true)
            { // The second arg is existing label
                code_arr[ic1][7] = '1';

                // Enter to the next byte the address of the label:
                int label_index2 = indexInLabelTable(second_arg);
                int label_value2 = labelTable[label_index2].value;

                if(labelTable[label_index2].type != 2) 
                    label_value2 += START_CODE_PLACE;
                    

                int label_value_bin2 = decimalToBinary(label_value2);

                char label_bin_str2[] = {0};
                my_itoa(label_value_bin2, label_bin_str2);

                int diff = 2;
                if(isFirstArgHasPoint == true) {diff = 3;}

                int bin_counter2 = BYTE_LABEL_WITHOUT_ARE - strlen(label_bin_str2);
                for(int i = 0; i < strlen(label_bin_str2); i++) {
                    code_arr[ic1+diff][bin_counter2] = label_bin_str2[i];
                    bin_counter2++;
                }

                if(labelTable[label_index2].type == 2) 
                    code_arr[ic1+diff][9] = '1';
                else
                    code_arr[ic1+diff][8] = '1';
            } 

            else if(is_substring(".",second_arg) == true)
            { // The second arg is LENGTH.OFFSET
                code_arr[ic1][6] = '1';

                // Clean the second arg from blank spaces (front and back)
                removeSpaceFront(second_arg);
                removeSpaceTail(second_arg);

                // Get the offset number and the label name and code them in binary
                // In the right byte in code_arr

                int number1 = atoi(&second_arg[strlen(second_arg)-1]);
                int bin_number1 = decimalToBinary(number1);
                char bin_str[BYTE_FULL_SIZE] = {0};

                my_itoa(bin_number1,bin_str);
                int diff = 2;
                if(isFirstArgHasPoint == true) {diff = 3;}

                int bin_counter = BYTE_LABEL_WITHOUT_ARE - strlen(bin_str);
                for(int i = 0; i < strlen(bin_str); i++) {
                    code_arr[ic1+diff+1][bin_counter] = bin_str[i];
                    bin_counter++;
                }

                char dev[] = ".";
                char *label_name = strtok(second_arg,dev);
                
                if(isInLabelTabel(label_name) == false) {
                    printf("Line: %d\tLabel error -> '%s' doesn't exist\n\n",line_counter,label_name);
                }
                else {
                    int label_index = indexInLabelTable(second_arg);
                    int label_value = START_CODE_PLACE + labelTable[label_index].value;

                    int label_value_bin = decimalToBinary(label_value);

                    char label_bin_str[] = {0};
                    my_itoa(label_value_bin, label_bin_str);

                    int bin_counter1 = BYTE_LABEL_WITHOUT_ARE - strlen(label_bin_str);
                    for(int i = 0; i < strlen(label_bin_str); i++) {
                        code_arr[ic1+diff][bin_counter1] = label_bin_str[i];
                        bin_counter1++;

                    }
                    code_arr[ic1+diff][8] = '1';
                }

            }
                
            else { // Print error massege 
                printf("Line %d\tError: unrecognized argument\n\n",line_counter);
                is_errors = true;
            }
        }   
    }

    else if(is_substring(".data",line) == true)
    { // ------------ Label Type: Data ------------
        
        char line_copy[MAX_BUFFER_SIZE];
        strcpy(line_copy,line);
        int count = COUNTERS_DEFULT_VALUE;

        // Remove the label:
        while(line_copy[count] != ':') {
            line_copy[count] = ' ';
            count++;
        }
        line_copy[count] = ' ';
        removeSpaceFront(line_copy);

        // Go over every number using strtok() function and translate the number to binary
        // And code him in right byte in the data_arr

        char dev[] = ",";
        char *data_pointer = strtok(line_copy,dev);

        char *data_pointer1 = strtok(NULL,dev);
        int bin_num1;
        char bin_str1[BYTE_FULL_SIZE] = {0};
        int bin_counter1;
        int diff_counter = 1;

        while (data_pointer1 != NULL) {
            removeSpaceFront(data_pointer1);
            removeSpaceTail(data_pointer1);

            if(is_substring("-",data_pointer1) == true) {
                negativNumberToBin(data_pointer1,true,dc1+diff_counter);
            }
            else {
                bin_num1 = decimalToBinary(atoi(data_pointer1));
                my_itoa(bin_num1,bin_str1);

                bin_counter1 = BYTE_FULL_SIZE - strlen(bin_str1);
                for(int i = 0; i < strlen(bin_str1); i++) {
                    data_arr[dc1+diff_counter][bin_counter1] = bin_str1[i];
                    bin_counter1++; // Update the bin counter
                }
            }
            
            data_pointer1 = strtok(NULL,dev);
            diff_counter++; // update diff_counter
        }

        // Delete the '.data'
        data_pointer[0] = ' '; data_pointer[1] = ' '; data_pointer[2] = ' '; 
        data_pointer[3] = ' '; data_pointer[4] = ' '; 

        removeSpaceFront(data_pointer);
        removeSpaceTail(data_pointer);
        
        if(is_substring("-",data_pointer) == true) {
            negativNumberToBin(data_pointer,true,dc1);
        }

        else {
            int bin_number = decimalToBinary(atoi(data_pointer));
            char bin_str[] = {0};
            my_itoa(bin_number,bin_str);
            
            int bin_counter = BYTE_FULL_SIZE - strlen(bin_str);
            for(int i = 0; i < strlen(bin_str); i++) {
                data_arr[dc1][bin_counter] = bin_str[i];
                bin_counter++;
            }
        }
        

    }

    else if(is_substring(".string",line) == true)
    { // ---------- Label Type: String ----------
        
        char line_copy[MAX_BUFFER_SIZE];
        strcpy(line_copy,line);
        int count = COUNTERS_DEFULT_VALUE;

        // Delete the label name from line_copy:
        while(line_copy[count] != ':') {
            line_copy[count] = ' ';
            count++;
        }
        line_copy[count] = ' ';
        removeSpaceFront(line_copy);

        // Remove the '.string' from line_copy
        line_copy[0] = ' '; line_copy[1] = ' '; line_copy[2] = ' ';
        line_copy[3] = ' '; line_copy[4] = ' '; line_copy[5] = ' ';
        line_copy[6] = ' '; 
        removeSpaceFront(line_copy);
   
        line_copy[0] = ' '; line_copy[strlen(line_copy)-1] = ' ';
        removeSpaceFront(line_copy);
        removeSpaceTail(line_copy);

        int ascii_bin;
        char bin_str[BYTE_FULL_SIZE] = {0};
        int bin_counter;
        char ch;
        int diff_counter = COUNTERS_DEFULT_VALUE;

        // Going over the chars in line_copy, translate the chars to ascii using asciiToBin(),
        // and code then in right byte in code_arr

        for(int i = 0; i < strlen(line_copy); i++) {
            ch = line_copy[i]; // The current char
            ascii_bin = asciiToBinNumber(ch); // Translate to ascii bin number
            my_itoa(ascii_bin,bin_str);
            
            bin_counter = BYTE_FULL_SIZE - strlen(bin_str);

            for(int j = 0; j < strlen(bin_str); j++) {
                data_arr[dc1 + diff_counter][bin_counter] = bin_str[j];
                bin_counter++; // update the bin_counter
            }
            diff_counter++; // update the diff counter
            
        }


    }

    else if(is_substring(".struct",line) == true) 
    { // ---------- Label Type: Struct ----------
        
        char line_copy[MAX_BUFFER_SIZE];
        strcpy(line_copy,line);
        int count = COUNTERS_DEFULT_VALUE;

        // Remove the label name
        while(line_copy[count] != ':') {
            line_copy[count] = ' ';
            count++;
        }
        line_copy[count] = ' ';
        removeSpaceFront(line_copy);

        // Remove the '.struct'
        line_copy[0] = ' '; line_copy[1] = ' '; line_copy[2] = ' ';
        line_copy[3] = ' '; line_copy[4] = ' '; line_copy[5] = ' ';
        line_copy[6] = ' '; 
        removeSpaceFront(line_copy);

        char dev[] = ",";
        char *pointer = strtok(line_copy,dev);

        int bin_number;
        int diff = 0;
        char bin_str[BYTE_FULL_SIZE] = {0};
        int bin_counter;
        
        
        while(pointer != NULL) {

            // Remove blank spaces (front and back)
            removeSpaceFront(pointer);
            removeSpaceTail(pointer);
            
            int pointer_num = atoi(pointer);
            if(pointer_num == 0 && is_substring("-",pointer) == false) 
            { // not a number
                pointer[0] = ' '; pointer[strlen(pointer)-1] = ' ';
                removeSpaceFront(pointer);
                removeSpaceTail(pointer);

                int ascii_bin;
                char bin_str[BYTE_FULL_SIZE] = {0};
                int bin_counter;
                char ch;
                //int diff_counter = 0;

                for(int i = 0; i < strlen(pointer); i++) {
                    ch = pointer[i];
                    ascii_bin = asciiToBinNumber(ch);
                    my_itoa(ascii_bin,bin_str);
                    
                    bin_counter = BYTE_FULL_SIZE - strlen(bin_str);

                    for(int j = 0; j < strlen(bin_str); j++) {
                        data_arr[dc1 + diff][bin_counter] = bin_str[j];
                        bin_counter++;
                    }
                    diff++;
                    
                }
            }

            else 
            { // number

                if(is_substring("-",pointer) == true) {
                    negativNumberToBin(pointer,true,dc1+diff);
                }
                
                else {
                    bin_number = decimalToBinary(pointer_num);
                    my_itoa(bin_number,bin_str);

                    bin_counter = BYTE_FULL_SIZE - strlen(bin_str);
                    for(int i = 0; i < strlen(bin_str); i++) {
                        data_arr[dc1+diff][bin_counter] = bin_str[i];
                        bin_counter++;
                    }
                }
                
            }

            diff++;
            pointer = strtok(NULL,dev);
        }
    }
    
}

/* translateFileToBin(): This fuction gets a name of file in .am 
and knows how to translate every line in file to binary and store the binary code
in code_arr and data_arr. */
bool translateFileToBin(char *file_name) {
    
    FILE *fp = fopen(file_name,"r"); // Pointer to the file
    
    if(fp == NULL) { // Check if file is not exists
        printf("unrecognized file: %s\n",file_name);
        is_errors = false; // reaset the flag for next time
        return true; // Stop the function and return that there was error
    }

    //bool sub2_flag = false;
    reasetArr(); // Reaset the data_arr and code_arr bytes to '0000000000' 
    

    char buffer[MAX_BUFFER_SIZE]; // Define a buffer
    while(feof(fp) == false) 
    {
        fgets(buffer, 1024, fp);

        // Remove the blank spaces (front and back)
        removeSpaceFront(buffer);
        removeSpaceTail(buffer);
 
        int L = sentenceLength(buffer); // The line size in bytes
        char *label_name = isLabel(buffer); // The label name (if there is a label)

        if(is_substring(".extern",buffer) == true) // If it is extern line - continue
            continue;

        if(is_substring(".entry",buffer) == true)
        { // The line is an entry definition
            buffer[0] = ' '; buffer[1] = ' '; buffer[2] = ' '; buffer[3] = ' '; 
            buffer[4] = ' '; buffer[5] = ' '; 
            removeSpaceFront(buffer);

            // If there is no label with this name - print error massege
            bool isExsitingLabel = isInLabelTabel(buffer);
            if(isExsitingLabel == false) {
                printf("Line %d:\tEntry error -> the label '%s' doesn't exists\n\n",line_counter,buffer);
                is_errors = true;
                break; // End the loop because of the enrty label error
            }
            else { // An exisiting label
                
                int label_index = indexInLabelTable(buffer); // The label index
                labelTable[label_index].type = ENTRY_LABEL_TYPE; // Update the label type
                
                // **IGNORE**
                for(int i = 0; i < MAX_LABEL_AMOUNT; i++) {
                    if(labelTable[i].type != LABEL_EXTERN_TYPE)
                        labelTable[i].value -= 0;
                }
                //sub2_flag = true;
                
            }

        }
        else
        {
            if(label_name[0] != '!' && is_substring(".data",buffer) == false
                && is_substring(".string",buffer) == false && is_substring(".struct",buffer) == false)
            { // It is a code label - then remove the label
                int count = 0;
                while(buffer[count] != ':') {
                    buffer[count] = ' ';
                    count++;
                }
                buffer[count] = ' ';
                removeSpaceFront(buffer);
            }

            translateLineToBinary(buffer); // Translate the current line to binary
            
            if(is_substring(".data",buffer) == true || is_substring(".string",buffer) == true
                || is_substring(".struct",buffer) == true) 
            {
                dc1 += L; // Update the date counter
            }
            else 
            {
                ic1 += L; // Update the Instructions counter
            }   
        }
    line_counter++; // Update the line counter 
    }

    // Set the end of the code and data
    code_arr[ic1][0] = 'e'; 
    data_arr[dc1][0] = 'e';

    // Reaset the counters for the other files
    ic1 = COUNTERS_DEFULT_VALUE;
    dc1 = COUNTERS_DEFULT_VALUE;

    if(is_errors == true) {
        is_errors = false; // reaset the flag for the next files
        return true; // there was an error in the .am file
    }
    else {
        return false; // no errors
    }
} // End of function 

/* negativNumberToBin(): This function gets a pointer to string that represent a number, flag
for check if the number should store in the data or code, and offset.
The function translate the negativ number and store him in the right byte
according to the compration to 2 method. */
void negativNumberToBin(char *num, bool isInData, int offset) {

    char num_copy[20];
    strcpy(num_copy,num);

    if(isInData == true) {
        num_copy[0] = ' ';
    }
    else {
        num_copy[0] = ' '; num_copy[1] = ' ';
    }
    removeSpaceFront(num_copy);

    int bin_positive = decimalToBinary(atoi(num_copy));
    char bin_str[10] = {0};
    my_itoa(bin_positive,bin_str);

    if(isInData == true) {
        int bin_count = 10 - strlen(bin_str);
        for(int i = 0; i < strlen(bin_str); i++) {
            data_arr[offset][bin_count] = bin_str[i];
            bin_count++;
        }

    }
    else {
        int bin_count = 8 - strlen(bin_str);
        for(int i = 0; i < strlen(bin_str); i++) {
            code_arr[offset][bin_count] = bin_str[i];
            bin_count++;
        }
    }

    int pointer = 9;    

    if(isInData == true) {
        while(data_arr[offset][pointer] != '1')
            pointer--;
        pointer--;

        for(int j = pointer; j >= 0; j--) {
            if(data_arr[offset][j] == '1')
                data_arr[offset][j] = '0';
            else    
                data_arr[offset][j] = '1';
        }
    }

    else {
        while(code_arr[offset][pointer] != '1')
            pointer--;
        pointer--;

        for(int j = pointer; j >= 0; j--) {
            if(code_arr[offset][j] == '1')
                code_arr[offset][j] = '0';
            else    
                code_arr[offset][j] = '1';
        }
    }
    

    
    return;
}
