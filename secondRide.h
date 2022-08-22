#include <stdbool.h>
#define MAX_BYTES 2048


void negativNumberToBin(char *num, bool isInData, int offset);
void reasetArr();
void translateLineToBinary(char *line);
bool translateFileToBin(char *file_name);


char code_arr[MAX_BYTES][20];
char data_arr[MAX_BYTES][20];