/* [mmn 14 : assembler.c]:
In this file: The main function of the assembler project

author: Mikey Bar Yaacov Dunevich | Date: 21.8.2022
ID: 215356981 */

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdbool.h>

#include "utilities.h"
#include "symbolTable.h"
#include "firstRide.h"
#include "secondRide.h"
#include "utilRide2.h"
#include "debugger.h"
#include "preAssembler.h"
#include "outputFiles.h"
#include "dataUtils.h"

#define LABEL_MAX_AMOUNT 100

// The main function of the project: the function get a .am file names and
// create 3 files - .ob, .ext and .ext file in base 32.
int main(int argc, char* argv[]) {
    
    // The file names
    char file_name[20];
    char ob_file_name[20];
    char ent_file_name[20];
    char ext_file_name[20];
    char bin_file_name[20];


    for(int i = 1; i < argc; i++) {

        // Copy the file name
        strcpy(file_name,argv[i]);
        strcpy(ob_file_name,argv[i]);
        strcpy(ent_file_name,argv[i]);
        strcpy(ext_file_name,argv[i]);
        strcpy(bin_file_name,argv[i]);

        // Add to the file names the file type:
        strcat(file_name,".am");
        strcat(ob_file_name,".ob");
        strcat(ent_file_name,".ent");
        strcat(ext_file_name,".ext");
        strcat(bin_file_name,".bin");
        
        FILE *fp = fopen(file_name,"r"); // The file pointer
        
        // If there is no file with this name - stop the function and return error massege
        if(fp == NULL) {
            printf("File error: There is no file named as '%s'\n",file_name);
            fclose(fp);
            continue;
        }
        fclose(fp);

        printf("\n----- FILE %d: '%s' -----\n",i,argv[i]);

        removeMacro(file_name,"workFile.am"); // PreAssembler - remove and replace the macro in the file using preAssembler.c
        bool no_errors = debug("workFile.am"); // Check if there are errors in the file using debugger.c
        
        
        if(no_errors == true) { // If there is no errors - continue
            ride1("workFile.am"); // Go over the file and give every label a value using firstRide.c
            // Translate the file to binary and check if there was errors during the translation using secondRide.c
            bool is_translate_bin_errors = translateFileToBin("workFile.am"); // 

            // If there wasn't errors during the translation - create object, extern and extry files
            if(is_translate_bin_errors == false) {
                createBinaryFile(bin_file_name);
                createObjectAndExtFile(ob_file_name,ext_file_name);
                createEntFile(ent_file_name);  
                printf("\nThe file '%s' is a leagel assembly file and passed successfully the assembler compilation!\n",argv[i]);
            }

            else { // Print error massege to the user
                printf("There was an error during the translation the .am file to binary.\n");
            }
       
        }

        reasetArr(); // Reaset the data and code arrays
        // Reaset the label table
        for(int j = 0; j < LABEL_MAX_AMOUNT; j++) {
            labelTable[j].name[0] = '\0';
            labelTable[j].value = 0;
            labelTable[j].type = -2;
        }

    }
      
    return 0;
} // End of the function


