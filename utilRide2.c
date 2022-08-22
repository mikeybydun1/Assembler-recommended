/* [mmn 14 : utilRide2.c]:
In this file: helpfull functions for secondRide.c that help
to convert binary number to 32 base numbers.

author: Mikey Bar Yaacov Dunevich | Date: 21.8.2022
ID: 215356981 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#include "utilities.h"
#include "utilRide2.h"
#include "dataUtils.h"

/* binaryTo32Base(): The main functio of this file.
This function gets a pointer to string (with 5 binary chars) and a buffer,
and put in the buffer 32 base chars. */
void binaryTo32Base(char *bin_num, char new_word[]) 
{
    char low_word[] = "00000";
    char high_word[] = "00000";

    low_word[0] = bin_num[0]; low_word[1] = bin_num[1]; low_word[2] = bin_num[2];
    low_word[3] = bin_num[3]; low_word[4] = bin_num[4];

    high_word[0] = bin_num[5]; high_word[1] = bin_num[6]; high_word[2] = bin_num[7];
    high_word[3] = bin_num[8]; high_word[4] = bin_num[9];

    int decimal_low = binaryToDecimal(low_word);
    int decimal_high = binaryToDecimal(high_word);

    char base32_low = decimalTo32Chars(decimal_low);
    char base32_high = decimalTo32Chars(decimal_high);

    new_word[0] = base32_low;
    new_word[1] = base32_high;

}

/* binaryToDecimal(): This function gets a pointer to a string (binary chars)
and return the decimal number in int var. */
int binaryToDecimal(char *string) {
    int slen = strlen(string);
    int total = 0;
    int decval = 1;

    for(int i = (slen-1); i >= 0; i--) {
        if(string[i] == '1')
            total += decval;
        decval *= 2;
    }
    return total;
}

/* decimalTo32Chars(): This function gets a number in decimal,
return return a char that equals to that number in base 32. */
char decimalTo32Chars(int dec_num) {

    if(dec_num == 0) {return '!';} if(dec_num == 1) {return '@';} if(dec_num == 2) {return '#';} if(dec_num == 3) {return '$';}
    if(dec_num == 4) {return '%';} if(dec_num == 5) {return '^';} if(dec_num == 6) {return '&';} if(dec_num == 7) {return '*';}
    if(dec_num == 8) {return '<';} if(dec_num == 9) {return '>';} if(dec_num == 10) {return 'a';} if(dec_num == 11) {return 'b';}
    if(dec_num == 12) {return 'c';} if(dec_num == 13) {return 'd';} if(dec_num == 14) {return 'e';} if(dec_num == 15) {return 'f';}

    if(dec_num == 16) {return 'g';} if(dec_num == 17) {return 'h';} if(dec_num == 18) {return 'i';} if(dec_num == 19) {return 'j';}
    if(dec_num == 20) {return 'k';} if(dec_num == 21) {return 'l';} if(dec_num == 22) {return 'm';} if(dec_num == 23) {return 'n';}
    if(dec_num == 24) {return 'o';} if(dec_num == 25) {return 'p';} if(dec_num == 26) {return 'q';} if(dec_num == 27) {return 'r';}
    if(dec_num == 28) {return 's';} if(dec_num == 29) {return 't';} if(dec_num == 30) {return 'u';} if(dec_num == 31) {return 'v';}

}

