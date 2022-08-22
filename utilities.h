/* [mmn 14 : utilities.c]:

author: Mikey Bar Yaacov Dunevich | Date: 21.8.2022
ID: 215356981 */

int decimalToBinary(int num);
int getCommandNumber(char command[]);

bool is_substring(char *check, char *string);
void removeSpaceFront(char *s);

void removeSpaceTail(char *s);
char* getCommand(char *s1);

char* getFirstArg(char *s1);
char* getSecondArg(char *s1);

char* removeTillSpace(char *s1);
int amountInString(char *s1, char ch);

char* getRegisterBainry(char *s);
char* getOnlyArg(char *s1);

int asciiToBinNumber(char ch);
