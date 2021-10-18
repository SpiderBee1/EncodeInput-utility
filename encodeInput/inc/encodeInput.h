/*
* FILE          : encodeInput.h
* PROJECT       : SENG2030 - Assignment 2
* PROGRAMMER    : Matthew Goodman
* FIRST VERSION : 2021-02-17
* DESCRIPTION :
* this file contains necessary prototypes and definitions
* for functions used in the encodeInput application
*/

#define DATA_RAW_MAX_LENGTH 16          //maximum number of bytes for the data section of the SREC or DC.B formats

//SREC.c prototypes
int sRecordLogic(FILE* input, FILE* output);
unsigned char CalculateSum(unsigned short byteSum);
void writeHex(char* destination, int pos, unsigned char c);

//ASM.c prototypes
int assemblyLogic(FILE* input, FILE* output);
void writeHexASM(char* destination, unsigned char c);
