/* [mmn 14 : preAssembler.h]:
In this file: remove all the macroes in the file and replacing 
thier commands in the right place in the file.

author: Mikey Bar Yaacov Dunevich | Date: 21.8.2022
ID: 215356981 */

/*In this header file: short explanation of all the function*/

/*removeMacro(): This function checks if a string is in the macro table*/
int isInMacroTable(char *s); 

/*isInMacroTable(): This function mapping the macros and replace them*/
void removeMacro(char *fileWithMacro, char *fileWithoutMacro);